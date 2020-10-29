#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "martian.c"
#include "report.c"

#define TIME 100 // Solo para testear
#define N 3 // Solo para testear

int mode;
int* Offsets;
int Offsets_len;
int manual_insert;

pthread_t* planner; 
pthread_t* threads;

int lcm(node_p* head) { 
    if (head == NULL) {
        return 0;
    }
    node_p* temp = head;
    while (temp->State == 0) {
        temp = temp->Next_Process;
    }
    int multiple = temp->Period;
    while (temp->Next_Process != NULL) {
        if (temp->Next_Process->State == 1) {
            multiple = (multiple > temp->Next_Process->Period) 
            ? multiple : temp->Next_Process->Period;
        }
        temp = temp->Next_Process;
    }

    int found;
    while (1) {
        found = 1;
        temp = head;
        while (temp != NULL) {
            if (temp->State == 1) {
                if (multiple % temp->Period != 0) {
                    found = 0;
                    break;
                }
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
        if ((current_cycle + *(temp->Offset)) % temp->Period == 0 && temp->State == 1) {
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
                if (temp_for_count->State == 1) {
                    tmp_cycle = current_cycle;
                    while (1) {
                        tmp_cycle++;
                        if (((tmp_cycle + *(temp_for_count->Offset)) 
                            % temp_for_count->Period == 0 
                            || tmp_cycle - current_cycle == temp->Current_Energy)) {
                            break;
                        }
                    }
                    if (cycles == 0 || cycles > tmp_cycle - current_cycle) {
                        cycles = tmp_cycle - current_cycle;
                    }
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
        if (temp->State == 1) {
            tmp_deadline = current_cycle;
            while (1) {
                tmp_deadline++;
                if ((tmp_deadline + *(temp->Offset)) % temp->Period == 0) {
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

void close_threads(node_p* head, int end) {
    finished = 1;
    node_p* temp = head;
    while (temp != NULL) { 
        if (temp->State == 0) {
            temp = temp->Next_Process;
            continue;
        }
        pthread_mutex_lock(&lock_turn);
        turn = temp->Id;
        set_end(turn, end);
        pthread_cond_broadcast(&cond_turn);
        pthread_mutex_unlock(&lock_turn);
        temp = temp->Next_Process;
        usleep(25000);
    }
}

int create_offset() {
    Offsets_len++;
    int* Offsets_temp = (int*)realloc(Offsets, Offsets_len);
    if (Offsets_temp == NULL) { 
        free(Offsets);
        return -1;
    } 
    else {
        Offsets = Offsets_temp; 
        *(Offsets + Offsets_len - 1) = 0;
        return 0;
    }
}

void update_offsets(int offset) {
    for (int i = 0; i < Offsets_len; i++) {
        *(Offsets + i) += offset;
    }
}

void* planning (void* vargp){
    node_p* head = (node_p*)vargp;
    int i = 0;
    while (i < TIME) {

        if (current_cycle == multiple) {
            current_cycle = 0;
        }

        if (i == 6 && manual_insert == 1) { 
            /* Simula el efecto de un proceso nuevo manual, pero fijado en 6 
            (no deberia de estarlo) */
            update_offsets(current_cycle);
            current_cycle = 0;
            if (create_offset() == -1) {
                printf("Couldn't create the new offset!\n");
                close_threads(head, i);
                return NULL;
            }
            head = add_node(head, size_list, 2, 9, Offsets + Offsets_len - 1);
            add_data(size_list - 1, i, 2, 9);
            proc_len++;
            // Los datos 2 (T) y 9 (P) deben ser extraidos del GUI
            multiple = lcm(head);
            pthread_t* threads_temp = (pthread_t*)realloc(threads, size_list);
            if (threads_temp == NULL) { 
                printf("Couldn't create the new thread!\n");
                close_threads(head, i);
                return NULL;
            } 
            else {
                threads = threads_temp;
            }
            node_p* temp = head;
            while (temp->Id != size_list - 1) {
                temp = temp->Next_Process;
            }
            pthread_create(threads + size_list - 1, NULL, exec_thread, (void *) temp);
            manual_insert = 0;
        }

        if (i == 6 && manual_finish == -1) {
            manual_finish = 0; // Borrar
        }

        if (regen_energy(head) == -1) {
            printf("Couldn't find a way to schedule the processes!\n");
            close_threads(head, i);
            return NULL;
        }

        if (mode == 0) {
            give_turn_rms(head);
        }
        else {
            give_turn_edf(head);
        }

        int temp_turn = turn;
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
            i += temp_cycle;
            node_p* temp = head;
            while (temp->Id != temp_turn) {
                temp = temp->Next_Process;
            }
            if (temp->State == 0) {
                multiple = lcm(head);
                set_end(temp->Id, i);
            }
            add_executed_cycle(temp_turn, temp_cycle);
            usleep(25000);
        }
        else {
            printf("skipped!\n");
            current_cycle++;
            i++;
            add_executed_cycle(-1, 1);
        }
    }
    printf("Calendarization ended!\n");
    close_threads(head, i);
}

int main() {

    /* INIT */

    srand(time(NULL)); // Para los random de report

    node_p* head = NULL;
    size_list = 0;
    int times[] = {1, 2, 6}; 
    int periods[] = {6, 9, 18}; 
    Offsets = (int*)malloc(sizeof(int));
    *Offsets = 0;
    Offsets_len = 1;

    for (int i = 0; i < N; i++) { 
        head = add_node(head, i, times[i], periods[i], Offsets);
        add_data(i, 0, times[i], periods[i]);
    }
    proc_len = size_list;

    /* Planner and threads */
    planner = (pthread_t*)malloc(sizeof(pthread_t));
    threads = (pthread_t*)malloc(sizeof(pthread_t) * size_list);

    /* Global variables */

    turn = -1;
    finished = 0;
    cycles = 0;
    multiple = lcm(head);
    executed = 0;
    current_cycle = 0;
    mode = 0; // Input del usuario
    manual_insert = 0; // Borrar
    manual_finish = -2; // Borrar

    /* Start threads */
    
    node_p* temp = head;
    for (int i = 0; i < size_list; i++) { 
        pthread_create(threads + i, NULL, exec_thread, (void *) temp);
        temp = temp->Next_Process;
    }
    
    pthread_create(planner, NULL, planning, (void *) head);

    /* Join threads */

    pthread_join(*planner, NULL);
    
    for (int i = 0; i < size_list; i++) {
        pthread_join(*(threads + i), NULL);
    }

    /* Show report */

    mode_r = mode;
    show_report();

    return 0;
}

/*
gcc planner.c -o planner -lpthread $(pkg-config allegro-5 allegro_font-5 --libs --cflags)

./planner
*/