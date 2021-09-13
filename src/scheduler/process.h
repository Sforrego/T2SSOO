// Tells the compiler to compile this file once
#pragma once

typedef struct Process{
  int pid;
  char *name;
  char *status; // RUNNING READY WAITING FINISHED
  int factory_number;
  int init_time;
  int total_bursts;
  int current_burst;
  int burst_array[1];
}Process; 

typedef struct Node{
    struct Process* data;
    struct Node* next;
}Node;