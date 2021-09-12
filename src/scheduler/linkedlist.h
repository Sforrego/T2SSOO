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

typedef struct LinkedList {
  int count;
  Node *front;
  Node *rear;
}LinkedList;


void initialize(LinkedList *q){
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
};

int isempty(LinkedList *q){
    return (q->rear==NULL);
}

void append(LinkedList *q, Process new_process){
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

Process remove(LinkedList *q, int position){ // removes the node #position
    Node *current_node = q->front;
    Node *tmp;
    Process removed_process;
    if(position>=q->count){
        printf("Tried to removed from a position higher than process count\n");
        return removed_process;
    }
    for(int i=0; i<q->count;i++){
        if (position==0&&q->count==1){ // remove first node of a 1 item list
            tmp = current_node;
            removed_process = tmp->data;
            q->front = NULL;
            free(tmp);
            break;
        }
        else if(i==position){
            tmp = current_node->next;
            removed_process = tmp->data;
            current_node->next = tmp->next;
            free(tmp);
            break;
        }
        current_node = current_node->next;
    }

    return(removed_process);
}