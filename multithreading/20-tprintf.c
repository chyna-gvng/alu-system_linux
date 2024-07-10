#include "multithreading.h"

/**
 * init_mutex - program that initializes the mutex
 * this function is marked with the constructor attribute, which instructs
 * the compiler to run it before any other main function in the program;
 * it initializes a static 'pthread_mutex_t', ensuring it's done only once
 * across all function calls;
 * the static keyword ensures the mutex persists in memory throughout the
 * program's execution, but is only accessible within the scope of this
 * function
 * Return: nothing (void)
 */

__attribute__((constructor))
void init_mutex(void)
{
	static pthread_mutex_t print_mutex;
	static int initialized;

	if (!initialized)
	{
		pthread_mutex_init(&print_mutex, NULL);
		initialized = 1;
	}
}

/**
 * destroy_mutex - program that destroys the mutex
 * this function is marked with the destructor attribute, meaning it will
 * be executed after the main function completes or 'exit()' is called;
 * it destroys the static mutex initialized in the init_mutex function,
 * cleaning up resources to prevent memory leaks;
 * this is especially important for persistent or long-running applications
 * Return: nothing (void)
 */

__attribute__((destructor))
void destroy_mutex(void)
{
	static pthread_mutex_t print_mutex;

	pthread_mutex_destroy(&print_mutex);
}

/**
 * tprintf - a thread-safe printf function
 * this function provides a thread-safe version of printf;
 * it uses a static mutex to ensure exclusive access to the standard
 * output, preventing garbled output when multiple threads attempt
 * to print simultaneously;
 * it locks the mutex before printing and unlocks it afterwards;
 * the function uses 'vfprintf' to format and print the arguments as
 * printf does
 * @format: a string specifying the format to print, similar to printf
 * @...: variadic arguments to be formatted and printed
 * Return: the total number of characters written excluding the null byte
 */

int tprintf(char const *format, ...)
{
	static pthread_mutex_t print_mutex;
	int ret;
	va_list args;

	va_start(args, format);

	pthread_mutex_lock(&print_mutex);
	printf("[%lu] ", pthread_self());
	ret = vfprintf(stdout, format, args);
	pthread_mutex_unlock(&print_mutex);

	va_end(args);

	return (ret);
}
