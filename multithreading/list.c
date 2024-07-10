#include "list.h"

/**
 * node_create - program that creates a node structure and initializes it
 * @content: the address of the custom content to store in the node
 * Return: a pointer to the created node
 */

node_t *node_create(void *content)
{
	node_t *node = malloc(sizeof(*node));

	node->content = content;
	node->prev = NULL;
	node-> next = NULL;

	return (node);
}

/**
 * list_add - program that creates a node and adds it to the back of a list
 * @list: a pointer to the list to add the node to
 * @content: the address of the custom content to store in the node
 * Return: a pointer to the created node
 */

node_t *list_add(list_t *list, void *content)
{
	node_t *node = node_create(content);

	node->prev = list->tail;

	if (list->tail)
		list->tail->next = node;
	else
		list->head = node;

	list->tail = node;
	++list->size;

	return (node);
}

/**
 * list_init - program that initializes a list structure
 * @list: a pointer to the list to initialize
 * Return: a pointer to the list
 */

list_t *list_init(list_t *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return (list);
}

/**
 * list_destroy - program that destroys the content of a list
 * @list: a pointer to the list structure to destroy the content of
 * @free_func: a pointer to a function to use to free the content of a node
 * Return: nothing (void)
 */

void list_destroy(list_t *list,  node_func_t free_func)
{
	node_t *node;

	for (node = list->head; node;)
	{
		node_t *tmp = node;

		if (free_func)
			free_func(node->content);

		node = node->next;
		free(tmp);
	}
	list->size = 0;
}

/**
 * list_each - program that iterates over a list and calls a function for each node
 * @func: a pointer to a function to call with the content of each node
 * Return: nothing (void)
 */

void list_each(list_t *list, node_func_t func)
{
	node_t *node;

	for (node = list->head; node; node = node->next)
		func(node->content);
}
