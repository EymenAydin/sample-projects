#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct graph{
   struct Vertex* head;
   int count;
}Graph;
typedef struct Vertex{
   struct Vertex* next;
   int data;
   int in_degree;
   int out_degree;
   bool intree;
   int processed;
   int path_length;
   struct Arc* first;
}vertex;
typedef struct Arc{
   struct Vertex* dest;
   int weight;
   bool intree;
   struct Arc* next;
}arc;
#include "queue.h"
#include "stacks.h"
Graph* create_graph(){
   Graph* new_graph=(Graph*)malloc(sizeof(Graph));
   new_graph->count=0;
   new_graph->head=NULL;
   return new_graph;
}
int Vertex_Insert(Graph* graph,int value){
    vertex* iterator=graph->head;
    while(iterator!=NULL){
        if(iterator->data==value){
            return -1;
        }
        iterator=iterator->next;
    }
    vertex* new_vertex=(vertex*)malloc(sizeof(vertex));
    new_vertex->data=value;
    new_vertex->in_degree=0;
    new_vertex->out_degree=0;
    new_vertex->processed=0;
    new_vertex->intree=false;
    new_vertex->path_length=1000;
    new_vertex->first=NULL;
    new_vertex->next=NULL;
    if(graph->head==NULL){
         graph->head=new_vertex;
    }else{
        vertex* iterator=graph->head;
        if(new_vertex->data<iterator->data){
            graph->head=new_vertex;
            new_vertex->next=iterator;
        }else{
            while(iterator->next!=NULL && iterator->next->data<new_vertex->data){
                iterator=iterator->next;
            }
            new_vertex->next=iterator->next;
            iterator->next=new_vertex;
        }
    }
    graph->count++;
    return 1;
}
int Vertex_Delete(Graph* graph,int key){
    vertex* iterator=graph->head;
    vertex* deleted=NULL;

    while(iterator!=NULL){
        if(iterator->data==key){
            deleted=iterator;
            break;
        }
        iterator=iterator->next;
    }

    if(deleted==NULL){
        return -1;
    }
    if(deleted->in_degree!=0 || deleted->out_degree!=0){
        return -2;
    }
    if(graph->count==1){
        graph->head=NULL;
    }
    else if(graph->head==deleted){
        graph->head=graph->head->next;
    }
    else{
        vertex* prev=graph->head;
        while(prev->next!=deleted){
           prev=prev->next;
        }
        prev->next=deleted->next;
    }
    graph->count--;
    free(deleted);
    return 0;
}
int Arc_Insert(Graph* graph,int fromkey,int tokey,int value){
    vertex* iterator=graph->head;
    vertex* from_vertex=NULL;
    vertex* to_vertex=NULL;
    int a=0;
    while(iterator!=NULL){
        if(iterator->data==fromkey){
            from_vertex=iterator;
            if(a==1){
                break;
            }
            a=1;
        }
        if(iterator->data==tokey){
            to_vertex=iterator;
            if(a==1){
                break;
            }
            a=1;
        }
        iterator=iterator->next;
    }
    if(from_vertex==NULL || to_vertex==NULL){
        return -1;
    }
    arc* new_arc=(arc*)malloc(sizeof(arc));
    from_vertex->out_degree++;
    to_vertex->in_degree++;
    new_arc->next=NULL;
    new_arc->dest=to_vertex;
    new_arc->weight=value;
    if(from_vertex->first==NULL){
        from_vertex->first=new_arc;
        new_arc->next=NULL;
        return 1;
    }
    arc* iterator2=from_vertex->first;
        if(new_arc->dest->data<iterator2->dest->data){
            from_vertex->first=new_arc;
            new_arc->next=iterator2;
            return 1;
        }else{
            while(iterator2->next!=NULL){
                if(new_arc->dest->data>iterator2->dest->data && new_arc->dest->data<iterator2->next->dest->data){
                    arc* temp=iterator2->next;
                    iterator2->next=new_arc;
                    new_arc->next=temp;
                    return 1;
                }
                iterator2=iterator2->next;
            }
            iterator2->next=new_arc;
            return 1;
        }
}
int Arc_Delete(Graph* graph,int fromkey,int tokey){
    vertex* iterator=graph->head;
    vertex* from_vertex=NULL;
    while(iterator!=NULL){
       if(iterator->data==fromkey){
           from_vertex=iterator;
           break;
       }
       iterator=iterator->next;
    }
    if(from_vertex==NULL){
        return -1;
    }
    arc* iterator2=from_vertex->first;
    arc* desired=NULL;
    while(iterator2!=NULL){
        if(iterator2->dest->data==tokey){
           desired=iterator2;
           break;
        }
        iterator2=iterator2->next;
    }
    if(desired==NULL){
        return -2;
    }
    from_vertex->out_degree--;
    desired->dest->in_degree--;
    iterator2=from_vertex->first;
    if(iterator2==desired){
        from_vertex->first=iterator2->next;
    }else{
        while(iterator2->next!=NULL && iterator2->next!=desired){
            iterator2=iterator2->next;
        }
        if(iterator2->next==desired){
            iterator2->next=desired->next;
        }
    }
    free(desired);
    return 0;
}
vertex** depth_first_traversal(Graph* graph){
    if(graph->count==0){
        return NULL;
    }
    vertex** traversal=(vertex**)calloc((graph->count+1),sizeof(vertex*));
    int index=0;
    vertex* temp=NULL;
    arc* temp1=NULL;
    vertex* walker=graph->head;
    while(walker!=NULL){
        walker->processed=0;
        walker=walker->next;
    }
    Stack* traverse=create_stack();
    walker=graph->head;
    while(walker!=NULL){
        if(walker->processed==0){
            push(traverse,walker);
            walker->processed=1;
        }
            while(traverse->count!=0){
                temp=pop(traverse);
                traversal[index]=temp;
                index++;
                temp->processed=2;
                temp1=temp->first;
                while(temp1!=NULL){
                    if(temp1->dest->processed==0){
                        push(traverse,temp1->dest);
                        temp1->dest->processed=1;
                    }
                    temp1=temp1->next;
                }
            }
        walker=walker->next;
    }
    traversal[index]=NULL;
    destroystack(traverse);
    return traversal;
}
vertex** breadth_first_traversal(Graph* graph){
    if(graph->count==0){
        return NULL;
    }
    vertex* iterator=graph->head;
    while(iterator!=NULL){
        iterator->processed=0;
        iterator=iterator->next;
    }
    Queue* traverse=create_queue();
    vertex** traversal=(vertex**)calloc((graph->count+1),sizeof(vertex*));
    int index=0;
    vertex* temp=NULL;
    arc* temp1=NULL;
    iterator=graph->head;
    while(iterator!=NULL){
        if(iterator->processed==0){
            enqueue(traverse,iterator);
            iterator->processed=1;
        }
        while(traverse->size!=0){
            temp=dequeue(traverse);
            traversal[index]=temp;
            index++;
            temp->processed=2;
            temp1=temp->first;
            while(temp1!=NULL){
                if(temp1->dest->processed==0){
                    enqueue(traverse,temp1->dest);
                    temp1->dest->processed=1;
                }
                temp1=temp1->next;
            }
        }
        iterator=iterator->next;
    }
    traversal[index]=NULL;
    destroy_queue(traverse);
    return traversal;
}
void minimum_spanning_tree(Graph* graph){
    if(graph->count==0){
        return;
    }
    vertex* iterator=graph->head;
    arc* iterator1;
    while(iterator!=NULL){
        iterator->intree=false;
        iterator1 = iterator->first;
        while(iterator1!=NULL){
            iterator1->intree=false;
            iterator1=iterator1->next;
        }
        iterator=iterator->next;
    }
    iterator=graph->head;
    iterator->intree=true;
    bool tree_complete=false;
    while(!tree_complete){
        tree_complete=true;
        int min_edge=1000;
        arc* min_edge_loc=NULL;
        iterator = graph->head; /* reset iterator each pass */
        while(iterator!=NULL){
            if(iterator->intree && iterator->out_degree>0){
                iterator1=iterator->first;
                while(iterator1!=NULL){
                    if(!iterator1->dest->intree){
                        tree_complete=false;
                        if(iterator1->weight<min_edge){
                            min_edge=iterator1->weight;
                            min_edge_loc=iterator1;
                        }
                    }
                    iterator1=iterator1->next;
                }
            }
            iterator=iterator->next;
        }
        if(min_edge_loc!=NULL){
            min_edge_loc->intree=true;
            min_edge_loc->dest->intree=true;
        }
    }
}
void shortest_path(Graph* graph){
    if(graph->count==0){
        return;
    }
    vertex* iterator=graph->head;
    arc* iterator2;
    while(iterator!=NULL){
        iterator->path_length=1000;
        iterator->intree=false;
        iterator2 = iterator->first;
        while(iterator2!=NULL){
            iterator2->intree=false;
            iterator2=iterator2->next;
        }
        iterator=iterator->next;
    }
    iterator=graph->head;
    iterator->intree=true;
    iterator->path_length=0;
    bool tree_complete=false;
    while(!tree_complete){
        tree_complete=true;
        arc* path_loc=NULL;
        int new_path_len=1000;
        iterator = graph->head; /* reset iterator for this pass */
        while(iterator!=NULL){
            if(iterator->intree && iterator->out_degree>0){
                iterator2=iterator->first;
                int min_path=iterator->path_length;
                int min_edge=1000;
                arc* min_local_arc = NULL;
                while(iterator2!=NULL){
                    if(!iterator2->dest->intree){
                        tree_complete=false;
                        if(iterator2->weight<min_edge){
                            min_edge=iterator2->weight;
                            min_local_arc=iterator2;
                        }
                    }
                    iterator2=iterator2->next;
                }
                if(min_local_arc!=NULL && min_path+min_edge<new_path_len){
                    new_path_len=min_path+min_edge;
                    path_loc=min_local_arc;
                }
            }
            iterator=iterator->next;
        }
        if(path_loc!=NULL){
            path_loc->intree=true;
            path_loc->dest->intree=true;
            path_loc->dest->path_length=new_path_len;
        }
    }
}
void destroy_graph(Graph* graph){
    vertex* iterator=graph->head;
    vertex* temp=NULL;
    arc* iterator2=NULL;
    arc* temp2=NULL;

    while(iterator!=NULL){
         iterator2=iterator->first;
         while(iterator2!=NULL){
            temp2=iterator2->next;
            free(iterator2);
            iterator2=temp2;
         }
         temp=iterator->next;
         free(iterator);
         iterator=temp;
    }
    free(graph);
}

