#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define VADDR_WIDTH        16
#define PADDR_WIDTH        16
#define OFFSET_FIELD_WIDTH 8 
#define PAGE_FIELD_WIDTH   (VADDR_WIDTH - OFFSET_FIELD_WIDTH)
#define PAGE_SIZE          (1 << OFFSET_FIELD_WIDTH)

#define OFFSET_MASK 0xFF
#define PAGE_MASK 0xFF00

#define OFFSET(x) ((uint8_t) (x & OFFSET_MASK))
#define PAGE_NO(x) ((uint8_t) ((x & PAGE_MASK) >> OFFSET_FIELD_WIDTH))

#define ADDR(page, off) (((uint16_t) page) << OFFSET_FIELD_WIDTH) | off

#define PTE_NUM (1 << PAGE_FIELD_WIDTH)

#define TLB_SIZE 16

struct pte_t {
    uint8_t frame_no;
    bool valid;
};

struct page_table_t {
    struct pte_t ptes[PTE_NUM];
};

struct mem_t {
    uint8_t bytes[1 << PADDR_WIDTH];
};

struct tlb_t {
    uint8_t map[TLB_SIZE][2];
    bool valid[TLB_SIZE];
    uint64_t lru_priority[TLB_SIZE];
    uint64_t lru_gen;
};

struct tlb_res {
    uint16_t frame;
    bool valid;
};

static struct mem_t gmem;
static struct tlb_t *gtlb;

static uint64_t page_faults;
static uint64_t tlb_misses;
static uint64_t translations;

struct page_table_t *table_init(size_t pte_num) {
    struct page_table_t *table = malloc(sizeof (struct page_table_t));

    for (int i = 0; i < pte_num; i++) {
        table->ptes[i].frame_no = 0x00;
        table->ptes[i].valid = false;
    }

    return table;
}

struct tlb_t *tlb_init() {
    struct tlb_t *tlb = malloc(sizeof (struct tlb_t));
    
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb->map[i][0] = 0x0000;
        tlb->map[i][1] = 0x0000;
        tlb->valid[i] = false;
        tlb->lru_priority[i] = 0x0000;
    }

    tlb->lru_gen = 0;

    return tlb;
}

uint8_t tlb_lru_entry(struct tlb_t *tlb) {
    uint64_t min_priority = ULLONG_MAX;
    uint8_t ret = 0;
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->lru_priority[i] < min_priority) {
            min_priority = tlb->lru_priority[i];
            ret = i;
        }
    }

    return ret;
}

int8_t get_invalid_entry(struct tlb_t *tlb) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tlb->valid[i]) {
            return i;
        }
    }

    return -1;
}

uint8_t tlb_get_insertion_index(struct tlb_t *tlb) {
    int8_t ret = get_invalid_entry(tlb);

    if (ret != -1) {
        return ret;
    } else {
        return tlb_lru_entry(tlb);
    }
}

uint8_t tlb_update_priority(struct tlb_t *tlb, uint8_t used_idx) {
    tlb->lru_priority[used_idx] = tlb->lru_gen++;
}

struct tlb_res tlb_translate(struct tlb_t *tlb, uint8_t page_no) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->map[i][0] == page_no && tlb->valid[i]) {
            tlb_update_priority(tlb, i);
            return (struct tlb_res){ tlb->map[i][1], true };
        }
    }

    return (struct tlb_res){ 0x00, false };
}

void tlb_insert(struct tlb_t *tlb, uint8_t page_no, uint8_t frame_no) {
    uint8_t idx = tlb_get_insertion_index(tlb);
    tlb->map[idx][0] = page_no;
    tlb->map[idx][1] = frame_no;
    tlb->valid[idx] = true;
    tlb_update_priority(tlb, idx);
}

void page_fault_handler(struct page_table_t *table, uint16_t vaddr) {
    int fd = open("BACKING_STORE.bin", 0);
    int moff = PAGE_NO(vaddr) << OFFSET_FIELD_WIDTH;

    page_faults++;

    if (lseek(fd, moff, SEEK_SET) == -1) {
        perror("lseek");
    }

    if (read(fd, gmem.bytes + moff, PAGE_SIZE) == -1) {
        perror("read");
    }

    table->ptes[PAGE_NO(vaddr)].valid = true;
    table->ptes[PAGE_NO(vaddr)].frame_no = PAGE_NO(vaddr);
    close(fd);
}

uint16_t translate(struct page_table_t *table, uint16_t vaddr) {
    uint8_t page_no = PAGE_NO(vaddr);
    uint8_t offset  = OFFSET(vaddr);
    struct tlb_res tr;
    uint16_t ret;
    struct pte_t *entry;

    translations++;
    tr = tlb_translate(gtlb, page_no);

    if (tr.valid) {
        return (tr.frame << OFFSET_FIELD_WIDTH) | offset;
    }

    tlb_misses++;

    entry = &table->ptes[page_no];
    if (entry->valid) {
        ret = ADDR(entry->frame_no, offset);
        tlb_insert(gtlb, page_no, PAGE_NO(ret));
        return ret;
    } else {
        page_fault_handler(table, vaddr);
        return translate(table, vaddr);
    }
}

int main() {
    struct page_table_t *table;
    FILE *fp;
    int32_t vaddr;
    uint16_t haddr;
    gtlb = tlb_init();
    table = table_init(PTE_NUM);

    if ((fp = fopen("addr.txt", "r")) == NULL) {
        perror("fopen");
    }

    while (fscanf(fp, "%d", &vaddr) && vaddr != -1) {
        uint16_t haddr = translate(table, vaddr & 0xFFFF);
        printf("0x%04x: 0x%02x\n", vaddr & 0xFFFF, gmem.bytes[haddr]);
    }

    printf("translations: %lu\n", translations);
    printf("page faults: %lu\n", page_faults);
    printf("tlb misses: %lu\n", tlb_misses);

    fclose(fp);
}
