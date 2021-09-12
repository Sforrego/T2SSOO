// Tells the compiler to compile this file once
#pragma once

typedef struct Process{
  int pid;
  char *name;
  int factory_number;
  char *status; // RUNNING READY WAITING FINISHED
}Process; 

