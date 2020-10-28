#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 

pthread_mutex_t mutex;
int turn;

typedef struct Process {
    int Id;
    int Time;
    int Period;
    struct Process* Next_Process;
} node_p;

node_p* add_node(node_p* head, int id, int time, int period) {
    if (head == NULL) {
        head = (node_p*)malloc(sizeof(node_p));
        head->Id = id;
        head->Time = time;
        head->Period = period;
        head->Next_Process = NULL;
    }
    else {
        node_p* temp = head;
        while (temp->Next_Process != NULL) {
            temp = temp->Next_Process;
        }
        node_p* new_node = (node_p*)malloc(sizeof(node_p));
        new_node->Id = id;
        new_node->Time = time;
        new_node->Period = period;
        new_node->Next_Process = NULL;
        temp->Next_Process = new_node;
    }
    return head;
}

void print_list(node_p* head) {
    node_p* temp = head;
    printf("[");
    while (temp != NULL) {
        printf("(%d, %d, %d)", temp->Id, temp->Time, temp->Period);
        temp = temp->Next_Process;
        if (temp != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}

void* exec_thread(void* vargp) {
    printf("FINISHED\n");
}

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

int main() {
    node_p* head = NULL;
    int times[] = {1, 2, 6};
    int periods[] = {6, 9, 18};

    head = add_node(head, 0, times[0], periods[0]);
    head = add_node(head, 2, times[2], periods[2]);
    head = add_node(head, 1, times[1], periods[1]);
    print_list(head);

    printf("%d\n", lcm(head));

    // pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * 3);

    // turn = head->Id;
    // pthread_mutex_init(&mutex, NULL);

    // node_p* temp = head;
    // for (int i = 0; i < 3; i++) {
    //     pthread_create(threads + i, NULL, exec_thread, (void *) temp);
    //     temp = temp->Next_Process;
    // }

    // for (int i = 0; i < 3; i++) {
    //     pthread_join(*(threads + i), NULL);
    // }

    // pthread_mutex_destroy(&mutex);

    return 0;
}