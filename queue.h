#ifndef Def
#define Def 1
#include <stdlib.h>
typedef struct element{
   struct Vertex* vertice;
   struct element* next;
}element;
typedef struct queue{
   element* rear;
   element* front;
   int size;
}Queue;
bool enqueue(Queue* q,struct Vertex* value){
    element* new_node=malloc(sizeof(element));
    new_node->vertice=value;
    new_node->next=NULL;
    if(q->size==0){
        q->front=new_node;
        q->rear=new_node;
    }else{
        q->rear->next=new_node;
        q->rear=new_node;
    }
    q->size++;
    return true;
}
Queue* create_queue(){
    Queue* q=malloc(sizeof(Queue));
    q->front=NULL;
    q->rear=NULL;
    q->size=0;
    return q;
}
struct Vertex* dequeue(Queue* q){
    if(q->size==0) return NULL;
    element* temp=q->front;
    if(q->size==1){
        q->front=NULL;
        q->rear=NULL;
    }else{
        q->front=temp->next;
    }
    struct Vertex* value=temp->vertice;
    free(temp);
    q->size=q->size-1;
    return value;
}
struct Vertex* retrievefront(Queue* q){
    if(q->front==NULL) return NULL;
    return q->front->vertice;
}
struct Vertex* retrieverear(Queue* q){
    if(q->rear==NULL) return NULL;
    return q->rear->vertice;
}
void destroy_queue(Queue* q){
    element* temp;
   while(q->front!=NULL){
       temp=q->front->next;
       free(q->front);
       q->front=temp;
   }
   free(q);
}
#endif