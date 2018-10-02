#include <stdlib.h>

#include "linkedList.h"

static int  getListLengthRecurse(LinkedListNode* node);
static void clearListStackRecurse(LinkedListNode** node);
static void clearListMallocRecurse(LinkedListNode** node);
static void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode);
static void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode);
static void setNextInList(LinkedListNode* node);
static void setPrevInList(LinkedListNode* node);

LinkedListNode* initNode(void* newValue)
{
    LinkedListNode* newNode;

    newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));

    if (newNode)
    {
        newNode -> next = NULL;
        newNode -> prev = NULL;
        newNode -> value = newValue;
    }

    return newNode;
}

LinkedList* initList()
{
    LinkedList* newList;

    newList = (LinkedList*)malloc(sizeof(LinkedList));

    if (newList)
    {
        newList -> head = NULL;
        newList -> tail = NULL;
    }

    return newList;
}

void insertFirst(LinkedList* list, void* newValue)
{
    LinkedListNode* newNode;

    newNode = initNode(newValue);

    if (list && isEmpty(list))
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

    if (list && isEmpty(list))
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

    if (list && ! isEmpty(list))
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

    if (list && ! isEmpty(list))
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

    if (list && ! isEmpty(list))
    {
        nodeValue = list -> head -> value;
    }

    return nodeValue;
}

void* peekLast(LinkedList* list)
{
    void* nodeValue;

    nodeValue = NULL;

    if (list && ! isEmpty(list))
    {
        nodeValue = list -> tail -> value;
    }

    return nodeValue;
}

int getListLength(LinkedList* list)
{
    LinkedListNode* inList;
    int length;

    inList = NULL;
    length = 0;

    if (list && ! isEmpty(list))
    {
        inList = list -> head;
        length = getListLengthRecurse(inList);
    }

    return length;
}

static int getListLengthRecurse(LinkedListNode* node)
{
    int length;

    length = 0;

    if (node)
    {
        length = 1 + getListLengthRecurse(node -> next);
    }

    return length;
}

void clearListStack(LinkedList** list)
{
    if (*list)
    {
        if (! isEmpty(*list))
        {
            clearListStackRecurse(&((*list) -> head));
        }

        free(*list);
        *list = NULL;
    }
}

static void clearListStackRecurse(LinkedListNode** node)
{
    if (*node)
    {
        clearListStackRecurse(&((*node) -> next));
        free(*node);
        *node = NULL;
    }
}

void clearListMalloc(LinkedList** list)
{
    if (*list)
    {
        if (! isEmpty(*list))
        {
            clearListMallocRecurse(&((*list) -> head));
        }

        free(*list);
        *list = NULL;
    }
}

static void clearListMallocRecurse(LinkedListNode** node)
{
    if (*node)
    {
        clearListMallocRecurse(&((*node) -> next));
        if ((*node) -> value)
        {
            free((*node) -> value);
            (*node) -> value = NULL;
        }
        free(*node);
        *node = NULL;
    }
}

static void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode)
{
    if (node && nextNode)
    {
        node -> next = nextNode;
    }
}

static void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode)
{
    if (node && prevNode)
    {
        node -> prev = prevNode;
    }
}

static void setNextInList(LinkedListNode* node)
{
    if (node)
    {
        node = node -> next;
    }
}

static void setPrevInList(LinkedListNode* node)
{
    if (node)
    {
        node = node -> prev;
    }
}

int isEmpty(LinkedList* list)
{
    return (list && list -> head == NULL && list -> tail == NULL);
}