void Insert_Vertex(Graph* graph,int value){
    if(Vertex_Insert(graph,value)==1){
        printf("Data Inserted\n");
    }else{
        printf("Cant Insert Same Element\n");
    }
}
void Delete_Vertex(Graph* graph,int key){
    int a=Vertex_Delete(graph,key);
    if(a==-2){
        printf("Cant find the element\n");
    }else if(a==-1){
        printf("Node has arc leading into or out of it\n");
    }else{
        printf("Data Deleted\n");
    }
}
void Delete_Arc(Graph* graph,int fromkey,int tokey){
    int a=Arc_Delete(graph,fromkey,tokey);
    if(a==-2){
        printf("Cannot find the source node\n");
    }else if(a==-1){
        printf("Cannot find the arc\n");
    }else{
        printf("Arc deleted\n");
    }
}
void Insert_Arc(Graph* graph,int fromkey,int tokey,int value){
    if(Arc_Insert(graph,fromkey,tokey,value)==-1){
        printf("Could not find at least one node of the arc\n");
    }else{
        printf("Arc Inserted\n");
    }
}
vertex* retrieve_vertex(Graph* graph,int key){
    vertex* temp=graph->head;
    while(temp!=NULL){
        if(temp->data==key){
            return temp;
        }
        temp=temp->next;
    }
    return temp;
}
vertex* find_vertex(Graph* graph,int position){
    vertex* iterator=graph->head;
    for(int i=0;i<position;i++){
        iterator=iterator->next;
    }
    return iterator;
}
int main(){
    Graph* graph=create_graph();
    Insert_Vertex(graph,12);
    Insert_Vertex(graph,23);
    Insert_Vertex(graph,52);
    Insert_Vertex(graph,34);
    Insert_Vertex(graph,25);
    Insert_Arc(graph,12,52,2);
    Insert_Arc(graph,12,34,6);
    Insert_Arc(graph,34,52,5);
    Insert_Arc(graph,34,25,4);
    Insert_Arc(graph,23,12,3);
    Insert_Arc(graph,23,52,8);
    Insert_Arc(graph,25,23,1);
    Insert_Arc(graph,25,34,9);
    Insert_Arc(graph,52,12,7);
    Insert_Arc(graph,52,25,10);
    vertex** depth=depth_first_traversal(graph);
    vertex** breadth=breadth_first_traversal(graph);
    minimum_spanning_tree(graph);

    printf("Minimum spanning tree edges (u -> v : weight):\n");
    vertex* v = graph->head;
    while(v!=NULL){
        arc* a = v->first;
        while(a!=NULL){
            if(a->intree){
                printf("%d -> %d : %d\n", v->data, a->dest->data, a->weight);
            }
            a = a->next;
        }
        v = v->next;
    }

    shortest_path(graph);
    printf("Shortest path lengths from %d:\n", graph->head->data);
    v = graph->head;
    while(v!=NULL){
        if(v->path_length>=1000){
            printf("%d : INF\n", v->data);
        }else{
            printf("%d : %d\n", v->data, v->path_length);
        }
        v = v->next;
    }

    int index=0;
    while(depth[index]!=NULL){
        printf("%d |",depth[index]->data);
        index++;
    }
    printf("\n");
    index=0;
    while(breadth[index]!=NULL){
        printf("%d |",breadth[index]->data);
        index++;
    }
    printf("\n");
    free(breadth);
    free(depth);
    destroy_graph(graph);
    return EXIT_SUCCESS;
}
