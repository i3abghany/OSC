#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int nums[] = {1, 2, 3, 4, 5};

int main()
{
	int i; pid_t pid;
	pid = fork();
	if (pid == 0) {
		for (int i = 0; i < 5; i++) {
			nums[i] *= -i;
			printf("CHILD: %d\n", nums[i]);	
		}	
	}
	else if (pid > 0) {
		wait(NULL);
		for (int i = 0; i < 5; i++) {
			printf("PARENT: %d \n", nums[i]);
		}
	}
	return 0;
}
