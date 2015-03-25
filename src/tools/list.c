/*
 * List.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include "list.h"

List_t* list_create()
{
	return (List_t*)malloc(sizeof(List_t));
}

void list_push(List_t* list, void* value)
{
	Node_t* node = malloc(sizeof(Node_t));
	node->value = value;

	if(list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;
}

void* list_first(List_t* list)
{
	Node_t* node = list->first;
	return node;
}
