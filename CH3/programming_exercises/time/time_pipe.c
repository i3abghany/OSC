#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

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

	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		fprintf(stderr, "Pipe failure.");
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");	
		return 1;
	}
	else if (pid == 0) {
		close(fd[READ_END]); // unused end of pipe.
		struct timespec *tp = malloc(sizeof(struct timespec));
		if (clock_gettime(CLOCK_MONOTONIC, tp)) {
			fprintf(stderr, "Clock naccessible in the child.");
			return 1;
		}
		ssize_t w = write(fd[WRITE_END], (char *)tp, sizeof(*tp));
		if (w == 0) {
			printf("CHILD: could not write to the pipe.");
			return 1;	
		}
		execvp(prog, new_argv);
		perror("execvp");
		return 1;
	} else {
		close(fd[WRITE_END]);
		struct timespec tp0, tp1;
		int r = read(fd[READ_END], &tp0, sizeof(tp0));
		if (r == 0) {
			fprintf(stderr, "PARENT: could not read from the pipe.");
			return 1;
		}
		int stat;
		wait(&stat);
		if (stat != 0) {
			fprintf(stderr, "An error occurred with executing the program.\n");
			return 1;
		}
		if (clock_gettime(CLOCK_MONOTONIC, &tp1) == -1) {
			fprintf(stderr, "Clock naccessible in the parent.");	
			return 1;
		}
		long ms = (tp1.tv_nsec - tp0.tv_nsec) / 1000000L;
		long sec = (tp1.tv_sec - tp0.tv_sec);
	printf("Execution took: %ld [s] %ld [ms] \n", sec, ms);

	}
}
