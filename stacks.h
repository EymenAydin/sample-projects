#ifndef DEF
#define DEF 1
typedef struct stack{
    struct node* top;
    int count;
}Stack;
typedef struct node{
    struct Vertex* vertice;
    struct node* down;
}node;
Stack* create_stack(){
   Stack* s=malloc(sizeof(Stack));
   s->top=NULL;
   s->count=0;
   return s;
}
void push(Stack* s,vertex* new_vertice){
   node* n=malloc(sizeof(node));
   n->vertice=new_vertice;
   if(s->count==0){
       s->top=n;
       s->top->down=NULL;
   }else{
       n->down=s->top;
       s->top=n;
   }
   s->count+=1;
}
vertex* top(Stack* s){
    if(s->count==0){
        return NULL;
    }
    return s->top->vertice;
}
vertex* pop(Stack* s){
    vertex* a=NULL;
    if(s->count==0){
        return a;
    }else if(s->count==1){
        a=s->top->vertice;
        free(s->top); 
    }else{
        node* temp=s->top;
        a=temp->vertice;
        s->top=s->top->down;
        free(temp);
    }
    s->count--;
    return a;
}
bool compare(Stack* s, vertex* new_vertice){
    node* temp=s->top;
    while(temp!=NULL){
       if(temp->vertice==new_vertice){
        return true;
       }
       temp=temp->down;
    }
    return false;
}
void destroystack(Stack* s){
    node* temp=NULL;
    for(int i=0;i<s->count;i++){
        temp=s->top;
        free(s->top);
        s->top=temp->down;
    }
    printf("The stack is destroyed\n");
}
#endif