#include <stdio.h>
#include <stdlib.h>
#include "src/scheduler/process.h"
#include <string.h>

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

int main(){
    struct Process *incoming[8];
    Process *new_process = (Process *) malloc(sizeof(struct Process));
    new_process->factory_number = 3;
    new_process->name = "PROCESS WOLOLO";
    Process *new_process2 = (Process *) malloc(sizeof(struct Process));
    new_process2->factory_number = 3;
    new_process2->name = "PROCESS AOLOLO";
    Process *new_process3 = (Process *) malloc(sizeof(struct Process));
    new_process3->factory_number = 1;
    new_process3->name = "PROCESS BOLOLO";
    Process *new_process4 = (Process *) malloc(sizeof(struct Process));
    new_process4->factory_number = 2;
    new_process4->name = "PROCESS BOLOLO";
    Process *new_process5 = (Process *) malloc(sizeof(struct Process));
    new_process5->factory_number = 7;
    new_process5->name = "PROCESS BOLOLO";
    Process *new_process6 = (Process *) malloc(sizeof(struct Process));
    new_process6->factory_number = 6;
    new_process6->name = "PROCESS BOLOLO";
    Process *new_process7 = (Process *) malloc(sizeof(struct Process));
    new_process7->factory_number = 5;
    new_process7->name = "PROCESS BOLOLO";
    Process *new_process8 = (Process *) malloc(sizeof(struct Process));
    new_process8->factory_number = 4;
    new_process8->name = "PROCESS BOLOLO";
    incoming[0] = new_process;
    incoming[1] = new_process2;
    incoming[2] = new_process3;
    incoming[3] = new_process4;
    incoming[4] = new_process5;
    incoming[5] = new_process6;
    incoming[6] = new_process7;
    incoming[7] = new_process8;
    super_sort(incoming,8);
    printf("PROCESS: %s\n",incoming[0]->name);
    printf("PROCESS: %s\n",incoming[1]->name);
    printf("PROCESS: %s\n",incoming[2]->name);
    printf("PROCESS: %s\n",incoming[3]->name);
    printf("PROCESS: %s\n",incoming[4]->name);
    printf("PROCESS: %s\n",incoming[5]->name);
    printf("PROCESS: %s\n",incoming[6]->name);
    printf("PROCESS: %s\n",incoming[7]->name);
    
}