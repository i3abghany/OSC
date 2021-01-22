#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define MIN_PID 300
#define MAX_PID 5000
#define BITS_PER_BYTE 8
#define BITS_PER_UINT (BITS_PER_BYTE * 4)

#define NO_PID_AVAILABLE 0

uint32_t *map; 
uint32_t num_ints; 

extern int allocate_map(void);
extern uint32_t allocate_pid(void);
extern void release_pid(uint32_t);

extern uint32_t is_pid_available(uint32_t);
extern void reserve_pid(uint32_t);

sem_t sem;
sem_t sem_r;
