#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

bool lockB, lockC;
float a, b, c, result_1, result_2;

void *receive_params( void *params) {
  printf("A: ");
  scanf("%f", &a);

  printf("B: ");
  scanf("%f", &b);
  
  printf("C: ");
  scanf("%f", &c);

  lockB = false;

  pthread_exit(0);
}

void *resolve_equation( void *params) {
  while(lockB);

  float delta = (pow(b, 2) - 4*a*c);

  result_1 = ((-b + sqrt(delta))/2*a);

  result_2 = ((-b - sqrt(delta))/2*a);

  lockC = false;

  pthread_exit(0);
}

void *print_result( void *params) {
  while(lockC);

  printf("Resultado 1: %.2f\n", result_1);
  printf("Resultado 2: %.2f\n", result_2);

  pthread_exit(0);
}

int main(int argc, char const *argv[]){
  
  pthread_t threadA, threadB, threadC;

  lockB = lockC = true;

  pthread_create( &threadA, NULL, receive_params, (void*) NULL );
  pthread_create( &threadB, NULL, resolve_equation, (void*) NULL );
  pthread_create( &threadC, NULL, print_result, (void*) NULL );

  pthread_join( threadA, NULL);
  pthread_join( threadB, NULL);
  pthread_join( threadC, NULL);

  return 0;
}