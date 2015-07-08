#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__linux__)
  #include <pthread.h>
#else
  #include <process.h>
#endif

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

void insert_character_function( void *element ){
     char *character;
     character = (char *) element;
     
     while(alive) {
         
         if (*character == 'A'){

          while(lockA);
          array_char[index_array++] = 'A';
          lockA = true;
          lockB = false;

         } else if(*character == 'B') {

          while(lockB);
          array_char[index_array++] = 'B';
          lockB = true;
          lockC = false;

         }else {

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
}

int main(int argc, char *argv[]){

  #if defined(__linux__)
    pthread_t threadA, threadB, threadC;
  #else
    int threadA, threadB, threadC;
  #endif

  const char *element_A = "A";
  const char *element_B = "B";
  const char *element_C = "C";

  alive = true;
  lockA = false;
  lockB = true;
  lockC = true;
  index_array = 0;

  #if defined(__linux__)
    pthread_create( &threadA, NULL, insert_character_function, (void*) element_A );
    pthread_create( &threadB, NULL, insert_character_function, (void*) element_B );
    pthread_create( &threadC, NULL, insert_character_function, (void*) element_C );
  #else
    if ((threadA = _beginthread(insert_character_function, 4096, (void*) element_A)) == -1){
      printf("Erro na threadA.");
    }
    if ((threadB = _beginthread(insert_character_function, 4096, (void*) element_B)) == -1){
      printf("Erro na threadB.");
    }
    if ((threadC = _beginthread(insert_character_function, 4096, (void*) element_C)) == -1){
      printf("Erro na threadC."); 
    }
  #endif

  #if defined(__linux__)
    pthread_join( threadA, NULL);
    pthread_join( threadB, NULL);
    pthread_join( threadC, NULL);
  #endif

  return 0;
}
