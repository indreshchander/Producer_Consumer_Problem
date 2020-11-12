#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>

#define PRODUCER_THREADS 10
#define CONSUMER_THREADS 10

void *producer(void *vararg)
{
  int val;
  int seed = *(int *)vararg;
  printf("seed:%d\n", seed);

  for(int i=1; i<10; i++){
    sleep(1);
    val = i*seed;
    printf("Seed:%d, value:%d\n", seed, val);
  }

  return NULL;
}

int main()
{
  pthread_t consumer_thread_id[PRODUCER_THREADS+1];
  int arg[PRODUCER_THREADS+1];

  for(int i=1; i<PRODUCER_THREADS+1; i++){
    arg[i] = i;
    printf("creating producer thread:%d\n", i);
    pthread_create(&consumer_thread_id[i], NULL, producer, &arg[i]);
  }

  for(int i=1; i<PRODUCER_THREADS+1; i++){
    printf("joining producer thread:%d\n", i);
    pthread_join(consumer_thread_id[i], NULL);
  }

  return 1;
}
