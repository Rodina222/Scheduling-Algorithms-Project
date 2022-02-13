#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
 
struct Node 
{
    int address;
    int used;
    struct Node* next;
};
 
struct Node* create_list()
{
    struct Node * Head;
    
    return Head;
}
int isEmpty(struct Node * Head ){
   return Head==NULL;
}
int length(struct Node * head) {
   int length = 0;
   struct Node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}
struct Node* delete( struct Node* head,int address) {

   //start from the first link
   struct Node* current = head;
   struct Node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->address != address) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}
struct Node* delete_end(struct Node* head)
{
   if(isEmpty(head))
   return NULL;

   struct Node *last=head->next;
   struct Node *prev=head;
   if(last==NULL)
   {
      head=NULL;
    return prev;
   }
while (last->next != NULL)
{
        last = last->next;
      prev=prev->next;
}
       prev->next=NULL;
        return last;
}

void append(struct Node* head, int address)
{
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
   new_node->used=0;
    struct Node *last = head;  /* used in step 5*/
 
    /* 2. put in the data  */
    new_node->address  = address;

    /* 3. This new node is going to be the last node, so make next 
          of it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (head== NULL)
    {
       head = new_node;
       return;
    }  
     
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
 
    /* 6. Change the next of last node */
    last->next = new_node;
    return;    
}

int main()
{
struct Node * L=create_list();
int x= isEmpty(L);
printf(" List intially is %d\n",x);
append(L,0);
x=isEmpty(L);
printf(" List secondly is %d\n",x);
append(L,1);
append(L,3);
append(L,5);
append(L,7);

struct Node * n;
   // while(!isEmpty(L))
   // {
   //    n=delete_end(L);
   //    printf("---");
   //    if(n!=NULL)
   //    printf("Node Info %d \n",n->address);
   // }
   
 

}