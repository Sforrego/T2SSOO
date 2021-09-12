#include <stdio.h>

#include "../file_manager/manager.h"
#include "process.h"
#include "queue.h"
#include "linkedlist.h"
#include <stdbool.h>

int current_time = 0;
Queue *process_queue;
LinkedList *process_not_queue; // processes that have been created but havent joined the queue yet.


bool process_running = false;
int process_count = 0; // to assign pid to each process

int main(int argc, char **argv)
{
  // initialize queue and linkedlist
  process_queue = malloc(sizeof(Queue));
  process_not_queue = malloc(sizeof(LinkedList));
  initializeq(process_queue);
  initializel(process_not_queue);

  


  printf("Hello T2!\n");

  InputFile *file = argv[1];

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];

    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
    // Create Process and add it to processes not in queue;
    char new_process_name[255] = line[0];
    Process new_process;
    new_process.factory_number = atoi(line[1]);
    new_process.name = new_process_name;
    new_process.pid = process_count;
    new_process.status = "READY";
    append(process_not_queue,new_process);
    process_count++;
  }

  while(true){

    sleep(1);
    current_time++;
  }

}
