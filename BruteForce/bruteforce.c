#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "uthread.h"
#include "threadpool.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>

char answer[256];
char *valid;

void call(tpool_t pool, void *arg){
  struct crypt_data data;
  char *ar = arg;
  crypt_r(ar, "$5$12345", &data);
  if((strcmp(data.output,answer) == 0) || strlen(ar) == 8){
    if(strcmp(data.output,answer) == 0){
      printf("%s\n",ar);
      exit(0);
    }
  }
  else{
    for(int i =0; i < 10; i++){
      char *newstring = malloc(strlen(ar) +2);
      strncpy(newstring,ar,strlen(ar));
      newstring[strlen(ar) + 1] = NULL;
      newstring[strlen(ar)] = valid[i];
      tpool_schedule_task(pool, call, (void *) newstring);
    }
  }
  free(ar);
  return;

}

int main(int argc, char *argv[]) {
  tpool_t pool;
  int num_threads;
  if (argc != 3) {
    fprintf(stderr, "Usage: %s NUM_THREADS NUM_TASKS\n", argv[0]);
    return -1;
  }
  
  num_threads = strtol(argv[1], NULL, 10);
  uthread_init(num_threads <= 1 ? 2 : num_threads);
  pool = tpool_create(num_threads);
  strncpy(answer,argv[2],strlen(argv[2]));
  valid = "0123456789";
  tpool_schedule_task(pool,call,strdup(""));
  tpool_join(pool);
  printf("No match was found\n");
  return 0;
}