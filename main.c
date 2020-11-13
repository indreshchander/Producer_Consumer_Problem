#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h> 

#define PRODUCER_THREADS 10
#define CONSUMER_THREADS 10
#define BUFFER_SIZE      10

int buffer[BUFFER_SIZE];
int top = 0; //top free index
sem_t full, empty;
pthread_mutex_t lock;


void *producer(void *vararg)
{
  int val;
   int i;
  int seed = *(int *)vararg;
  printf("seed:%d\n", seed);

  for(i=1; i<10; i++){
    int randSleep = rand();
    randSleep = 20 + randSleep % 10;
    printf("PRODUCER: seed:%d, sleeping for %d\n", seed, randSleep);
    sleep(randSleep);
    val = i*seed;
    sem_wait(&empty);
    pthread_mutex_lock(&lock);

    printf("PRODUCER: PRODUCING ITEM, Seed:%d, value:%d, top:%d\n", seed, val, top);
    buffer[top++] = val; 
  
    pthread_mutex_unlock(&lock);
    sem_post(&full);
  }

  return NULL;
}

void *consumer(void *vararg)
{
  while(1){
    sleep(1);
    sem_wait(&full);
    pthread_mutex_lock(&lock);

    top--;
    printf("CONSUMER: CONSUMING ITEM, %d, top:%d\n", buffer[top], top);
    buffer[top] = -1;

    pthread_mutex_unlock(&lock);
    sem_post(&empty);    
  }    
}

void initializeBuffer()
{
  int i;
  for(i=0; i<BUFFER_SIZE; i++){
    buffer[i] = -1;
  }
}

int main()
{
  pthread_t producer_thread_id[PRODUCER_THREADS+1], consumer_thread_id[PRODUCER_THREADS+1];
  int arg_prod[PRODUCER_THREADS+1], arg_cons[PRODUCER_THREADS+1];

  initializeBuffer();

  sem_init(&empty,0,BUFFER_SIZE);
  sem_init(&full,0,0);
  pthread_mutex_init(&lock, NULL);
  int i;
  for(i=1; i<PRODUCER_THREADS+1; i++){
    arg_prod[i] = i;
    printf("creating producer thread:%d\n", i);
    pthread_create(&producer_thread_id[i], NULL, producer, &arg_prod[i]);
  }

  for(i=1; i<CONSUMER_THREADS+1; i++){
    arg_cons[i] = i;
    printf("creating consumer thread:%d\n", i);
    pthread_create(&consumer_thread_id[i], NULL, consumer, &arg_cons[i]);
  }


  for(i=1; i<PRODUCER_THREADS+1; i++){
    printf("joining consumer and producer thread:%d\n", i);
    pthread_join(producer_thread_id[i], NULL);
    pthread_join(consumer_thread_id[i], NULL);
  }

  pthread_mutex_destroy(&lock);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 1;
}
