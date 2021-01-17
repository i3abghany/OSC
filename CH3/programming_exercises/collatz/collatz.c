#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>

int next_collatz(int n) {
	if (n % 2 == 0) return n / 2;
	return n * 3 + 1;
}

int main() {
	int n;
	const size_t SIZE = 4096;
	const char *sh_name = "/collatz_sh";

	scanf("%d", &n);
	pid_t pid;

	int fd;
	fd = shm_open(sh_name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, SIZE);

	char *ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (fork() == 0) {
		wait(NULL);
		 while(n != 1) {
		 	sscanf(ptr, "%d", &n);
		 	printf("%d ", n);
			char str[16]; sprintf(str, "%d", n);
			ptr += strlen(str) + 1;
		 }
		shm_unlink(sh_name);

	} else {
		while(n != 1) {
			n = next_collatz(n);
			char str[32];
			sprintf(str, "%d ", n);
			strcat(ptr, str);
		}
	}
}
