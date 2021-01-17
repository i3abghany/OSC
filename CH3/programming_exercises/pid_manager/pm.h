#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MIN_PID 300
#define MAX_PID 5000
#define BITS_PER_BYTE 8
#define BITS_PER_UINT (BITS_PER_BYTE * 4)

#define NO_PID_AVAILABLE 0

static uint32_t *map; 
static uint32_t num_ints; 

int allocate_map(void);
uint32_t allocate_pid(void);
void release_pid(uint32_t);

static uint32_t is_pid_available(uint32_t);
static void reserve_pid(uint32_t);

