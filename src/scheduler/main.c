#include <stdio.h>

#include "../file_manager/manager.h"
#include "process.h"
#include "linkedlist.h"
#include <stdbool.h>
#include <string.h>

int current_time = 0;
LinkedList *process_queue;
LinkedList *process_not_queue; // processes that have been created but havent joined the queue yet.
int Q = 100;
int quantum = 0;
int process_count = 0; // to assign pid to each process
Process *process_running = NULL;
struct Process *incoming[8];
int incoming_count = 0;

void super_sort(Process *incoming[8], int incoming_count){
    Process *swap;
    for(int j=0;j<incoming_count - 1;j++){ // son 8 en total pero comparamos desde el penultimo
        for(int i=0;i<incoming_count - j - 1;i++){
            if (incoming[i]->factory_number>incoming[i+1]->factory_number){
                swap = incoming[i];
                incoming[i] = incoming[i+1];
                incoming[i+1] = swap;
            } else if (incoming[i]->factory_number == incoming[i+1]->factory_number && strcmp(incoming[i] -> name, incoming[i+1]-> name) > 0) {
                swap = incoming[i];
                incoming[i] = incoming[i+1];
                incoming[i+1] = swap;
                
            }
        }
    }
};

int main(int argc, char **argv)
{
  // initialize queue and linkedlist
  process_queue = malloc(sizeof(LinkedList));
  process_not_queue = malloc(sizeof(LinkedList));
  initializeq(process_queue);
  initializel(process_not_queue);

  incoming[1] = NULL;

  if(argc==4){
    printf("Q IS %s\n", argv[3]);
    Q = argv[3];
  }


  printf("Hello T2!\n");

  char *filename = argv[1];
  InputFile *file = read_file(filename);

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];

    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
    // Create Process and add it to processes not in queue;
    int total_bursts = 2*atoi(line[3])-1;
    char new_process_name[255] = "asd";
    Process *new_process = (Process *) malloc(sizeof(struct Process)+total_bursts*sizeof(int));
    new_process->factory_number = atoi(line[1]);
    new_process->name = new_process_name;
    new_process->pid = process_count;
    new_process->init_time = atoi(line[1]);
    new_process->status = "READY";
    new_process->current_burst = 0;
    new_process->total_bursts = total_bursts;
    printf("TOTAL bursts: %d\n",total_bursts);
    // new_process.burst_array[total_bursts]; 
    for(int i =0;i<total_bursts; i++){
      
      new_process->burst_array[i] = atoi(line[4+i]);
      printf("Potato %d\n",new_process->burst_array[i]);
    }
    // falta guardar los A y B
    append(process_not_queue,new_process);
    process_count++;
  }

  while(true){

    
    if(process_running){ // if there is a process in the cpu
      process_running->burst_array[process_running->current_burst]--;
      quantum--; // Tick down quantum, and check if quantum reaches 0, and update the process burst
      if(process_running->burst_array[process_running->current_burst]==0){ // Proceso cede la CPU pasa a wait y se va al final de la cola
        // check if process finished
        if (process_running->current_burst==process_running->total_bursts-1){
          process_running->status = "FINISHED";
          free(process_running);
          process_running=NULL;
        }
        else {
          process_running->status="WAITING";
          process_running->current_burst++; // Now IO burst
          append(process_queue,process_running); // proceso se va al final de la cola
          process_running = NULL;
        }
      }
      else if(quantum<=0){
        process_running->status = "READY";
        append(process_queue,process_running); // proceso se va al final de la cola
        process_running = NULL;
      }
    }

    // check if there is any process to add to the queue, calculate new quantum, add waiting process to queue
    incoming_count = 0;
    if(process_not_queue->count>0){
      Node *tmp = process_not_queue->front;
      for(int i = 0;i<process_not_queue->count;i++){
        if(tmp->data->init_time==current_time){
          incoming[incoming_count] = tmp->data;
          incoming_count++;
          remove_node(process_not_queue,i-incoming_count);
        }
        tmp = tmp->next;
      }
      super_sort(incoming,incoming_count);
      for(int i = 0;i<incoming_count;i++){
        append(process_queue,incoming[i]);
      }
    }
    // add process to queue
    // calculate qi

    // update waiting

    current_time++;
  }

// stats

}
