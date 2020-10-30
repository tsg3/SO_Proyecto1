#include "movements.c"

node_p *add_node(node_p *head, int id, int energy, int period, int *offset, float pos_x, float pos_y, char direction)
{

    if (head == NULL)
    {
        head = (node_p *)malloc(sizeof(node_p));
        head->Id = id;
        head->Max_Energy = energy;
        head->Current_Energy = 0;
        head->Period = period;
        head->Next_Process = NULL;
        head->Offset = offset;
        head->State = 1;
        head->pos_x = pos_x;
        head->pos_y = pos_y;
        head->direction = direction;
        return head;
    }
    else if (period <= head->Period)
    {
        node_p *new_head = (node_p *)malloc(sizeof(node_p));
        new_head->Id = id;
        new_head->Max_Energy = energy;
        new_head->Current_Energy = 0;
        new_head->Period = period;
        new_head->Next_Process = head;
        new_head->Offset = offset;
        new_head->State = 1;
        new_head->pos_x = pos_x;
        new_head->pos_y = pos_y;
        new_head->direction = direction;
        return new_head;
    }
    else
    {
        node_p *temp = head;
        while (temp->Next_Process != NULL)
        {
            if (period <= temp->Next_Process->Period)
            {
                break;
            }
            temp = temp->Next_Process;
        }
        node_p *new_node = (node_p *)malloc(sizeof(node_p));
        new_node->Id = id;
        new_node->Max_Energy = energy;
        new_node->Current_Energy = 0;
        new_node->Period = period;
        new_node->Next_Process = temp->Next_Process;
        new_node->Offset = offset;
        new_node->State = 1;
        new_node->pos_x = pos_x;
        new_node->pos_y = pos_y;
        new_node->direction = direction;
        temp->Next_Process = new_node;
        return head;
    }
}

void print_list(node_p *head)
{
    node_p *temp = head;
    printf("[");
    while (temp != NULL)
    {
        printf("(%d, %d, %d, %d)",
               temp->Id, temp->Max_Energy, temp->Current_Energy, temp->Period);
        temp = temp->Next_Process;
        if (temp != NULL)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void *exec_thread(void *vargp)
{
    node_p *proc = (node_p *)vargp;
    printf("T%d Started!\n", proc->Id);
    while (1)
    {
        pthread_mutex_lock(&lock_turn);
        while (turn != proc->Id)
        {
            pthread_cond_wait(&cond_turn, &lock_turn);
        }
        if (finished == 1)
        {
            turn = -1;
            printf("T%d Finished!\n", proc->Id);
            proc->State = 0;
            pthread_mutex_unlock(&lock_turn);
            break;
        }
        turn = -1;
        int temp_cycles = cycles;
        while (cycles > 0)
        {
            if (!keep_execution){
                break;
            }
            make_movement(proc);

            // if (steps == 0)
            // {
                // proc->direction = 'c';
                proc->Current_Energy--;
                cycles--;
                usleep(time_pause);
            //     steps = 8;
            // }
        }
        printf("T%d executed %d cycles!\n", proc->Id, temp_cycles);
        pthread_mutex_unlock(&lock_turn);
        pthread_mutex_lock(&lock_exec);
        executed = 1;
        pthread_cond_signal(&cond_exec);
        pthread_mutex_unlock(&lock_exec);
        if (proc->State == 0)
        {
            printf("T%d finished!\n", proc->Id);
            break;
        }
    }
}