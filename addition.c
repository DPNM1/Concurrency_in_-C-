#include <pthread.h>
#include <stdio.h>
void *add(void *arg) {
  int *a = (int *)arg;
  *a += 1;
  printf("The value is now %d ", *a);
  return NULL;
}
int main() {
  pthread_t worker;
  int a = 10;
  pthread_create(&worker, NULL, &add, &a);
  pthread_join(worker, NULL);
  return 0;
}
