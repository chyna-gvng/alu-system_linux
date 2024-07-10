#include "multithreading.h"

/**
 * thread_entry - the entry point for a pthread
 * this function serves as the entry point for a pthread;
 * it takes a single argument 'arg', which can be used to pass data
 * or configuration information to the thread;
 * in this specific implementation, it expects 'arg' to be a pointer to
 * a null-terminated string (char *), and it simply prints that string
 * to the standard output;
 * once the string has been printed, the thread exits using pthread_exit
 * @arg: a pointer to an argument that can be passed to the thread
 * Return: nothing (void)
 */

void *thread_entry(void *arg)
{
	if (!arg)
		pthread_exit(NULL);

	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
}
