#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

char array_char[30];
int index;
bool lockA;
bool lockB;
bool lockC;
bool alive;

void *functionA(void *args) {
  while(alive) {
    
    while(lockA);
    array_char[index++] = 'A';
    lockA = true;
    lockB = false;
  }
  pthread_exit(0);
}

void *functionB(void *args) {
  while(alive) {
    
    while(lockB);
    array_char[index++] = 'B';
    lockB = true;
    lockC = false;
  }
  pthread_exit(0);
}

void *functionC(void *args) {
  
  while(alive) {
    
    while(lockC);
    array_char[index++] = 'C';
    lockC = true;
    lockA = false;

    if (index > 30){
      int index_aux;
      for (index_aux = 0; index_aux < 30; index_aux++){
        printf("%d: %c\n", index_aux, array_char[index_aux]);
      }
      alive = false;
      lockA = false;
      lockB = false;
      pthread_exit(0);
    }

  }
}

int main(int argc, char *argv[]){

   pthread_t threadA, threadB, threadC;
   const char *element_A = "A";
   const char *element_B = "B";
   const char *element_C = "C";

   alive = true;
   lockA = false;
   lockB = true;
   lockC = true;
   index = 0;

   pthread_create( &threadA, NULL, functionA, (void*) element_A );
   pthread_create( &threadB, NULL, functionB, (void*) element_B );
   pthread_create( &threadC, NULL, functionC, (void*) element_C );

   pthread_join( threadA, NULL);
   pthread_join( threadB, NULL);
   pthread_join( threadC, NULL);
   
   printf("Fim do Exercício\n");

   return 0;
}
 
