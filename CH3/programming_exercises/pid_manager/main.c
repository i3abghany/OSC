#include <stdio.h>
#include <stdint.h>

extern uint32_t allocate_map(void);
extern uint32_t allocate_pid(void);
extern void release_pid(uint32_t);

int main(void) {
	if (allocate_map() != 1) {
		printf("Error initializing the map.\n");
		return 1;
	} else {
		printf("Map initialized successfully.\n");
	}
	
	// some elementary tests.
	
	for (int i = 0; i < 20; i++) {
		uint32_t a = allocate_pid();
		printf("Allocated: %d\n", a);
	}

	release_pid(309);
	release_pid(319);

	uint32_t a = allocate_pid();
	printf("Allocated: %d\n", a);

	a = allocate_pid();
	printf("Allocated: %d\n", a);

	return 0;
}
