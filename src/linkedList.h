#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedListNode LinkedListNode;
typedef struct LinkedList LinkedList;

struct LinkedListNode
{
    struct LinkedListNode* next;
    struct LinkedListNode* prev;
    void* value;
};

struct LinkedList
{
    LinkedListNode* head;
    LinkedListNode* tail;
};

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

#endif
