#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * Struct Name: LinkedListNode
 * Purpose:     A node that is part of a linked list. Contains two
 *              pointer to another node, one for previous node and
 *              one for next node. By default, both will point to null.
 *              This node is node is designed to be doubly linked
 *
 *          +-----------+           +-----------+
 *          |           |   next    |           |
 *          |           | --------> |           |
 * Null <-- |   node    |           |   node    | --> Null
 *          |           | <-------- |           |
 *          |           |   prev    |           |
 *          +-----------+           +-----------+
 *
 **/

typedef struct LinkedListNode
{
    struct LinkedListNode* next;
    struct LinkedListNode* prev;
    void* value;
} LinkedListNode;

/**
 * Struct Name: LinkedList
 * Purpose:     A "wrapper" struct for containing linked list nodes.
 *              It's essentially providing a pointer to the start and
 *              the end of a list. This list is designed to be double
 *              ended
 *
 *              +-----------+           +-----------+
 *     Null <-- |           |   next    |           | --> Null
 *              |           | --------> |           |
 * +------+     |   node    |           |   node    |     +------+
 * | head | --> |           | <-------- |           | <-- | tail |
 * +------+     |           |   prev    |           |     +------+
 *              +-----------+           +-----------+
 *
 **/

typedef struct LinkedList
{
    LinkedListNode* head;
    LinkedListNode* tail;
} LinkedList;

/* Function prototypes */
LinkedListNode* initNode(void* newValue);
LinkedList* initList();
void insertFirst(LinkedList* list, void* newValue);
void insertLast(LinkedList* list, void* newValue);
LinkedListNode* removeFirst(LinkedList* list);
LinkedListNode* removeLast(LinkedList* list);
void* peekFirst(LinkedList* list);
void* peekLast(LinkedList* list);
int getListLength(LinkedList* list);
void clearListStack(LinkedList** list);
void clearListMalloc(LinkedList** list);
int isEmpty(LinkedList* list);

#endif /* End of linkedList.h */
