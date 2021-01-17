CC=gcc
CFLAGS=-ggdb -pedantic-errors -Wall -Werror -Wmissing-prototypes -Wstrict-prototypes -Wextra -Wconversion -std=gnu11

main: main.c pm.o
	$(CC) $(CFLAGS) -o pid_manager main.c pm.o

pm: pm.c 
	$(CC) $(CFLAGS) -c -o pm.o pm.c

clean: 
	rm pm.o pid_manager
