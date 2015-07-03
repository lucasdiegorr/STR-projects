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

void print_array() {
  int index_aux;
  for (index_aux = 0; index_aux < 30; index_aux++){
    printf("%d: %c\n", index_aux, array_char[index_aux]);
  }
}

void *insert_character_function( void *element ){
     char *character;
     character = (char *) element;
     
     while(alive) {
         
         if (*character == 'A'){

          while(lockA);
          array_char[index++] = 'A';
          lockA = true;
          lockB = false;

         } else if(*character == 'B') {

          while(lockB);
          array_char[index++] = 'B';
          lockB = true;
          lockC = false;

         }else {

          while(lockC);
          array_char[index++] = 'C';
          lockC = true;
          lockA = false;
         
          if (index > 30){
            
            print_array();

            alive = false;
            lockA = false;
            lockB = false;
          }

         }

     }

     pthread_exit(0);

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

   pthread_create( &threadA, NULL, insert_character_function, (void*) element_A );
   pthread_create( &threadB, NULL, insert_character_function, (void*) element_B );
   pthread_create( &threadC, NULL, insert_character_function, (void*) element_C );

   pthread_join( threadA, NULL);
   pthread_join( threadB, NULL);
   pthread_join( threadC, NULL);
   
   return 0;
}