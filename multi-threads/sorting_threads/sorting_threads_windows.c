#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>
#include <math.h>

bool alive;
int thread_array[6], index_array;

void sort_and_show() {
  int i, j, swap;
  for (i = 0 ; i < ( 6 - 1 ); i++)
   {
     for (j = 0 ; j < 6 - i - 1; j++){

       if (thread_array[j] > thread_array[j+1]) {
         swap  = thread_array[j];
         thread_array[j]   = thread_array[j+1];
         thread_array[j+1] = swap;
       }
     }
   }

   for ( i = 0; i < 6; ++i){
     printf("A thread %d tem o PID: %d\n", i, thread_array[i]);
   }

   alive = false;
}

void foo() {
  while(alive);
}

bool hasValue(int array[], int value) {
  int i;
  for (i = 0; i <= index_array; i++){
    if (array[i] == value){
      return true;
    }
  }
  return false;
}

void create_thread(void *function) {

  int thread_id;

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(function ,4096, NULL)) == (unsigned long)-1) { printf("Erro na thread %d.\n", index_array); }
  #else
    if ((thread_id = _beginthread(function ,4096, NULL)) == -1) { printf("Erro na thread %d.\n", index_array); }
  #endif

  if (hasValue(thread_array, thread_id)){
    create_thread(function);
  } else {
    thread_array[index_array++] = thread_id;
  }

}

int main(int argc, char const *argv[]){

  alive = true;
  index_array = 0;

  do{
    create_thread(foo);
  } while( index_array < 5 );
  
  create_thread(sort_and_show);

  while(alive);

  system("PAUSE");
  return 0;
} 