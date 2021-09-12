// Tells the compiler to compile this file once
#pragma once
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

// https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
typedef struct Node{
    struct Process data;
    struct Node* next;
}Node;

typedef struct Queue {
  int count;
  Node *front;
  Node *rear;
}Queue;


void initializeq(Queue *q){
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
};

int isemptyq(Queue *q){
    return (q->rear==NULL);
}

void enqueue(Queue *q, Process new_process){
    Node *tmp;
    tmp = (Node *) malloc(sizeof(Node));
    tmp->data = new_process;
    tmp->next = NULL;
    if(!isempty(q)){
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else {
        q->front = q->rear = tmp;
    }
    q->count++;
}

Process dequeue(Queue *q){
    Node *tmp;
    Process removed_process = q->front->data;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);
    return(removed_process);
}