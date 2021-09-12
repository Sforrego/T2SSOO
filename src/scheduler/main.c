#include <stdio.h>

#include "../file_manager/manager.h"
#include "process.h"
#include "queue.h"
#include <stdbool.h>

int current_time = 0;

int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = argv[1];

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
    
  }

  while(true){

    sleep(1);
    current_time++;
  }

}
