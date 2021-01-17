#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
	const size_t size = 4096;
	const char *name = "/OS";

	int fd; 
	char *ptr;
	fd = shm_open(name, O_RDONLY, 0666);	

	ptr = (char *)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	printf("%s", ptr);
	shm_unlink(name);
	return 0;
}
