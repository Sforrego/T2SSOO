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

void super_sort(Process *incoming[8], int incoming_count)
{
  Process *swap;
  for (int j = 0; j < incoming_count - 1; j++)
  { // son 8 en total pero comparamos desde el penultimo
    for (int i = 0; i < incoming_count - j - 1; i++)
    {
      if (incoming[i]->factory_number > incoming[i + 1]->factory_number)
      {
        swap = incoming[i];
        incoming[i] = incoming[i + 1];
        incoming[i + 1] = swap;
      }
      else if (incoming[i]->factory_number == incoming[i + 1]->factory_number && strcmp(incoming[i]->name, incoming[i + 1]->name) > 0)
      {
        swap = incoming[i];
        incoming[i] = incoming[i + 1];
        incoming[i + 1] = swap;
      }
    }
  }
};

// Calculemos el Qi!
int calculate_qi(LinkedList *queue, Process *process)
{
  int factory_process = 0; // procesos de la misma fabrica que el proceso a pasar a cpu
  int factories[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
  int factory_count = 0;
  Node *tmp = queue->front; // temporal es el primer nodo de la cola

  for (int i = 0; i < queue->count; i++) // itero por la cola
  {
    if (tmp->data->factory_number == process->factory_number) // reviso cuantos procesos son de la misma fabrica que el proceso a pasar a cpu
    {
      factory_process++;
    }
    for (int j = 0; j < 8; j++)
    {
      if (factories[j] == tmp->data->factory_number) // si encuentro la fabrica del proceso que estamos revisando no la agrego
      {
        break;
      }
      else if (factories[j] == -1) // si es que no encuentro la fabrica entonces la agrego
      {
        factories[j] = tmp->data->factory_number;
        factory_count++;
        break;
      }
    }
    tmp = tmp->next;
  }

  int result = Q / (factory_process * factory_count);
  return result;
}

int main(int argc, char **argv)
{
  // initialize queue and linkedlist
  process_queue = malloc(sizeof(LinkedList));
  process_not_queue = malloc(sizeof(LinkedList));
  initializel(process_queue);
  initializel(process_not_queue);

  incoming[1] = NULL;

  if (argc == 4)
  {
    printf("Q IS %s\n", argv[3]);
    Q = atoi(argv[3]);
  }

  printf("SIZES LL: %ld, NODE:%ld, PROCESS:%ld, INT:%ld", sizeof(LinkedList), sizeof(Node), sizeof(Process), sizeof(int));

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
    int total_bursts = 2 * atoi(line[3]) - 1;
    Process *new_process = (Process *)malloc(sizeof(struct Process) + total_bursts * sizeof(int));
    new_process->factory_number = atoi(line[2]);
    new_process->init_time = atoi(line[1]);
    new_process->name = line[0];
    new_process->pid = process_count;
    new_process->status = "READY";
    new_process->current_burst = 0;
    new_process->total_bursts = total_bursts;
    new_process->created = 0;
    new_process->response_time_registered = 0;
    new_process->times_interrupted = 0;
    new_process->turnaround_time = 0;
    new_process->response_time = 0;
    new_process->waiting_time = 0;
    new_process->times_chosen = 0;

    for (int i = 0; i < total_bursts; i++)
    {
      new_process->burst_array[i] = atoi(line[4 + i]);
    }
    append(process_not_queue, new_process);
    process_count++;
  }

  FILE *fw = fopen(argv[2], "w");

  while (true)
  {
    if (process_not_queue->count == 0 && process_queue->count == 0 && process_running == NULL)
    {
      break;
    }

    incoming_count = 0;

    if (process_running)
    { // 1. if there is a process in the cpu
      process_running->burst_array[process_running->current_burst]--;
      quantum--; // Tick down quantum, and check if quantum reaches 0, and update the process burst
      if (quantum <= 0)
      {
        process_running->times_interrupted++;
      }

      if (process_running->burst_array[process_running->current_burst] == 0)
      { // Proceso cede la CPU pasa a wait y se va al final de la cola
        // check if process finished
        if (process_running->current_burst == process_running->total_bursts - 1)
        {
          process_running->status = "FINISHED";
          process_running->turnaround_time = current_time - process_running->init_time;
          printf("[t = %d] El proceso %s ha pasado a estado FINISHED.\n", current_time, process_running->name);
          // write to file here, check speed
          fprintf(fw, "%s,%d,%d,%d,%d,%d\n", process_running->name,
                  process_running->times_chosen, process_running->times_interrupted, process_running->turnaround_time, process_running->response_time, process_running->waiting_time);
          free(process_running);
          process_running = NULL;
        }
        else
        {
          process_running->status = "WAITING";
          process_running->current_burst++; // Now IO burst
          incoming[0] = process_running;    // proceso se une al grupo que entra a la cola en 2
          incoming_count++;
          printf("[t = %d] El proceso %s ha pasado a estado WAITING.\n", current_time, process_running->name);
          process_running = NULL;
        }
      }
      else if (quantum <= 0)
      {
        process_running->status = "READY";
        incoming[0] = process_running; // proceso se une al grupo que entra a la cola en 2
        incoming_count++;
        printf("[t = %d] El proceso %s ha pasado a estado READY.\n", current_time, process_running->name);
        process_running = NULL;
      }
    }
    // check if there is any process to add to the queue, calculate new quantum, add waiting process to queue
    // 2. procesos creados entran a la cola
    if (process_not_queue->count + incoming_count > 0)
    {
      Node *tmp = process_not_queue->front;
      Node *tmp2;

      for (int i = incoming_count; i < process_not_queue->count; i++)
      {
        tmp2 = tmp->next;
        if (tmp->data->init_time == current_time)
        {
          incoming[incoming_count] = tmp->data;
          remove_node(process_not_queue, i - incoming_count, 1);
          incoming_count++;
        }

        tmp = tmp2;
      }
      if (incoming_count > 0)
      {
        super_sort(incoming, incoming_count);
        for (int i = 0; i < incoming_count; i++)
        {
          append(process_queue, incoming[i]);
          if (!incoming[i]->created)
          {
            printf("[t = %d] El proceso %s ha sido creado.\n", current_time, incoming[i]->name);
            incoming[i]->created = 1;
          }
        }
      }
    }

    // 3. move process to CPU
    if (process_running == NULL && process_queue->count > 0)
    {
      Node *tmp = process_queue->front;
      for (int i = 0; i < process_queue->count; i++)
      {
        if (strcmp(tmp->data->status, "READY") == 0)
        {
          process_running = tmp->data;
          tmp->data->status = "RUNNING";
          tmp->data->times_chosen++;
          if (!tmp->data->response_time_registered)
          {
            tmp->data->response_time = current_time - tmp->data->init_time;
            tmp->data->response_time_registered = 1;
          }
          printf("[t = %d] El proceso %s ha pasado a estado RUNNING.\n", current_time, tmp->data->name);

          // calculate qi
          quantum = calculate_qi(process_queue, process_running);
          remove_node(process_queue, i, 1);
          break;
        }

        tmp = tmp->next;
      }

      if (process_running == NULL)
      {

        printf("[t = %d] No hay ningun proceso ejecutando en la CPU.\n", current_time);
      }
      // 5. update waiting
      tmp = process_queue->front;
      for (int i = 0; i < process_queue->count; i++)
      {
        if (strcmp(tmp->data->status, "WAITING") == 0 || strcmp(tmp->data->status, "READY") == 0)
        {
          tmp->data->waiting_time++;
        }
        if (strcmp(tmp->data->status, "WAITING") == 0)
        {
          tmp->data->burst_array[tmp->data->current_burst]--;
          if (tmp->data->burst_array[tmp->data->current_burst] == 0)
          {
            tmp->data->status = "READY";
            tmp->data->current_burst++;
            printf("[t = %d] El proceso %s ha pasado a estado READY.\n", current_time + 1, tmp->data->name);
          }
        }
        tmp = tmp->next;
      }
    }

    current_time++;
  }

  free(process_queue);
  free(process_not_queue);
  fclose(fw);
  input_file_destroy(file);
}
