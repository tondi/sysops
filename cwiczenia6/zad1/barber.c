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

key_t k;
int semid;
int shmid;
struct Queue* data;
int numOfSems = 2;
int N = 5;

void handleExit(int t){
    ReleaseSemaphores(semid);
    
    shmctl(shmid, IPC_RMID, NULL);
    
    exit(0);
}

int main(int c, char* v[]){
    if(c < 2){
        printf("Usage: ./barber <ilosc miejsc w kolejce>\n");
        return 0;
    }
    signal(SIGINT, handleExit);
    signal(SIGTERM, handleExit);
    
    //printf("%ld\n%ld\n%ld\n", sizeof(struct Queue), sizeof(int), sizeof(int*));
    N = atoi(v[1]);
    k = CreateKey("queuekey.txt", 4);
    semid = CreateSemaphores(k, numOfSems);
    shmid = CreateSharedMem(k, sizeof(struct Queue));
    data = (struct Queue*)GetMemPointer(shmid);
    QueueInit(data, N);
    Add(semid, QUEUE_ID, 1);
    Add(semid, BARBER_ID, 1);
    printf("Queue size: %d.\n", data->size);
    while(1){
        Add(semid, QUEUE_ID, -1);
        int v = QueueEmpty(data);
        Add(semid, QUEUE_ID, 1);
        if(v == 1){
            printf("Zaśnięcie.\n");
            Add(semid, BARBER_ID, -1);
            Add(semid, BARBER_ID, -1);
            printf("Obudzenie\n");
        }
        Add(semid, QUEUE_ID, -1);
        printf("Queue in: %d, Queue out: %d\n", data->queueIn, data->queueOut);
        int pid = QueueGet(data);
        Add(semid, QUEUE_ID, 1);
        printf("Zaproszenie klienta %d z poczekalni do strzyżenia.\n", pid);
        printf("Rozpoczęcie strzyżenia klienta %d.\n", pid);

        //Symulacja pracy
        sleep(3);
        
        printf("Zakończenie strzyzenia %d.\n", pid);
        kill(pid, SIGUSR1);
    }
    
    
    handleExit(0);
}













