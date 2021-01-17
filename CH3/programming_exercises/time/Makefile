CC=gcc
CFLAGS=-O3 -pedantic-errors -Werror -Wall -Wmissing-prototypes -Wstrict-prototypes -Wextra

time_a: time.o
	$(CC) $(CFLAGS) time.o -o time
	rm time.o

time_c: time.c
	$(CC) $(CFLAGS) -c time.c -o time.o

clean:
	rm time
