#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "queue.h"

//sem1:
//  0 - fryzjer pracuje
//  <0 -fryzjer spi
//sem2: 
//  0 - klient moze wejsc do kolejki
// <0 - klient bedzie mogl wejsc do kolejki pozniej 

key_t k;
int semid;
int shmid;
struct Queue* data;
int numOfSems = 2;
int N = 5;

void onExit(int t){
    ReleaseSemaphores(semid);
    
    shmctl(shmid, IPC_RMID, NULL);
    
    exit(0);
}

int main(int c, char* v[]){
    if(c < 2){
        printf("Uzycie programu: ./b N\n");
        printf("N - liczba siedzen w kolejce do golibrody.\n");
        return 0;
    }
    signal(SIGINT, onExit);
    signal(SIGTERM, onExit);
    
    //printf("%ld\n%ld\n%ld\n", sizeof(struct Queue), sizeof(int), sizeof(int*));
    N = atoi(v[1]);
    k = CreateKey("newkey.txt", 4);
    semid = CreateSemaphores(k, numOfSems);
    shmid = CreateSharedMem(k, sizeof(struct Queue));
    data = (struct Queue*)GetMemPointer(shmid);
    QInit(data, N);
    Add(semid, QUEUE_ID, 1);
    Add(semid, BARBER_ID, 1);
    while(1){
        Add(semid, QUEUE_ID, -1);
        int v = QEmpty(data);
        Add(semid, QUEUE_ID, 1);
        if(v == 1){
            printf("Zasypianie.\n");
            Add(semid, BARBER_ID, -1);
            Add(semid, BARBER_ID, -1);
            printf("Wstaje\n");
        }
        Add(semid, QUEUE_ID, -1);
        printf("Q: in = %d, out = %d, size = %d.\n", data->queueIn, data->queueOut, data->size);
        int pid = QGet(data);
        Add(semid, QUEUE_ID, 1);
        printf("Strzyrzenie %d.\n", pid);
        
        //Symulacja pracy
        sleep(3);
        
        printf("Zatrzymanie strzyzenia %d.\n", pid);
        kill(pid, SIGUSR1);
    }
    
    
    onExit(0);
}













