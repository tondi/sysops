#include <unistd.h>
#include <signal.h>

#include "lib.h"
#include "queue.h"

#define QUEUE ("sem1")
#define BARBER ("sem2")
#define BARBER_TEST ("sem3")

sem_t* SemQ;
sem_t* SemB;
sem_t* SemT;
struct Queue* Data;
size_t N;

void handleExit(int t){
    CloseSemaphore(SemQ);
    CloseSemaphore(SemB);
    CloseSemaphore(SemT);
    RemoveSemaphore(BARBER);
    RemoveSemaphore(QUEUE);
    RemoveSemaphore(BARBER_TEST);
    ReleaseSharedMem(Data, sizeof(struct Queue), QUEUE);
    exit(0);
}

int main(int c, char* v[]){
    if(c < 2){
        printf("Uzycie programu: ./barber N\n");
        printf("N - liczba siedzen w kolejce do golibrody.\n");
        return 0;
    }
    signal(SIGINT, handleExit);
    signal(SIGTERM, handleExit);
    SemQ = CreateSemaphore(QUEUE, 1);
    SemB = CreateSemaphore(BARBER, 1);
    SemT = CreateSemaphore(BARBER_TEST, 1);
    
    N = atoi(v[1]);
    Data = (struct Queue*)CreateSharedMem(QUEUE, sizeof(struct Queue));
    QueueInit(Data, N);
    
    while(1){
        Take(SemQ);
            int v = QueueEmpty(Data);
        Release(SemQ);
        if(v == 1){
            printf("Zaśnięcie.\n");
                Take(SemB);
                Take(SemB);
            printf("Obudzenie.\n");
        }
        Take(SemQ);
            int pid = QueueGet(Data);
        Release(SemQ);
        
        printf("Strzyzenie %d.\n", pid);
        
        //praca
        sleep(1);

        printf("Zakończenie strzyzenia %d.\n", pid);
        kill(pid, SIGUSR1);
    }
    
    handleExit(0);
}
















