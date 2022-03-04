//The problem that i am having is that if the number of threads is greater then the queue then the threads are never joined i believed and it runs forever

#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "threadpool.h"

struct worker_node{
  void   (*function)(tpool_t, void*);
  void   *argument;
  struct worker_node *next;
}; typedef struct worker_node worker_node;

struct worker_list{
  struct worker_node *head;
  struct worker_node *tail;
  int count;
};typedef struct worker_list worker_list;

struct tpool {
  int num_th;
  int join; 
  uthread_t *t;
  uthread_mutex_t  mx;
  uthread_cond_t   cd;
  struct worker_list wl;
};

static void enqueue(worker_list *worker_list, void (*fun)(tpool_t, void *), void *arg){
    worker_node *worker_node = malloc(sizeof(struct worker_node));
    worker_node->function = fun;
    worker_node->argument = arg;
    worker_node->next = NULL;
    if(worker_list->count == 0){
      worker_list->head = worker_node;
      worker_list->tail = worker_node;
    }
    else{
      worker_list->tail->next = worker_node;
      worker_list->tail       = worker_node;
    }
    worker_list->count++;
} 

static worker_node *dequeue(worker_list *list){
  worker_node *task;
  task = list -> head;
  list->head = list->head->next;
  list->count--;
  return task;
}

/* Function executed by each pool worker thread. This function is
 * responsible for running individual tasks. The function continues
 * running as long as either the pool is not yet joined, or there are
 * unstarted tasks to run. If there are no tasks to run, and the pool
 * has not yet been joined, the worker thread must be blocked.
 * 
 * Parameter: param: The pool associated to the thread.
 * Returns: nothing.
 */

static void *worker_thread(void *param) {
  printf("worker\n");
  tpool_t pool = param;
  uthread_mutex_lock(pool->mx);

  while(!pool -> join) {
    if(pool -> wl.count == 0)
      printf("yes\n");
      uthread_cond_wait(pool->cd);
    while(pool -> wl.count > 0){
      printf("do task\n");
      struct worker_node * work = dequeue(&pool->wl);
      uthread_mutex_unlock(pool -> mx);
      work -> function(pool, work -> argument);
      uthread_mutex_lock(pool -> mx);
      uthread_cond_broadcast(pool->cd);
    }
  }
  uthread_mutex_unlock(pool -> mx);
  return NULL;
}

/* Creates (allocates) and initializes a new thread pool. Also creates
 * `num_threads` worker threads associated to the pool, so that
 * `num_threads` tasks can run in parallel at any given time.
 *
 * Parameter: num_threads: Number of worker threads to be created.
 * Returns: a pointer to the new thread pool object.
 */
tpool_t tpool_create(unsigned int num_threads) {
  tpool_t pool = malloc(sizeof(struct tpool));
  pool -> t = malloc(num_threads * sizeof(uthread_t));
  pool->wl.head = pool->wl.tail = NULL; 
  pool->wl.count = 0; 
  pool->join = 0;
  pool -> num_th = num_threads;
  pool -> mx = uthread_mutex_create();
  pool -> cd = uthread_cond_create(pool -> mx);
  for(int i = 0; i < num_threads; i++){
    pool -> t[i] = uthread_create(worker_thread, pool);
  }
  return pool;
}

/* Queues a new task, to be executed by one of the worker threads
 * associated to the pool. The task is represented by function `fun`,
 * which receives the pool and a generic pointer as parameters. If any
 * of the worker threads is available, `fun` is started immediately by
 * one of the worker threads. If all of the worker threads are busy,
 * `fun` is scheduled to be executed when a worker thread becomes
 * available. Tasks are retrieved by individual worker threads in the
 * order in which they are scheduled, though due to the nature of
 * concurrency they may not start exactly in the same order. This
 * function returns immediately, and does not wait for `fun` to
 * complete.
 *
 * Parameters: pool: the pool that is expected to run the task.
 *             fun: the function that should be executed.
 *             arg: the argument to be passed to fun.
 */
void tpool_schedule_task(tpool_t pool, void (*fun)(tpool_t, void *), void *arg) {
  uthread_mutex_lock(pool->mx);
  enqueue(&pool->wl,fun,arg);
  printf("task\n");
  uthread_cond_signal(pool->cd);
  uthread_mutex_unlock(pool->mx);
}

/* Blocks until the thread pool has no more scheduled tasks; then,
 * joins all worker threads, and frees the pool and all related
 * resources. Once this function returns, the pool cannot be used
 * anymore.
 *
 * Parameters: pool: the pool to be joined.
 */
void tpool_join(tpool_t pool) {
  pool -> join = 1; 
  printf("join\n");
  for(int i =0; i < pool -> num_th; i++){
    uthread_join(pool->t[i], NULL);
  }
  uthread_cond_destroy(pool->cd);
  uthread_mutex_destroy(pool->mx);
  free(pool->t);
  free(pool);
}