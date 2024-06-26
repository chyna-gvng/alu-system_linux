#include <Python.h>
#include <stdio.h>

/**
 * print_python_bytes - program that prints detailed information
 * about a Python bytes object
 * this function reports the size and first few bytes of the Python bytes object;
 * if the object is not a valid PyBytesObject, an error message is printed
 * the output includes:
 * - the size of the bytes object
 * - the actual bytes up to a maximum of 10 bytes, regardless of the size
 * @p: PyObject pointer representing the Python bytes object
 * Return: nothing (void)
 */

void print_python_bytes(PyObject *p)
{
	int i;
	Py_ssize_t size, printed_bytes;
	char *array_as_string = NULL;

	printf("[.] bytes object info\n");

	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	size = ((PyVarObject *)(p))->ob_size;

	printf("  size: %li\n", size);

	array_as_string = ((PyBytesObject *)(p))->ob_sval;

	printf("  trying string: %s\n", array_as_string);

	printed_bytes = (size + 1 >= 10) ? 10 : size + 1;

	printf("  first %li bytes:", printed_bytes);

	for (i = 0; i < printed_bytes; i++)
		printf(" %02x", (unsigned char)(array_as_string[i]));
	putchar('\n');
}

/**
 * print_python_list - program that prints detailed information
 * about a Python list object
 * this function reports the size and elements of the list;
 * if an element is a bytes object, detailed bytes information is also printed
 * the output includes:
 * - the size of the list
 * - the memory allocation size
 * - the type of each element in the list
 * @p: PyObject pointer representing the Python list object
 * Return: nothing (void)
 */

void print_python_list(PyObject *p)
{
	int i;
	Py_ssize_t size;
	PyObject *list_member;

	printf("[*] Python list info\n");

	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}

	size = ((PyVarObject *)(p))->ob_size;

	printf("[*] Size of the Python List = %li\n", size);

	printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);

	for (i = 0; i < size; i++)
	{
		list_member = ((PyListObject *)p)->ob_item[i];
		printf("Element %d: %s\n", i,
		       list_member->ob_type->tp_name);

		if (PyBytes_Check(list_member))
			print_python_bytes(list_member);
	}
}
