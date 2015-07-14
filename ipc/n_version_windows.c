#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>
#include <math.h>

int votes[3];
int array_channels[6] = {-1};
bool alive;

void send_message_asynchronous(int *buffered_value, int channel){
  array_channels[channel] = *buffered_value;
  return;
}

void receive_message_synchronous(int *buffered_value, int channel){
  while(array_channels[channel] == -1);
  *buffered_value = array_channels[channel];
  array_channels[channel] = -1;
  return;
}

void function_version_0(void *params) {
  int vote = 5 + 5;
  send_message_asynchronous(&vote, 0);

  int singnal;
  receive_message_synchronous(&singnal, 3);

  while(alive);
}

void function_version_1(void *params) {
  int vote = 2 * 5;
  send_message_asynchronous(&vote, 1);

  int singnal;
  receive_message_synchronous(&singnal, 4);

  while(alive);
}

void function_version_2(void *params) {
  int vote = 3 + 7;
  send_message_asynchronous(&vote, 2);

  int singnal;
  receive_message_synchronous(&singnal, 5);

  while(alive);
}

int create_thread(void *function) {

  int thread_id;

  #if     defined(__WIN32__)
    if ((thread_id = _beginthread(function ,4096, NULL)) == (unsigned long)-1) { printf("Erro na thread.\n"); return -1;}
  #else
    if ((thread_id = _beginthread(function ,4096, NULL)) == -1) { printf("Erro na thread.\n"); return -1;}
  #endif

  return thread_id;

}

int mode(int array[], int length) {
  int *array_occurrence = malloc(length * sizeof(int));
  int index, index_2, mode, aux, max_occurrence;

  for (index = 0; index < length; index++){
    array_occurrence[index] = 0;
  }

  for (index = 0; index < length; index++){
    aux = array[index];
    for (index_2 = 0; index_2 < length; index_2++){
      if (aux == array[index_2]){
        array_occurrence[index] += 1;
      }
    }
  }

  max_occurrence = array_occurrence[0];
  mode = array[0];

  for (index = 1; index < length; index++){
    if (max_occurrence < array_occurrence[index]){
      mode = array[index];
    }
  }

  return mode;

}

void driver(void *params) {

  int array_comparison[3] = {0};
  int majority_vote;

  create_thread(function_version_0);
  create_thread(function_version_1);
  create_thread(function_version_2);

  receive_message_synchronous(&array_comparison[0], 0);
  receive_message_synchronous(&array_comparison[1], 1);
  receive_message_synchronous(&array_comparison[2], 2);

  majority_vote = mode(array_comparison, 3);

}

int main(int argc, char const *argv[]){

  system("PAUSE");
  return 0;
}
