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
	sem_wait(&sem);
	for (uint32_t i = MIN_PID; i <= MAX_PID; i++) {
		if (is_pid_available(i)) {
			reserve_pid(i);
			printf("Thread %ld allocated pid [%u]\n", syscall(SYS_gettid), i);
			sem_post(&sem);
			return i;
		}
	}
	sem_post(&sem);
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
	sem_wait(&sem_r);
	uint32_t bit_idx = pid % BITS_PER_UINT;
	uint32_t idx = pid / num_ints;
	map[idx] &= ~(1U << bit_idx);
	printf("Thread %ld released pid [%u]\n", syscall(SYS_gettid), pid);
	sem_post(&sem_r);
}
