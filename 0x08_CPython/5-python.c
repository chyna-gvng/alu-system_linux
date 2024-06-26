#include <Python.h>
#include <stdio.h>
#include <stdbool.h>

#define _ABS(x) ((x) < 0 ? -(x) : (x))

/**
 * print_python_int - program that prints a Python integer
 * this function translates a Python integer (PyLongObject)
 * into a C unsigned long int and prints it;
 * it checks if the input PyObject is a valid Python integer
 * and handles large integers that exceed the capacity of
 * a C unsigned long int
 * Error handling:
 * - if the input is not a valid Python integer, prints "Invalid Int Object"
 * - if the integer's absolute value is too large to fit in an unsigned long int,
 *   prints "C unsigned long int overflow"
 * @p: PyObject pointer representing the Python integer object
 * Return: nothing (void)
 */

void print_python_int(PyObject *p)
{
	Py_ssize_t i, size;
	bool negative;
	unsigned long base10 = 0, pow_base_pylong;
	unsigned int base_pylong = 1 << PyLong_SHIFT;

	if (!PyLong_Check(p))
	{
		puts("Invalid Int Object");
		return;
	}

	size = _ABS(((PyVarObject *)p)->ob_size);
	negative = ((PyVarObject *)p)->ob_size < 0;

	if (size < 3 || (size == 3 &&
			 ((PyLongObject *)p)->ob_digit[2] < 16))
	{
		pow_base_pylong = 1;

		for (i = 0; i < size; i++)
		{
			base10 += pow_base_pylong *
				  ((PyLongObject *)p)->ob_digit[i];
			pow_base_pylong *= base_pylong;
		}

		if (negative)
			putchar('-');
		printf("%lu\n", base10);
	}
	else
		puts("C unsigned long int overflow");
}
