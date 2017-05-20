//CONSPROD.C by Julian Ullrich. Memo to Myself:Compile with gcc -c queue.c -o queue.o and gcc consprod.c queue.o -o consprod -lpthread
#define ARG_ERROR_TEXT "\nHUCH!! Das war wohl nichts..........\nAm besten gleich noch einmal probieren!\nUsage: ./consprod ANZAHL_PRODUCER ANZAHL_CONSUMER MENGE_DER_ELEMENTE\nBeispiel:./consprod 4 2 10000\n\n"
#define ARG_ERROR -1
#define T_CREATE_ERROR_TEXT "Error creating Threads!\n"
#define T_CREATE_ERROR 1
#define T_JOIN_ERROR_TEXT "Error joining Threads!\n"
#define T_JOIN_ERROR 2

#include "queue.h"
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>

struct queue* consprodqueue;
int elementsToPush;
int elementsToPop;

void *consumerthreadfunction(void* arg){
	pthread_mutex_t* mutex= (pthread_mutex_t*) arg;
	int size;
	int consumer=pthread_self();
	int* ret=malloc(sizeof(int));
	while(1){
		pthread_mutex_lock(mutex);
		if(elementsToPop>0){
			size=queue_pop_front(consprodqueue,ret);
			if(size!=-1){
				elementsToPop--;
			}
		} else {
			pthread_mutex_unlock(mutex);
			free(ret);
			return NULL;
		}
		pthread_mutex_unlock(mutex);

	}


}

void *producerthreadfunction(void* arg){
	pthread_mutex_t* mutex= (pthread_mutex_t*) arg;
	int randomvalue;
	int size;
	int producer=pthread_self();
	while(1){
		randomvalue=rand();
		pthread_mutex_lock(mutex);
		if(elementsToPush>0){
			size=queue_push_back(consprodqueue,randomvalue);
			if(size!=-1){
			elementsToPush--;
		}
		} else {
			pthread_mutex_unlock(mutex);
			return NULL;
		}
		pthread_mutex_unlock(mutex);

	}

}
// Funktions str2num and len taken/adapted from mandel.c / Abgabe 1!

int len(char * str) {
    int grddorf=0;
    while (str[grddorf] != '\0' && str[grddorf] != ' ' && grddorf != 4022500) ++grddorf;
    return grddorf;
}

int str2num(char * str) {
    int result = 0;
    int b = 1;
    int l = len(str);
    int i;
    for(i=1; i<l; ++i) b *= 10;
    for(i=0; i<l; ++i) {
        result += b * (int)(str[i] - '0');
        b /= 10;
    }
    return result;
}

int strIsNum(char * str){
    int l= len(str);
    int i;
    for(i=0; i<l; i++){
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}


int main(int argc, char ** argv, char ** envp) {

	if(argc!=4){
		printf(ARG_ERROR_TEXT);
		return ARG_ERROR;
	}
	if(!strIsNum(argv[1]) || !strIsNum(argv[2]) || !strIsNum(argv[3])){
		printf(ARG_ERROR_TEXT);
		return ARG_ERROR;
	}

	int prodAmount=str2num(argv[1]);
	int consAmount= str2num(argv[2]);
	elementsToPop=str2num(argv[3]);
	elementsToPush=elementsToPop;

consprodqueue = queue_new(100);
pthread_t prodT[prodAmount];
pthread_t consT[consAmount];

pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
int i;
for(i=0;i<prodAmount;i++){
	if(pthread_create(&prodT[i],NULL,producerthreadfunction,&mutex)){
		fprintf(stderr, T_CREATE_ERROR_TEXT);
		pthread_mutex_destroy(&mutex);
		queue_delete(consprodqueue);
		return T_CREATE_ERROR;
	} else {
		printf("Create Prod: %d\n", i);
	}
}
for(i=0;i<consAmount;i++){
	if(pthread_create(&consT[i],NULL,consumerthreadfunction,&mutex)){
		fprintf(stderr, T_CREATE_ERROR_TEXT);
		pthread_mutex_destroy(&mutex);
		queue_delete(consprodqueue);
		return T_CREATE_ERROR;
	} else {
		printf("Create Cons: %d\n", i);
	}
}

for(i=0;i<prodAmount;i++){
	if(pthread_join(prodT[i],NULL)){
		fprintf(stderr, T_JOIN_ERROR_TEXT);
		pthread_mutex_destroy(&mutex);
		queue_delete(consprodqueue);

		return T_JOIN_ERROR;
	}else {
		printf("Join Prod: %d\n", i);
	}
}
for(i=0;i<consAmount;i++){
	if(pthread_join(consT[i],NULL)){
		fprintf(stderr, T_JOIN_ERROR_TEXT);
		pthread_mutex_destroy(&mutex);
		queue_delete(consprodqueue);
		return T_JOIN_ERROR;
	} else {
		printf("Join Cons: %d\n", i);
	}
}


pthread_mutex_destroy(&mutex);

queue_delete(consprodqueue);
}