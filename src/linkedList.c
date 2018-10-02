#include <stdlib.h>

#include "linkedList.h"

static void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode);
static void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode);
static void setNextInList(LinkedListNode* node);
static void setPrevInList(LinkedListNode* node);

LinkedListNode* initNode(void* newValue)
{
    LinkedListNode* newNode;
    newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));

    newNode -> next = NULL;
    newNode -> prev = NULL;
    newNode -> value = newValue;

    return newNode;
}

LinkedList* initList()
{
    LinkedList* newList;

    newList = (LinkedList*)malloc(sizeof(LinkedList));

    newList -> head = NULL;
    newList -> tail = NULL;

    return newList;
}

void insertFirst(LinkedList* list, void* newValue)
{
    LinkedListNode* newNode;

    newNode = initNode(newValue);

    if (isEmpty(list))
    {
        list -> head = newNode;
        list -> tail = newNode;
    }
    else
    {
        setPrevToNode(list -> head, newNode);
        setNextToNode(newNode, list -> head);
        list -> head = newNode;
    }
}

void insertLast(LinkedList* list, void* newValue)
{
    LinkedListNode* newNode;

    newNode = initNode(newValue);

    if (isEmpty(list))
    {
        list -> head = newNode;
        list -> tail = newNode;
    }
    else
    {
        setPrevToNode(newNode, list -> tail);
        setNextToNode(list -> tail, newNode);
        list -> tail = newNode;
    }
}

LinkedListNode* removeFirst(LinkedList* list)
{
    LinkedListNode* remove;

    remove = NULL;

    if (! isEmpty(list))
    {
        remove = list -> head;
        if (list -> head == list -> tail)
        {
            list -> head = NULL;
            list -> tail = NULL;
        }
        else
        {
            setNextInList(list -> head);
        }
    }

    return remove;
}

LinkedListNode* removeLast(LinkedList* list)
{
    LinkedListNode* remove;

    remove = NULL;

    if (! isEmpty(list))
    {
        remove = list -> tail;
        if (list -> head == list -> tail)
        {
            list -> head = NULL;
            list -> tail = NULL;
        }
        else
        {
            setPrevInList(list -> tail);
        }
    }

    return remove;
}

void* peekFirst(LinkedList* list)
{
    void* nodeValue;

    nodeValue = NULL;

    if (! isEmpty(list))
    {
        nodeValue = list -> head -> value;
    }

    return nodeValue;
}

void* peekLast(LinkedList* list)
{
    void* nodeValue;

    nodeValue = NULL;

    if (! isEmpty(list))
    {
        nodeValue = list -> tail -> value;
    }

    return nodeValue;
}

void clearListStack(LinkedList** list)
{
    LinkedListNode* inList;

    inList = NULL;

    if (*list)
    {
        if (! isEmpty(*list))
        {
            while ((*list) -> head)
            {
                inList = (*list) -> head;
                (*list) -> head = (*list) -> head -> next;
                free(inList);
                inList = NULL;
            }
        }

        free(*list);
        *list = NULL;
    }
}

void clearListMalloc(LinkedList** list)
{
    LinkedListNode* inList;

    inList = NULL;

    if (*list)
    {
        if (! isEmpty(*list))
        {
            while ((*list) -> head)
            {
                inList = (*list) -> head;
                (*list) -> head = (*list) -> head -> next;
                free(inList -> value);
                inList -> value = NULL;
                free(inList);
                inList = NULL;
            }
        }

        free(*list);
        *list = NULL;
    }
}

void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode)
{
    node -> next = nextNode;
}

void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode)
{
    node -> prev = prevNode;
}

void setNextInList(LinkedListNode* node)
{
    node = node -> next;
}

void setPrevInList(LinkedListNode* node)
{
    node = node -> prev;
}

int isEmpty(LinkedList* list)
{
    return (list && list -> head == NULL && list -> tail == NULL);
}
