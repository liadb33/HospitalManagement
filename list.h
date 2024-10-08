#ifndef _LIST_
#define _LIST_

#include "def.h"

typedef struct 
{
	NODE head;
}LIST;

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, DATA Value);	// add new node after *pNode

BOOL L_delete(NODE* pNode, void (*freeKey)(void*));				// erase node after *pNode

const NODE* L_find(const NODE* pNode, DATA Value, int (*compare)(const void*, const void*));		// return a pointer to the node 

BOOL L_free(LIST* pList, void (*freeKey)(void*));					// free list memory

int L_print(const LIST* pList, void (*print)(const void*));					// print the list content

int L_length(const LIST* pList);					// count nodes


#endif
