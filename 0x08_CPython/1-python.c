#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - program that prints information about a Python list
 *
 * this function prints various pieces of information about the Python list,
 * including its size, the number of allocated slots for elements,
 * and the type names of each element in the list
 *
 * @p: a pointer to the PyObject representing a Python list
 *
 * Return: nothing (void)
 *
 */

void print_python_list(PyObject *p)
{
	int i;

    	printf("[*] Python list info\n");

    	printf("[*] Size of the Python List = %lu\n", Py_SIZE(p));

    	printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);

    	for (i = 0; i < Py_SIZE(p); i++)
    	{
        	printf("Element %d: %s\n", i, Py_TYPE(PyList_GetItem(p, i))->tp_name);
    	}
}