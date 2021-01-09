#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "lib.h"
#include "queue.h"

struct Queue* data;
int pid;
int left;

void handleFinish(int x){
    left--;
    printf("%d: Zakonczono strzyzenie. Pozostało %d strzyzen.\n", pid, left);
    if(!left){
        printf("%d: Opuszczenie zakładu po zakończeniu strzyżenia.\n", pid);
        exit(0);
    }
}

void handleExit(int t){}

int main(int c, char* v[]){
    if(c < 3){
        printf("Usage: ./client <ilosc klientow> <ilosc strzyzen dla jednego klienta>\n");
        return 0;
    }
    signal(SIGUSR1, handleFinish);
    // signal(SIGINT, kill);
    
    key_t key = CreateKey("queuekey.txt", 4);
    int semid = GetSemaphores(key);
    int shmid = GetSharedMem(key);
    data = (struct Queue*)GetMemPointer(shmid);
    int N = atoi(v[1]);
    left = atoi(v[2]);
    for(int i = 0; i < N; ++i){
        if(fork() == 0){
            pid = getpid();
            printf("%d: Wejscie do golibrody.\n", pid);
            while(1){
                Add(semid, QUEUE_ID, -1);
                int r = QueuePut(data, getpid());
                Add(semid, QUEUE_ID, 1);
                printf("%d: Zajęcie miejsca w poczekalni. Pozostalych strzyzen: %d\n", pid, left);

                if(r == -1){
                    printf("%d: Kolejka pełna. Opuszczenie zakładu z powodu braku wolnych miejsc w poczekalni.\n", pid);
                    exit(0);
                }else if(IsZero(semid, BARBER_ID)){
                    printf("%d: Budzenie golibrody.\n", pid);
                    printf("%d: Zajęcie miejsca na krześle do strzyżenia.\n", pid);
                    Add(semid, BARBER_ID, 2);
                }
                pause();
            }
            break;
        }
    }
    //pause();
}
