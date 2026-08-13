#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
pthread_rwlock_t lock;
int counter = 1;
void *my_write(void *arg) {
  for (int i = 1; i < 10; i++) {
    pthread_rwlock_wrlock(&lock);
    counter++;
    pthread_rwlock_unlock(&lock);
  }
  return NULL;
}
void *my_read(void *arg) {
  pthread_rwlock_rdlock(&lock);
  printf("Thread entered\n");
  printf("The value of counter is %d\n ", counter);
  sleep(1);
  pthread_rwlock_unlock(&lock);
  return NULL;
}
int main() {
  pthread_t writer;
  pthread_t reader1;
  pthread_t reader2;
  pthread_rwlock_init(&lock, NULL);
  pthread_create(&writer, NULL, my_write, NULL);
  pthread_create(&reader1, NULL, my_read, NULL);
  pthread_create(&reader2, NULL, my_read, NULL);
  pthread_join(writer, NULL);
  pthread_join(reader1, NULL);
  pthread_join(reader2, NULL);
  return 0;
}
