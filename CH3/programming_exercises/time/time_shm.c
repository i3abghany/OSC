#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Supply an executable as a command line argument.\n");	
		return 1;
	}
	const char *prog = argv[1];
 
	char *new_argv[argc]; 
	new_argv[argc - 1] = NULL;
	for (int i = 1; i < argc; i++) {
		new_argv[i - 1] = argv[i];
	}

	const size_t SIZE = 4096;
	const char *fname = "/timespec_shm";
	int fd;
	fd = shm_open(fname, O_CREAT | O_RDWR, 0666);
	if (ftruncate(fd, SIZE) != 0) {
		fprintf(stderr, "Could not truncate the shared memory.");	
		return 1;
	}
	struct timespec *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
							fd, 0);
	

	pid_t pid;
	pid = fork();
	if (pid < 0) {
		perror("fork");	
		return 1;
	}
	else if (pid == 0) {
		struct timespec *tp = malloc(sizeof(struct timespec));
		if (clock_gettime(CLOCK_MONOTONIC, tp)) {
			fprintf(stderr, "Clock naccessible in the child.");
			return 1;
		}
		snprintf((char *)ptr, sizeof(struct timespec), "%s", (char *) tp);
		execvp(prog, new_argv);
		perror("execvp");
		return 1;
	} else {
		struct timespec tp0, tp1;
		int stat;
		wait(&stat);
		if (stat != 0) {
			fprintf(stderr, "An error occurred with executing the program.\n");
			return 1;
		}
		tp0 = *ptr;
		if (clock_gettime(CLOCK_MONOTONIC, &tp1) == -1) {
			fprintf(stderr, "Clock naccessible in the parent.");	
			return 1;
		}
		long ms = (tp1.tv_nsec - tp0.tv_nsec) / 1000000L;
		long sec = (tp1.tv_sec - tp0.tv_sec);
		printf("Execution took: %ld [s] %ld [ms] \n", sec, ms);
		
		shm_unlink("/timespec_shm");

	}
}
