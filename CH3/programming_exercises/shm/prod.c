#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	const size_t sz = 4096;
	const char *name = "/OS";

	const char *msg_0 = "Hello";
	const char *msg_1 = "World!";

	int fd;

	char *ptr;

	fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	ftruncate(fd, sz);

	ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	sprintf(ptr, "%s", msg_0);
	ptr += strlen(msg_0);

	sprintf(ptr, "%s", msg_1);
	ptr += strlen(msg_1);

	sleep(10);
	
	return 0;
}
