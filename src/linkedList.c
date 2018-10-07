/**
 * Filename: linkedList.c
 * Author:   Juilan Heng (19473701)
 * Purpose:  Provide the initialisation of a doubly linked double ended
 *           linked list as well as several utility functions.
 *
 * Last Modified: 2018-10-07T20:07:22+08:00
 **/

#include <stdlib.h>

#include "linkedList.h"

#define FALSE 0
#define TRUE !FALSE

/* Static functions */
static int  getListLengthRecurse(LinkedListNode* node);
static void clearListStackRecurse(LinkedListNode** node);
static void clearListMallocRecurse(LinkedListNode** node);
static void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode);
static void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode);
static void setNextInList(LinkedListNode* node);
static void setPrevInList(LinkedListNode* node);

/**
 * Name:     initNode
 * Purpose:
 *     Create a linked list node to be inserted into a linked list.
 *
 * Parameters:
 *   - newValue : A void pointer to a memory address to any kind of
 *                of variable to be stored in a linked list
 *
 * Returns:  LinkedListNode pointer
 * Assertions:
 *     Assumptions:
 *         newValue is not NULL
 *     Results:
 *         Return the memory address of the linked list node containing
 *         the newValue passed
 **/

LinkedListNode* initNode(void* newValue)
{
    LinkedListNode* newNode;

    /* Allocate memory in the heap for a linked list node */
    newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));

    /* Check if malloc failed */
    if (newNode)
    {
        /* Set default values */
        newNode -> next = NULL;
        newNode -> prev = NULL;
        newNode -> value = newValue;
    }

    return newNode;
}

/**
 * Name:     initList
 * Purpose:
 *     Create a linked list with head and tail pointing to NULL
 *
 * Parameters: None
 *
 * Returns: LinkedList pointer
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Return the memory address of a linked list
 **/

LinkedList* initList()
{
    LinkedList* newList;

    /* Allocate memory in the heap for a linked list */
    newList = (LinkedList*)malloc(sizeof(LinkedList));

    /* Check if malloc failed */
    if (newList)
    {
        /* Set default values */
        newList -> head = NULL;
        newList -> tail = NULL;
    }

    return newList;
}

/**
 * Name:     insertFirst
 * Purpose:
 *     Insert a linked list node to the head of the linked list
 *
 * Parameters:
 *   - list     : A pointer to a linked list
 *   - newValue : A void pointer to a variable to add to the linked list
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         newValue is not NULL
 *         list is not NULL
 *     Results:
 *         A new node will be allocated in the heap and be placed on the
 *         head of the linked list
 **/

void insertFirst(LinkedList* list, void* newValue)
{
    LinkedListNode* newNode;

    /* Create a new node with newValue */
    newNode = initNode(newValue);

    /* Special case for empty list */
    if (list && isEmpty(list))
    {
        list -> head = newNode;
        list -> tail = newNode;
    }
    else
    {
        /**
         * Set the previous pointer in the current head to newNode,
         * then set the next in newNode to current head.
         * Set the head to newNode
         **/
        setPrevToNode(list -> head, newNode);
        setNextToNode(newNode, list -> head);
        list -> head = newNode;
    }
}

/**
 * Name:     insertLast
 * Purpose:
 *     Insert a linked list node to the tail of the linked list
 *
 * Parameters:
 *   - list     : A pointer to a linked list
 *   - newValue : A void pointer to a variable to add to the linked list
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         newValue is not NULL
 *         list is not NULL
 *     Results:
 *         A new node will be allocated in the heap and be placed on the
 *         tail of the linked list
 **/

void insertLast(LinkedList* list, void* newValue)
{
    LinkedListNode* newNode;

    /* Create a new node with newValue */
    newNode = initNode(newValue);

    /* Special case for empty list */
    if (list && isEmpty(list))
    {
        list -> head = newNode;
        list -> tail = newNode;
    }
    else
    {
        /**
         * Set the previous pointer in newNode to point to the
         * current tail, then set the next in current tail to
         * newNode. Then set newNode as the new tail
         **/
        setPrevToNode(newNode, list -> tail);
        setNextToNode(list -> tail, newNode);
        list -> tail = newNode;
    }
}

