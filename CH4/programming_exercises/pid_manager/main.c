#include <stdio.h>
#include <stdint.h>
#include "pm.h"
#define NUM_THREADS 16

void *runner(void *);

int main(void) 
{
	sem_init(&sem, 0, 1);
	sem_init(&sem_r, 0, 1);
	if (allocate_map() != 1) {
		printf("Error initializing the map.\n");
		return 1;
	}	

	pthread_t tids[NUM_THREADS];
	
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tids[i], NULL, runner, NULL);
	}
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(tids[i], NULL);
	}

	sem_destroy(&sem_r);
	sem_destroy(&sem);
	return 0;
}

void *runner(void *p)
{
	int i = (p != NULL);
	uint32_t pid = allocate_pid();
	for (i = 0; i < rand() % 10000; i++);
	release_pid(pid);

	return NULL;
}
