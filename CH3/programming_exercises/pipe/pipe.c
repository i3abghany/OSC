#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {
	char write_msg[BUF_SIZE];
	char read_msg[BUF_SIZE];

	strcpy(write_msg, "Greetings, sir!");
	
	int fd[2];
	pid_t pid;
	if (pipe(fd) == -1) {
		perror("pipe");
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		perror("fork");
		return 1;	
	}

	if (pid > 0) {
		close(fd[READ_END]);

		ssize_t w = write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
		printf("%lu bytes were written to the pipe.\n", w);

		close(fd[WRITE_END]);
		int p = wait(NULL);
		if (p < 0) {
			perror("wait");
			return 1;
		}
	} else {
		close(fd[WRITE_END]);

		ssize_t c = read(fd[READ_END], read_msg, BUF_SIZE);
		printf("%lu bytes were read from the pipe: %s\n", c, read_msg);

		close(fd[READ_END]);
	}
	return 0;
}
