#ifndef PRIORITY_QUEUE_MEM
#define PRIORITY_QUEUE_MEM

#include <stdio.h>
#include <stdlib.h>

// Node
typedef struct Node_mem {
   
    int Address;
    
    struct Node_mem* next;

}Node_mem;

// Function to Create A New Node
Node_mem* new_pq_Node_mem(int p)
{
    Node_mem* temp = (Node_mem*)malloc(sizeof(Node_mem));
    temp->Address = p;
    
    temp->next = NULL;

    return temp;
}
struct Priority_Queue_mem {
    struct Node_mem *front, *rear;
};
struct Priority_Queue_mem* create_pq_queue_mem()
{

    struct Priority_Queue_mem* q = (struct Priority_Queue_mem*)malloc(sizeof(struct Priority_Queue_mem));
    q->front = q->rear = NULL;
    return q;
}



// Return the value at head
int peek_pq_mem(struct Priority_Queue_mem* head)
{   
    return head->front->Address;
}

// Removes the element with the
// highest priority form the list
void pop_mem(struct Priority_Queue_mem* head)
{
    Node_mem* temp = head->front;
    head->front = head->front->next;
    free(temp);
}

// Function to push according to priority
void push_mem(struct Priority_Queue_mem* head, int Add)
{
    Node_mem* start = head->front;

    // Create new Node
    Node_mem* temp = new_pq_Node_mem(Add);

    if(head->front==NULL)
        head->front=temp;
    else {
        // Special Case: The head of list has lesser
        // priority than new node. So insert new
        // node before head node and change head node.
        if (head->front->Address> Add) {

            // Insert New Node before head
            temp->next = head->front;
            head->front = temp;
        }
        else {

            // Traverse the list and find a
            // position to insert new node
            while (start->next != NULL &&
                    start->next->Address< Add) {
                start = start->next;
            }

            // Either at the ends of the list
            // or at required position
            temp->next = start->next;
            start->next = temp;
        }
    }
}

// Function to check is list is empty
int isEmpty_pq_mem(struct Priority_Queue_mem* head)
{
    return (head->front == NULL);
}

#endif