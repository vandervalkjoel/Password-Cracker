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

int main(int argc, char *argv[]) {
    char *ar = malloc(1);
for(int i = 0; i < 10; i++){
    snprintf(ar, 2, "%d", i);
    printf("%s\n",ar);
  }
  free(ar);
}