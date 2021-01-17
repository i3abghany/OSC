#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int i; pid_t pid;
	pid = fork();
	if (pid > 0) {
		sleep(10);
		wait(NULL);
	}
	return 0;
}
