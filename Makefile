CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -g -ggdb

main: main.o
	$(CC) $(CFLAGS) $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o main

