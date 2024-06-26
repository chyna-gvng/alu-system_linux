#include <Python.h>
#include <stdio.h>

#define MAX_SIZE 15

/**
 * print_python_int - program that prints the integer value of a Python integer object
 * this function interprets the internal representation of a Python integer
 * (`PyObject`) and prints its equivalent C unsigned long integer value;
 * it handles different scenarios like zero, positive, and negative integers;
 * for large integers, it checks for overflow conditions specific to
 * C's unsigned long int representation
 * @p: a PyObject pointer, expected to point to a Python integer object
 * Return: nothing (void)
 */

void print_python_int(PyObject *p)
{
	unsigned long int num = 0;
	int size, i, size_bk;

	if (PyLong_Check(p))
	{
		size = (int) ((PyVarObject *)p)->ob_size;
		size_bk = size;

		if (size == 0)
		{
			printf("%d\n", 0), fflush(stdout);
			return;
		}
		if (size == 1)
		{
			printf("%lu\n", (unsigned long int)((PyLongObject *)p)->ob_digit[0]);
			fflush(stdout);
			return;
		}
		if (size < 0)
			size_bk = -size;

		for (i = 0; i < size_bk; i++)
		{
			num += ((PyLongObject *)p)->ob_digit[i] * (1UL << (PyLong_SHIFT * i));

			if (size_bk > 3 || (size == 3 && num == 0))
			{
				printf("C unsigned long int overflow\n");
				fflush(stdout);
				return;
			}
		}
		if (size < 0)
			printf("-");

		printf("%lu\n", num);
		fflush(stdout);
	}
	else
	{
		printf("Invalid Int Object\n");
		fflush(stdout);
	}
}
