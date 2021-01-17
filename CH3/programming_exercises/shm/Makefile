CC=gcc
CFLAGS=-g -Wall -Werror -Wstrict-prototypes -Wextra -Wmissing-prototypes

all: prod_a cons_a

prod_a: prod.o
	$(CC) $(CFLAGS) prod.o -o prod -lrt
	rm prod.o

prod_c: prod.c
	$(CC) $(CFLAGS) prod.c -c -o prod.o

cons_a: cons.o
	$(CC) $(CFLAGS) cons.o -o cons -lrt
	rm cons.o

cons_c: cons.c
	$(CC) $(CFLAGS) cons.c -c -o cons.o

clean: 
	rm prod cons
