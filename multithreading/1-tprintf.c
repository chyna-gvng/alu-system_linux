#include "multithreading.h"

/**
 * tprintf - program that prints a formatted message with the current
 * thread's ID as a prefix
 * this function is designed for multithreaded programs and prints a message
 * with the current thread's ID followed by the specified formatted content
 * @format: a format string (similar to printf) for the message
 * @...: additional arguments for formatting the message
 * Return: the number of characters printed if successful;
 *         otherwise a negative value, indicating an error
 */

int tprintf(const char *format, ...)
{
	/* Get the current thread's ID */
	pthread_t thread_id = pthread_self();

	/* Print the thread ID and the formatted string */
	int result;
	va_list args;

	va_start(args, format);
	printf("[%lu] ", (unsigned long)thread_id);
	result = vprintf(format, args);
	va_end(args);

	return (result);
}
