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

void onExit(int t){
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
        printf("Uzycie programu: ./b N\n");
        printf("N - liczba siedzen w kolejce do golibrody.\n");
        return 0;
    }
    signal(SIGINT, onExit);
    signal(SIGTERM, onExit);
    SemQ = CreateSemaphore(QUEUE, 1);
    SemB = CreateSemaphore(BARBER, 1);
    SemT = CreateSemaphore(BARBER_TEST, 1);
    
    N = atoi(v[1]);
    Data = (struct Queue*)CreateSharedMem(QUEUE, sizeof(struct Queue));
    QInit(Data, N);
    
    while(1){
        Take(SemQ);
            int v = QEmpty(Data);
        Release(SemQ);
        if(v == 1){
            printf("Zasypianie.\n");
                Take(SemB);
                Take(SemB);
            printf("Wstaje.\n");
        }
        Take(SemQ);
            int pid = QGet(Data);
        Release(SemQ);
        
        printf("Strzyrzenie %d.\n", pid);
        
        //praca
        sleep(1);

        printf("Zatrzymanie strzyzenia %d.\n", pid);
        kill(pid, SIGUSR1);
    }
    
    onExit(0);
}
















