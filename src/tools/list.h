/*
 * List
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_TOOLS_LIST_H_
#define SRC_TOOLS_LIST_H_

#include <stdlib.h>
#include <stdint.h>

struct Node;
typedef struct Node {
	struct Node* next;
	struct Node* prev;
	void* value;
} Node_t;

typedef struct {
	uint8_t count;
	Node_t* first;
	Node_t* last;
} List_t;

List_t* list_create();

void list_push(List_t* list, void* value);

void* list_first(List_t* list);

#endif /* SRC_TOOLS_LIST_H_ */
