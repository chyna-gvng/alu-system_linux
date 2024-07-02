CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# task 0
0-strace.o: 0_strace.c
	$(CC) -c $(CFLAGS) 0_strace.c

strace_0: syscalls.h 0_strace.o
	$(CC) $(CFLAGS) syscalls.h 0_strace.o -o strace_0

# task 1
1_strace.o: 1_strace.c
	$(CC) -c $(CFLAGS) 1_strace.c

strace_1: syscalls.h 1_strace.o
	$(CC) $(CFLAGS) syscalls.h 1_strace.o -o strace_1

# task 2
2_strace.o: 2_strace.c
	$(CC) -c $(CFLAGS) 2_strace.c

strace_2: syscalls.h 2_strace.o
	$(CC) $(CFLAGS) syscalls.h 2_strace.o -o strace_2

# task 3
3_strace.o: 3_strace.c
	$(CC) -c $(CFLAGS) 3_strace.c

strace_3: syscalls.h 3_strace.o
	$(CC) $(CFLAGS) syscalls.h 3_strace.o -o strace_3

clean:
	rm -f strace_0
