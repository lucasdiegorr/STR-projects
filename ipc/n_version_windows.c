#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <process.h>
#include <math.h>

int votes[3];
int array_channels[6] = {-1, -1, -1, -1, -1, -1};
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
  bool thread_alive = true;
  int vote = 5 + 5;
  send_message_asynchronous(&vote, 0);

  int signal;
  receive_message_synchronous(&signal, 3);

  printf("A thread 0 recebeu o sinal %d\n", signal);

  if (signal == 1){
    printf("A thread 0 recebeu ordem para continuar mudando o voto que sera %d.\n", signal);

  }else if(signal == 2){
    printf("A thread 0 recebeu ordem de parar.\n");
  }else {
    printf("A thread 0 recebeu ordem de continuar a execucao.\n");
  }

  while(alive && thread_alive);
}

void function_version_1(void *params) {
  bool thread_alive = true;
  int vote = 2 * 5;
  send_message_asynchronous(&vote, 1);

  int signal;
  receive_message_synchronous(&signal, 4);

  printf("A thread 1 recebeu o sinal %d\n", signal);

  if (signal == 1){
    printf("A thread 1 recebeu ordem para continuar mudando o voto que sera %d.\n", signal);
    vote = signal;
  }else if(signal == 2){
    printf("A thread 1 recebeu ordem de parar.\n");
  }else {
    printf("A thread 1 recebeu ordem de continuar a execucao.\n");
  }

  while(alive && thread_alive);
}

void function_version_2(void *params) {
  bool thread_alive = true;
  int vote = 3 + 6;
  send_message_asynchronous(&vote, 2);

  int signal;
  receive_message_synchronous(&signal, 5);

  printf("A thread 2 recebeu o sinal %d\n", signal);

  if (signal == 1){
    printf("A thread 2 recebeu ordem para continuar mudando o voto que sera %d.\n", signal);
  }else if(signal == 2){
    printf("A thread 2 recebeu ordem de parar.\n");
  }else {
    printf("A thread 2 recebeu ordem de continuar a execucao.\n");
  }

  while(alive && thread_alive);
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

  int array_comparison[3] = {0,0,0};
  int majority_vote, index, signal_return;

  create_thread(function_version_0);
  create_thread(function_version_1);
  create_thread(function_version_2);

  receive_message_synchronous(&array_comparison[0], 0);
  receive_message_synchronous(&array_comparison[1], 1);
  receive_message_synchronous(&array_comparison[2], 2);

  majority_vote = mode(array_comparison, 3);

  printf("O voto majoritario foi %d\n", majority_vote);

  for(index = 0; index < 3; index++){
    if(array_comparison[index] == majority_vote){
      signal_return = 0;
    }else if((array_comparison[index] >= (majority_vote - 1)) && (array_comparison[index] <= (majority_vote + 1))){
      signal_return = 1;
    }else{
      signal_return = 2;
    }
    printf("Sinal para a thread %d foi %d.\n", index, signal_return);
    send_message_asynchronous(&signal_return, index+3);
  }
  alive = false;
}

int main(int argc, char const *argv[]){

  alive = true;
  create_thread(driver);
  system("PAUSE");
  return 0;
}
