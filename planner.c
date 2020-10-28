#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "martian.c"

int mode;

int lcm(node_p* head) { 
    node_p* temp = head;
    int multiple = temp->Period;
    while (temp->Next_Process != NULL) {
        multiple = (multiple > temp->Next_Process->Period) 
            ? multiple : temp->Next_Process->Period;
        temp = temp->Next_Process;
    }

    int found;
    while (1) {
        found = 1;
        temp = head;
        while (temp != NULL) {
            if (multiple % temp->Period != 0) {
                found = 0;
                break;
            }
            temp = temp->Next_Process;
        }
        if (found == 1){
            break;
        }
        multiple++;
    }
    return multiple;
}

int regen_energy(node_p* head) {
    node_p* temp = head;
    while (temp != NULL) {
        if (current_cycle % temp->Period == 0) {
            if (temp->Current_Energy != 0) {
                return -1;
            }
            temp->Current_Energy = temp->Max_Energy;
        }
        temp = temp->Next_Process;
    }
    return 0;
}

void give_turn_rms(node_p* head) {
    node_p* temp = head;
    while (temp != NULL) {
        if (temp->Current_Energy > 0) {
            turn = temp->Id;
            int tmp_cycle;
            node_p* temp_for_count = head;
            while (temp_for_count != NULL) {
                tmp_cycle = current_cycle;
                while (1) {
                    tmp_cycle++;
                    if (tmp_cycle % temp_for_count->Period == 0 
                    || tmp_cycle - current_cycle == temp->Current_Energy) {
                        break;
                    }
                }
                if (cycles == 0 || cycles > tmp_cycle - current_cycle) {
                    cycles = tmp_cycle - current_cycle;
                }
                temp_for_count = temp_for_count->Next_Process;
            }
            return;
        }
        temp = temp->Next_Process;
    }
}

void give_turn_edf(node_p* head) {
    node_p* temp = head;
    int deadline = 0;
    int min_deadline = 0;
    int tmp_deadline = 0;
    while (temp != NULL) {
        tmp_deadline = current_cycle;
        while (1) {
            tmp_deadline++;
            if (tmp_deadline % temp->Period == 0) {
                break;
            }
        }
        if (min_deadline == 0 || tmp_deadline <= min_deadline) {
            min_deadline = tmp_deadline;
        }
        if ((deadline == 0 || deadline > tmp_deadline) && temp->Current_Energy > 0) {
            turn = temp->Id;
            deadline = tmp_deadline;
        }
        temp = temp->Next_Process;
    }
    if (deadline != 0) {
        temp = head;
        while (temp->Id != turn) {
            temp = temp->Next_Process;
        }
        int final_deadline = (min_deadline < deadline) ? min_deadline : deadline;
        cycles = final_deadline - current_cycle;
        if (temp->Current_Energy <= final_deadline - current_cycle) {
            cycles = temp->Current_Energy;
        }
    }
}

void close_threads() {
    finished = 1;
    int n = size_list;
    for (int i = 0; i < n; i++) { 
        pthread_mutex_lock(&lock_turn);
        turn = i;
        pthread_cond_broadcast(&cond_turn);
        pthread_mutex_unlock(&lock_turn);
        usleep(25000);
    }
}

void* planning (void* vargp){
    node_p* head = (node_p*)vargp;
    while (current_cycle < 19) {
        if (regen_energy(head) == -1) {
            printf("Couldn't find a way to schedule the processes!\n");
            close_threads();
            return NULL;
        }
        if (mode == 0) {
            give_turn_rms(head);
        }
        else {
            give_turn_edf(head);
        }
        int temp_cycle = cycles;
        printf("Turn %d: ", current_cycle);
        if (turn != -1) {
            pthread_mutex_lock(&lock_turn);
            pthread_cond_broadcast(&cond_turn);
            pthread_mutex_unlock(&lock_turn);
            pthread_mutex_lock(&lock_exec);
            while (executed == 0) {
                pthread_cond_wait(&cond_exec, &lock_exec);
            }
            executed = 0;
            pthread_mutex_unlock(&lock_exec);
            current_cycle += temp_cycle;
            usleep(25000);
        }
        else {
            printf("skipped!\n");
            current_cycle++;
        }
    }
    printf("Calendarization ended!\n");
    close_threads();
}

int main() {

    /* INIT */

    node_p* head = NULL;
    // size_list = 3;
    // int times[] = {1, 2, 6}; 
    // int periods[] = {6, 9, 18}; 
    size_list = 2;
    int times[] = {3, 4}; 
    int periods[] = {6, 9}; 

    for (int i = 0; i < size_list; i++) { 
        head = add_node(head, i, times[i], periods[i]);
    }

    /* Planner and threads */

    pthread_t* planner = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * size_list);

    /* Global variables */

    turn = -1;
    finished = 0;
    cycles = 0;
    multiple = lcm(head);
    executed = 0;
    current_cycle = 0;
    mode = 0;

    /* Start threads */
    
    node_p* temp = head;
    for (int i = 0; i < size_list; i++) { 
        pthread_create(threads + i, NULL, exec_thread, (void *) temp);
        temp = temp->Next_Process;
    }

    pthread_create(planner, NULL, planning, (void *) head);

    /* Join threads */

    for (int i = 0; i < size_list; i++) {
        pthread_join(*(threads + i), NULL);
    }

    pthread_join(*planner, NULL);

    return 0;
}