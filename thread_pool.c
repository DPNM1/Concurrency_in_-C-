#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define QUEUE_SIZE 20
#define JOB_COUNT 10
#define WORKER_COUNT 3

typedef struct {
  void (*function)(void *);
  void *arg;
} Jobs;

typedef struct {
  Jobs jobs[QUEUE_SIZE];

  int head;
  int tail;
  int count;
  int shutdown;
  pthread_mutex_t lock;
  pthread_cond_t condition;
} Queue;

typedef struct {
  Queue *queue;
  Jobs *jobs;
  int job_count;
} Producer;

typedef struct {
  Queue *queue;
} Worker;

// ---------------- JOB FUNCTION ----------------

void process(void *arg) {
  int *number = arg;

  printf("Processing job %d\n", *number);

  sleep(1);

  printf("Finished job %d\n", *number);
}

// ---------------- QUEUE ----------------

void queue_init(Queue *queue) {
  queue->head = 0;
  queue->tail = 0;
  queue->count = 0;
  queue->shutdown = 0;

  pthread_mutex_init(&queue->lock, NULL);
  pthread_cond_init(&queue->condition, NULL);
}

void queue_push(Queue *queue, Jobs job) {
  pthread_mutex_lock(&queue->lock);

  if (queue->count < QUEUE_SIZE) {

    queue->jobs[queue->tail] = job;

    queue->tail = (queue->tail + 1) % QUEUE_SIZE;

    queue->count++;

    printf("Job pushed\n");
  }
  pthread_mutex_unlock(&queue->lock);
  pthread_cond_broadcast(&queue->condition);
}

int queue_pop(Queue *queue, Jobs *job) {
  pthread_mutex_lock(&queue->lock);

  while (queue->count == 0 && !queue->shutdown) {
    pthread_cond_wait(&queue->condition, &queue->lock);
  }

  // No jobs remain AND shutdown was requested
  if (queue->count == 0 && queue->shutdown) {
    pthread_mutex_unlock(&queue->lock);
    return 0;
  }

  // We have a real job
  *job = queue->jobs[queue->head];

  queue->head = (queue->head + 1) % QUEUE_SIZE;

  queue->count--;

  pthread_mutex_unlock(&queue->lock);

  return 1;
}
// ---------------- PRODUCER ----------------

void *produce(void *arg) {
  Producer *producer = arg;

  for (int i = 0; i < producer->job_count; i++) {

    queue_push(producer->queue, producer->jobs[i]);
  }
  producer->queue->shutdown = 1;
  return NULL;
}

// ---------------- WORKER ----------------

void *work(void *arg) {
  Worker *worker = arg;

  while (1) {

    Jobs job;

    int result = queue_pop(worker->queue, &job);

    if (result == 0) {
      break;
    }

    job.function(job.arg);
  }

  return NULL;
}

// ---------------- MAIN ----------------

int main() {
  Queue queue;

  queue_init(&queue);

  // Our actual data
  int numbers[JOB_COUNT];

  // Our 10 Jobs
  Jobs jobs[JOB_COUNT];

  // Create the 10 jobs
  for (int i = 0; i < JOB_COUNT; i++) {

    numbers[i] = i + 1;

    jobs[i].function = process;

    jobs[i].arg = &numbers[i];
  }

  // Producer
  Producer producer;

  producer.queue = &queue;
  producer.jobs = jobs;
  producer.job_count = JOB_COUNT;

  // Workers
  Worker worker;

  worker.queue = &queue;

  pthread_t producer_thread;

  pthread_t worker_threads[WORKER_COUNT];

  // Create workers
  for (int i = 0; i < WORKER_COUNT; i++) {

    pthread_create(&worker_threads[i], NULL, work, &worker);
  }

  // Create producer
  pthread_create(&producer_thread, NULL, produce, &producer);

  pthread_join(producer_thread, NULL);
  for (int i = 0; i < WORKER_COUNT; i++) {
    pthread_join(worker_threads[i], NULL);
  }
  return 0;
}
