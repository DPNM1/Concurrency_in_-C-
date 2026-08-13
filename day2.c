#include <pthread.h>
#include <stdio.h>
pthread_rwlock_t lock;
int counter = 1;
void *write(void *arg) {
  for (int i = 0; i < 10; i++) {
    pthread_rwlock_wrlock(&lock);
    counter++;
    pthread_rwlock_unlock(&lock);
  }
  return NULL;
}
void *read(void *arg) {
  pthread_rwlock_rdlock(&lock);
  printf("The value of counter is %d ", counter);
  pthread_rwlock_unlock(&lock);
  return NULL;
}
int main() {
  pthread_t writer;
  pthread_t reader1;
  pthread_t reader2;
  pthread_rwlock_init(&lock, NULL);
  pthread_create(&writer, NULL, write, NULL);
  pthread_create(&reader1, NULL, read, NULL);
  pthread_create(&reader2, NULL, read, NULL);
  pthread_join(writer, NULL);
  pthread_join(reader1, NULL);
  pthread_join(reader2, NULL);
  return 0;
}
