#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>

char array_char[30];
int index_array;
bool lockA;
bool lockB;
bool lockC;
bool alive;

void print_array() {
  int index_aux;
  for (index_aux = 0; index_aux < 30; index_aux++){
    printf("%d: %c\n", index_aux, array_char[index_aux]);
  }
}

void insert_character_A_function( void *params ){
  while(alive) {
    printf("Thread A\n");
    while(lockA);
    array_char[index_array++] = 'A';
    lockA = true;
    lockB = false;
  }
}

void insert_character_B_function(void *params) {
  while(alive) {
    while(lockB);
    array_char[index_array++] = 'B';
    lockB = true;
    lockC = false;
  }
}

void insert_character_C_function(void *params) {
  while(alive) {
    while(lockC);
      array_char[index_array++] = 'C';
      lockC = true;
      lockA = false;

      if (index_array > 30){

        print_array();

        alive = false;
        lockA = false;
        lockB = false;
    }
  }
}

void start_thread() {

  int thread_id;

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(insert_character_A_function,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(insert_character_A_function,4096, NULL)) == -1) { printf("Erro na thread 1.\n"); }
  #endif

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(insert_character_B_function,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(insert_character_B_function,4096, NULL)) == -1) { printf("Erro na thread 2.\n"); }
  #endif

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(insert_character_C_function,4096, NULL)) == (unsigned long)-1);
  #else
    if ((thread_id = _beginthread(insert_character_C_function,4096, NULL)) == -1) { printf("Erro na thread 3.\n"); }
  #endif
}

int main(int argc, char *argv[]){

  alive = true;
  lockA = false;
  lockB = true;
  lockC = true;
  index_array = 0;

  start_thread();

  system("PAUSE");
  
  return 0;
}