#include <pthread.h>
#include <stdio.h>
typedef struct {
  void (*function)(void *);
  void *arg;
} Job;
void print_number(void *arg) {
  int *a = arg;
  printf("The number is %d ", *a);
}
int main() {
  Job job;
  int x = 10;
  job.function = &print_number;
  job.arg = &x;
  job.function(job.arg);
  return 0;
}
