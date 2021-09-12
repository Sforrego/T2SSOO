// Tells the compiler to compile this file once
#pragma once

typedef struct Process{
  int pid;
  char *name;
  int factory_number;
  char *status; // RUNNING READY WAITING FINISHED
  int current_burst;
  int burst_array[1];
  int total_bursts;
  int init_time;
}Process; 

typedef struct Node{
    struct Process* data;
    struct Node* next;
}Node;