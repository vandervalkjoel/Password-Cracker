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

// void my_task(tpool_t pool, void *arg) {
//   long i = (long) arg;
//   printf("Task %ld started\n", i);
//   usleep(1000000);
//   printf("Task %ld ended\n", i);
// }

void recursiveTask(void *arg){
  struct crypt_data data;
  char *ar = (char*) arg;
  crypt_r(ar, "$5$12345", &data);
  if((strcmp(data.output,answer) == 0) || strlen(ar) == 4){
    if(strcmp(data.output,answer) == 0){
      printf("%s\n",ar);
      exit(0);
    }
  }
  else{
    char *car0 = malloc(strlen(ar)+1);
    strncpy(car0, "0",1);
    recursiveTask((void *)strncat(car0,ar,strlen(ar)));
    char *car1 = malloc(strlen(ar)+1);
    strncpy(car1, "1",1);
    recursiveTask((void *)strncat(car1,ar,strlen(ar)));
    char *car2 = malloc(strlen(ar)+1);
    strncpy(car2, "2",1);
    recursiveTask((void *)strncat(car2,ar,strlen(ar)));
    char *car3 = malloc(strlen(ar)+1);
    strncpy(car3, "3",1);
    recursiveTask((void *)strncat(car3,ar,strlen(ar)));
    char *car4 = malloc(strlen(ar)+1);
    strncpy(car4, "4",1);
    recursiveTask((void *)strncat(car4,ar,strlen(ar)));
    char *car5 = malloc(strlen(ar)+1);
    strncpy(car5, "5",1);
    recursiveTask((void *)strncat(car5,ar,strlen(ar)));
    char *car6 = malloc(strlen(ar)+1);
    strncpy(car6, "6",1);
    recursiveTask((void *)strncat(car6,ar,strlen(ar)));
    char *car7 = malloc(strlen(ar)+1);
    strncpy(car7, "7",1);
    recursiveTask((void *)strncat(car7,ar,strlen(ar)));
    char *car8 = malloc(strlen(ar)+1);
    strncpy(car8, "8",1);
    recursiveTask((void *)strncat(car8,ar,strlen(ar)));
    char *car9 = malloc(strlen(ar)+1);
    strncpy(car9, "9",1);
    recursiveTask((void *)strncat(car9,ar,strlen(ar)));
  }
  return;

}

int main(int argc, char *argv[]) {
  strncpy(answer,argv[1],strlen(argv[1]) + 1);
  char ar0[] = "0"; 
  char ar1[] = "1";
  char ar2[] = "2";
  char ar3[] = "3";
  char ar4[] = "4";
  char ar5[] = "5";
  char ar6[] = "6";
  char ar7[] = "7";
  char ar8[] = "8";
  char ar9[] = "9";
  recursiveTask((void *)ar0);
  recursiveTask((void *)ar1);
  recursiveTask((void *)ar2);
  recursiveTask((void *)ar3);
  recursiveTask((void *)ar4);
  recursiveTask((void *)ar5);
  recursiveTask((void *)ar6);
  recursiveTask((void *)ar7);
  recursiveTask((void *)ar8);
  recursiveTask((void *)ar9);
  return 0; 
}