#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>
#include <math.h>

bool lockB, lockC, alive;
float a, b, c, result_1, result_2;

void receive_params( void *params) {

  printf("A: ");
  scanf("%f", &a);

  printf("B: ");
  scanf("%f", &b);

  printf("C: ");
  scanf("%f", &c);

  lockB = false;

}

void resolve_equation( void *params) {
  while(lockB);

  float delta = (pow(b, 2) - 4*a*c);

  result_1 = ((-b + sqrt(delta))/2*a);

  result_2 = ((-b - sqrt(delta))/2*a);

  lockC = false;

}

void print_result( void *params) {
  while(lockC);

  printf("Resultado 1: %.2f\n", result_1);
  printf("Resultado 2: %.2f\n", result_2);

  alive = false;

}

void start_thread() {

  int thread_id;

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(receive_params,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(receive_params,4096, NULL)) == -1) { printf("Erro na thread 1.\n"); }
  #endif

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(resolve_equation,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(resolve_equation,4096, NULL)) == -1) { printf("Erro na thread 2.\n"); }
  #endif

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(print_result,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(print_result,4096, NULL)) == -1) { printf("Erro na thread 3.\n"); }
  #endif
}

int main(int argc, char const *argv[]){

  lockB = lockC  = alive = true;

  start_thread();

  while(alive);

  return 0;
}
