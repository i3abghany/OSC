#include "pm.h" 

int allocate_map() {
	const uint32_t pid_diff = (MAX_PID - MIN_PID);
	num_ints = pid_diff / BITS_PER_UINT; 

	if (pid_diff % BITS_PER_UINT != 0) num_ints++;

	map = malloc(num_ints * sizeof(uint32_t));

	if (map == NULL) return -1;
	else return 1;
}

uint32_t allocate_pid() {
	for (uint32_t i = MIN_PID; i <= MAX_PID; i++) {
		if (is_pid_available(i)) {
			reserve_pid(i);
			return i;
		}
	}
	return NO_PID_AVAILABLE;
}

uint32_t is_pid_available(uint32_t pid) {
	uint32_t bit_idx = pid % BITS_PER_UINT;
	uint32_t idx = pid / num_ints;
	
	return !((map[idx] >> bit_idx) & 1);
}

void reserve_pid(uint32_t pid) {
	uint32_t bit_idx = pid % BITS_PER_UINT;
	uint32_t idx = pid / num_ints;
	map[idx] |= (1U << bit_idx);
}

void release_pid(uint32_t pid) {
	uint32_t bit_idx = pid % BITS_PER_UINT;
	uint32_t idx = pid / num_ints;
	map[idx] &= ~(1U << bit_idx);
}
