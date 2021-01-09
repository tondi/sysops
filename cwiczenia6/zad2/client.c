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

int over;
int used;
int pid;
int s;

void handleExit(int t){
    CloseSemaphore(SemQ);
    CloseSemaphore(SemB);
    CloseSemaphore(SemT);
    CloseSharedMem(Data, sizeof(struct Queue));
    exit(0);
}


void goToB(){
    used = 0;
    Take(SemQ);
        int r = QueuePut(Data, pid);
    Release(SemQ);
    
    printf("%d: Zajęcie miejsca w poczekalni.\n", pid);
    if(r == -1){
        printf("%d: Kolejka pełna. Opuszczenie zakładu z powodu braku wolnych miejsc w poczekalni.\n", pid);
        handleExit(0);
    }else{
        Take(SemT);
            printf("%d: Budzenie golibrody SemB = %d\n", getpid(), GetValue(SemB));
            if(0 == 0){
                Release(SemB);
                while(GetValue(SemB) != 0);
                Release(SemB);
                printf("%d: Budzenie golibrody.\n", pid);
            }
        Release(SemT);
    }
    while(used == 0);
    if(!over)goToB();
    handleExit(0);
}

void handleFinished(int x){
    s--;
    printf("%d: Konczenie. Potrzeba %d strzyzen.\n", pid, s);
    used = 1;
    if(!s){
        printf("%d: Opuszczanie fryzjera.\n", pid);
        over = 1;
    }
}


int main(int c, char* v[]){
    if(c < 3){
        printf("Uzycie programu: ./client N S\n");
        printf("N - liczba klientow do stworzenia.\n");
        printf("S - liczba strzyzen dla klienta.\n");
        return 0;
    }
    signal(SIGUSR1, handleFinished);
    
    SemQ = GetSemaphore(QUEUE);
    SemB = GetSemaphore(BARBER);
    SemT = GetSemaphore(BARBER_TEST);
    
    Data = (struct Queue*)GetSharedMem(QUEUE, sizeof(struct Queue));
    int N = atoi(v[1]);
    s = atoi(v[2]);
    
    for(int i = 0; i < N; ++i){
        if(fork() == 0){
            pid = getpid();
            printf("%d: Wejscie do golibrody.\n", pid);
            goToB();
            exit(0);
        }
    }
    
    handleExit(0);
}















