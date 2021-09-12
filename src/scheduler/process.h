// Tells the compiler to compile this file once
#pragma once

typedef struct Process{
  int pid;
  char name[255];
  int factory_number;
  char status[10]; // RUNNING READY WAITING FINISHED
}; 

