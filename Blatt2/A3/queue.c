#include<stdio.h>
#include<malloc.h>
#include "queue.h"

struct queue* queue_new(int capacity){
	if(capacity>0){
struct queue* nQ; 
nQ= malloc(sizeof(struct queue_node)*capacity);
nQ->head = NULL;
nQ->capacity = capacity;
nQ->size = 0;
return nQ;
}
return NULL;
}

void queue_delete(struct queue* queue){
	int i;
	int*r = malloc(sizeof(int));
	while(queue->head->next!=NULL){
	i=queue_pop_front(queue,r);
	}
	free(queue->head);
	free(r);
	free(queue);


}

int queue_push_back (struct queue* queue,int value){
	if(queue->size<queue->capacity){
	if(queue->head == NULL){
		struct queue_node* nHead = malloc(sizeof(struct queue_node));
		nHead->value = value;
		nHead->next = NULL;
		queue->head = nHead;
		queue->size++;
		return queue->size;
	}

	struct queue_node* current;
	current=queue->head;
	while(current->next !=NULL){
		struct queue_node* temp = current->next;
		current=temp;
	}
	struct queue_node* nNode = malloc(sizeof(struct queue_node));
	nNode->value=value;
	nNode->next=NULL;
	current->next =nNode;
	queue->size++;
	return queue->size;
	}
	return -1;
}

int queue_pop_front(struct queue* queue, int* ret_value){
	if(queue->size>0){
		*ret_value=queue->head->value;
		if(queue->head->next!=NULL){
		struct queue_node* temp = queue->head->next;
		free(queue->head);
		queue->head = temp;
		}
		else{
		if(queue->size<1){
		free(queue->head);
		return --queue->size;
		}
		}
		return --queue->size;
	}
	return -1;
}

// Sample -Test Code 
/*int main() {

struct queue* wrongQueue = queue_new(-1);
if(wrongQueue==NULL){
	printf("Negative Queue detected\n");
}
struct queue* newQueue = queue_new(20);
if(newQueue!=NULL){
//printf("%d , %d  %d\n",(int)newQueue, (int)newQueue->size,(int)newQueue->head);
queue_push_back(newQueue,4);
queue_push_back(newQueue,6);
queue_push_back(newQueue,5);
queue_push_back(newQueue,1);
//printf("%d %d %d\n", (int)newQueue->size,(int)newQueue->head,(int)newQueue->head->next);
int* ret_value=malloc(sizeof(int));
int tt=queue_pop_front(newQueue,ret_value);
	printf("%d , %d \n",tt, *ret_value);
while(tt>0){
	tt=queue_pop_front(newQueue,ret_value);
		printf("%d , %d \n",tt, *ret_value);

}
free(ret_value);
}
queue_delete(newQueue);

}
*/