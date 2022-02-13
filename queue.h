#ifndef QUEUE
#define QUEUE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"


// A linked list (LL) node to store a queue entry
struct QNode {
    struct Process key;
    struct QNode* next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    struct QNode *front, *rear;
};

// A utility function to create a new linked list node.
struct QNode* newNode(struct Process k)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

int isEmpty(struct Queue * q) {
    return(q->front==NULL);
}
// A utility function to create an empty queue
struct Queue* create_queue()
{

    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// The function to add a key k to q
void enqueue(struct Queue* q, struct Process k)
{
    // Create a new LL node
    struct QNode* temp = newNode(k);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;


}

// Function to remove a key from given queue q
struct Process dequeue(struct Queue* q)
{
    struct Process t;
    t.id=0;

    t.arrival=-1;
    t.runtime=0;
    t.priority=0;
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return t;
    else
    {

        // Store previous front and move front one node ahead
        struct QNode* temp = q->front;
        struct Process n = q->front->key;
        q->front = q->front->next;

        // If front becomes NULL, then change rear also as NULL
        if (q->front == NULL)
            q->rear = NULL;


        free(temp);
        return (n);
    }
}

struct Process peek(struct Queue* q) {
    struct Process p;
    if(q->front ==NULL) {
        p.id=0;
        p.arrival=-1;
        p.runtime=0;
        p.priority=0;
        return p;
    }

    else
    {
        return q->front->key;

    }
}

#endif