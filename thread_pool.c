#include <pthread.h>
#include <stdio.h>
#define QUEUE_SIZE 5
typedef struct {
  void (*function)(void *);
  void *arg;
} Jobs;
typedef struct {
  Jobs jobs[QUEUE_SIZE];
  int head;
  int tail;
  int count;
} Queue;
typedef struct {
  Queue *queue;
  Jobs job;
} Producer;

typedef struct {
  Queue *queue;
} Worker;

void print_me(void *arg) {
  int *a = arg;
  printf("The value is %d\n", *a);
}

void increment(void *arg) {
  int *a = arg;
  *a += 1;
  printf("The incremented value is %d\n", *a);
}

void queue_push(Queue *queue, Jobs job) {
  if (queue->count < QUEUE_SIZE) {
    queue->jobs[queue->tail] = job;
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    queue->count++;
    printf("JOB PUSHED SUCCESSFULLY!\n");
  } else {
    printf("MAX JOBS IN QUEUE");
  }
}

Jobs queue_pop(Queue *queue) {
  if (queue->count != 0) {
    Jobs job = queue->jobs[queue->head];
    queue->head = (queue->head + 1) % QUEUE_SIZE;
    queue->count--;
    return job;
  } else {
    printf("NO EXISTING JOB");
  }
}

void queue_init(Queue *queue) {
  queue->head = 0;
  queue->tail = 0;
  queue->count = 0;
}

void *Produce(void *arg) {
  Producer *args = arg;
  queue_push(args->queue, args->job);
  return NULL;
}

void *Work(void *arg) {
  Worker *args = arg;
  Jobs work = queue_pop(args->queue);
  work.function(work.arg);
  printf("Work Completed");
  return NULL;
}

int main() {
  Jobs job1, job2;
  Queue q1;
  queue_init(&q1);
  int x = 10;
  job1.function = print_me;
  job1.arg = &x;
  job2.function = increment;
  job2.arg = &x;
  Producer p;
  p.queue = &q1;
  p.job = job2;
  Worker w;
  w.queue = &q1;
  pthread_t producer;
  pthread_t worker1, worker2;
  pthread_create(&producer, NULL, &Produce, &p);
  pthread_create(&worker1, NULL, &Work, &w);
  pthread_join(producer, NULL);
  pthread_join(worker1, NULL);
  return 0;
}
