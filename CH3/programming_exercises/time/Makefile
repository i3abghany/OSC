CC=gcc
CFLAGS=-O3 -pedantic-errors -Werror -Wall -Wmissing-prototypes -Wstrict-prototypes -Wextra

pipe: pipe_c
	$(CC) $(CFLAGS) time.o -o time
	rm time.o

pipe_c: time_pipe.c
	$(CC) $(CFLAGS) -c time_pipe.c -o time.o

shm: shm_c
	$(CC) $(CFLAGS) time.o -o time -lrt
	rm time.o

shm_c: time_shm.c
	$(CC) $(CFLAGS) -c time_shm.c -o time.o -lrt
	
clean:
	rm time
