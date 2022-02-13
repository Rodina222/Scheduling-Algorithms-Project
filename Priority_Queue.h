#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <stdio.h>
#include <stdlib.h>

// Node
typedef struct node {
    struct Process data;
    int priority;

    struct node* next;

} Node;

// Function to Create A New Node
Node* new_pq_Node(struct Process d,int p)
{
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = d;
    temp->priority  = p;
    temp->next = NULL;

    return temp;
}
struct Priority_Queue {
    struct node *front, *rear;
};
struct Priority_Queue* create_pq_queue()
{

    struct Priority_Queue* q = (struct Priority_Queue*)malloc(sizeof(struct Priority_Queue));
    q->front = q->rear = NULL;
    return q;
}



// Return the value at head
struct Process peek_pq(struct Priority_Queue* head)
{   
    return head->front->data;
}

// Removes the element with the
// highest priority form the list
void pop(struct Priority_Queue* head)
{
    Node* temp = head->front;
    head->front = head->front->next;
    free(temp);
}

// Function to push according to priority
void push(struct Priority_Queue* head, struct Process d, int p)
{
    Node* start = head->front;

    // Create new Node
    Node* temp = new_pq_Node(d,p);

    if(head->front==NULL)
        head->front=temp;
    else {
        // Special Case: The head of list has lesser
        // priority than new node. So insert new
        // node before head node and change head node.
        if (head->front->priority > p) {

            // Insert New Node before head
            temp->next = head->front;
            head->front = temp;
        }
        else {

            // Traverse the list and find a
            // position to insert new node
            while (start->next != NULL &&
                    start->next->priority < p) {
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
int isEmpty_pq(struct Priority_Queue* head)
{
    return (head->front == NULL);
}

#endif