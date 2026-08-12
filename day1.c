#include <pthread.h>
#include <stdio.h>
int counter = 100;
pthread_mutex_t lock;
void *worker(void *arg) {
  for (int i = 1; i <= 10000; i++) {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
int main() {
  pthread_t thread1;
  pthread_t thread2;
  pthread_mutex_init(&lock, NULL);
  pthread_create(&thread1, NULL, worker, NULL);
  pthread_create(&thread2, NULL, worker, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_mutex_destroy(&lock);
  printf("The counter is now %d\n", counter);
  return 0;
}