/**
 * Name:     removeFirst
 * Purpose:
 *     Remove a linked list node from the head of the linked list
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  LinkedListNode pointer
 * Assertions:
 *     Assumptions:
 *         list is not NULL
 *     Results:
 *         Return the memory address of the linked list node and set the
 *         head to the next node
 **/

LinkedListNode* removeFirst(LinkedList* list)
{
    LinkedListNode* remove;

    remove = NULL;

    /* Check if list is not null and not empty */
    if (list && ! isEmpty(list))
    {
        /* Get the first node */
        remove = list -> head;

        /* Special case for a single item linked list */
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

/**
 * Name:     removeLast
 * Purpose:
 *     Remove a linked list node from the tail of the linked list
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  LinkedListNode pointer
 * Assertions:
 *     Assumptions:
 *         list is not NULL
 *     Results:
 *         Return the memory address of the linked list node and set the
 *         tail to the previous node
 **/

LinkedListNode* removeLast(LinkedList* list)
{
    LinkedListNode* remove;

    remove = NULL;

    /* Check if list is not null and not empty */
    if (list && ! isEmpty(list))
    {
        /* Get the last node */
        remove = list -> tail;

        /* Special case for a single item linked list */
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

/**
 * Name:     peekFirst
 * Purpose:
 *     Get the memory address of the first linked list node in the list
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  void pointer
 * Assertions:
 *     Assumptions:
 *         list is not NULL
 *     Results:
 *         Return the memory address of the linked list node value
 **/

void* peekFirst(LinkedList* list)
{
    void* nodeValue;

    nodeValue = NULL;

    /* Check if list is not null and not empty */
    if (list && ! isEmpty(list))
    {
        /* Get value */
        nodeValue = list -> head -> value;
    }

    return nodeValue;
}

/**
 * Name:     peekLast
 * Purpose:
 *     Get the memory address of the last linked list node in the list
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  void pointer
 * Assertions:
 *     Assumptions:
 *         list is not NULL
 *     Results:
 *         Return the memory address of the linked list node value
 **/

void* peekLast(LinkedList* list)
{
    void* nodeValue;

    nodeValue = NULL;

    /* Check if list is not null and not empty */
    if (list && ! isEmpty(list))
    {
        /* Get value */
        nodeValue = list -> tail -> value;
    }

    return nodeValue;
}

/**
 * Name:     getListLength
 * Purpose:
 *     Get the number of linked list node in the linked list
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  int
 * Assertions:
 *     Assumptions:
 *         list is not NULL
 *     Results:
 *         Return the length of the imported list
 **/

int getListLength(LinkedList* list)
{
    LinkedListNode* inList;
    int length;

    inList = NULL;
    length = 0;

    /* Check if list is not null and not empty */
    if (list && ! isEmpty(list))
    {
        /* Start recursion */
        inList = list -> head;
        length = getListLengthRecurse(inList);
    }

    return length;
}

/**
 * Name:     getListLengthRecurse
 * Purpose:
 *     A recursive function to get the number of nodes in the linked
 *     list. Used in conjunction to getListLength
 *
 * Parameters:
 *   - node : A pointer to a linked list node
 *
 * Returns:  int
 * Assertions:
 *     Assumptions:
 *         none
 *     Results:
 *         Return length after recusively calling itself
 **/

static int getListLengthRecurse(LinkedListNode* node)
{
    int length;

    length = 0;

    /* Check if node is not null */
    if (node)
    {
        /* Call recusion on the next node */
        length = 1 + getListLengthRecurse(node -> next);
    }

    return length;
}

/**
 * Name:     clearListStack
 * Purpose:
 *     Clear all nodes in the linked list
 *
 * Parameters:
 *   - list : A pointer to a pointer to a linked list
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         All values in the linked list are stored in the stack
 *     Results:
 *         Entire list is freed in memory
 **/

void clearListStack(LinkedList** list)
{
    /* Check if list is not null */
    if (*list)
    {
        /* If list is not empty */
        if (! isEmpty(*list))
        {
            clearListStackRecurse(&((*list) -> head));
        }

        /* Free the list allocated in initList */
        free(*list);
        *list = NULL;
    }
}

/**
 * Name:     clearListStackRecurse
 * Purpose:
 *     A recursive function to clear all the nodes in the linked list
 *
 * Parameters:
 *   - node : A pointer to a pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Recursively remove all nodes in a linked list starting at
 *         the tail
 **/

static void clearListStackRecurse(LinkedListNode** node)
{
    /* If node is not null */
    if (*node)
    {
        /* Call recusion on the next node */
        clearListStackRecurse(&((*node) -> next));

        /* Free the node allocated in initNode */
        free(*node);
        *node = NULL;
    }
}

/**
 * Name:     clearListMalloc
 * Purpose:
 *     Clear all nodes in the linked list
 *
 * Parameters:
 *   - list : A pointer to a pointer to a linked list
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         All values in the linked list are stored in the heap
 *     Results:
 *         Entire list, including stored values are freed in memory
 **/

void clearListMalloc(LinkedList** list)
{
    /* If list is not null */
    if (*list)
    {
        /* If list is not empty */
        if (! isEmpty(*list))
        {
            clearListMallocRecurse(&((*list) -> head));
        }

        /* Free the list allocated in initList */
        free(*list);
        *list = NULL;
    }
}

/**
 * Name:     clearListMallocRecurse
 * Purpose:
 *     A recursive function to clear all the nodes in the linked list
 *
 * Parameters:
 *   - node : A pointer to a pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Recursively remove nodes and values in a linked list
 *         from memory, stating at the tail
 **/

static void clearListMallocRecurse(LinkedListNode** node)
{
    /* If node is not null */
    if (*node)
    {
        /* Call recursion on the next node */
        clearListMallocRecurse(&((*node) -> next));

        /* If the value in node is not pointing to null */
        if ((*node) -> value)
        {
            /* Free the value in node */
            free((*node) -> value);
            (*node) -> value = NULL;
        }

        /* Free the node allocated in initNode */
        free(*node);
        *node = NULL;
    }
}

/**
 * Name:     setNextToNode
 * Purpose:
 *     Set a linked list node to the next pointer of another linked list node
 *
 * Parameters:
 *   - node     : A pointer to a linked list node
 *   - nextNode : A pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         Both nodes are not NULL
 *     Results:
 *         nextNode is set to the next pointer in node
 **/

static void setNextToNode(LinkedListNode* node, LinkedListNode* nextNode)
{
    /* If node and nextNode is not null */
    if (node && nextNode)
    {
        node -> next = nextNode;
    }
}

/**
 * Name:     setPrevToNode
 * Purpose:
 *     Set a linked list node to the prev pointer of another linked list node
 *
 * Parameters:
 *   - node     : A pointer to a linked list node
 *   - prevNode : A pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         Both nodes are not NULL
 *     Results:
 *         prevNode is set to the prev pointer in node
 **/

static void setPrevToNode(LinkedListNode* node, LinkedListNode* prevNode)
{
    /* If node and prevNode is not null */
    if (node && prevNode)
    {
        node -> prev = prevNode;
    }
}

/**
 * Name:     setNextInList
 * Purpose:
 *     Set the node to the next node
 *
 * Parameters:
 *   - node : A pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         node is not null
 *     Results:
 *         The next node in node is set to node
 **/

static void setNextInList(LinkedListNode* node)
{
    /* If node is not null */
    if (node)
    {
        node = node -> next;
    }
}

/**
 * Name:     setPrevInList
 * Purpose:
 *     Set the node to the previous node
 *
 * Parameters:
 *   - node : A pointer to a linked list node
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         node is not NULL
 *     Results:
 *         The previous node in node is set to node
 **/

static void setPrevInList(LinkedListNode* node)
{
    /* If node is not null */
    if (node)
    {
        node = node -> prev;
    }
}

/**
 * Name:     isEmpty
 * Purpose:
 *     Checks if the linked list contains no linked list nodes.
 *
 * Parameters:
 *   - list : A pointer to a linked list
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns TRRUE (1) or FALSE (0)
 **/

int isEmpty(LinkedList* list)
{
    int isEmpty;

    isEmpty = FALSE;

    /* If list, head and tail is not null */
    if (list && (! (list -> head)) && (! (list -> tail)))
    {
        isEmpty = TRUE;
    }

    return isEmpty;
}
