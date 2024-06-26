#include <Python.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

/**
 * print_python_string - program that prints information
 * about a Python string object
 * this function will print the type, length, and value of a Python string object;
 * if the provided PyObject is not a valid string, it prints an error message
 * Notes:
 * - the function checks if 'p' is a Unicode object and whether
 *   it's in a ready state for processing
 * - it differentiates between compact and ASCII representations
 *   of Python strings
 * @p: PyObject pointer representing the Python string object;
 *     the object must be a valid Unicode object
 * Return: nothing (void)
 */

void print_python_string(PyObject *p)
{
	wchar_t *wchar_buf = NULL;

	printf("[.] string object info\n");

	if (!PyUnicode_Check(p) || !((PyASCIIObject *)p)->state.ready)
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}

	printf("  type: %s%s\n",
	       ((PyASCIIObject *)p)->state.compact ? "compact " : "",
	       ((PyASCIIObject *)p)->state.ascii ? "ascii" : "unicode object");

	printf("  length: %li\n",
	       ((PyASCIIObject *)(p))->length);

	wchar_buf = PyUnicode_AsWideCharString(p, NULL);

	if (wchar_buf != NULL)
	{
		printf("  value: %ls\n", wchar_buf);
		PyMem_Free(wchar_buf);
	}
}
