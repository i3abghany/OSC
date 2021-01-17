CC=gcc
CFLAGS=-O3 -Wall -Werror -Wstrict-prototypes -Wextra -Wmissing-prototypes

pipe_a: pipe.o
	$(CC) $(CFLAGS) -o pipe pipe.o
	rm pipe.o


pipe_c: pipe.c
	$(CC) $(CFLAGS) -c -o pipe.o

clean:
	rm pipe

