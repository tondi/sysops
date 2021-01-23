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
#include "utils.h"
#include "queue_utils.h"

struct Queue *data;
int pid;
int left;

void handleFinish(int x)
{
    left--;
    printf("%d: Zakonczono strzyzenie. Pozostało %d strzyzen.\n", pid, left);
    if (!left)
    {
        printf("%d: Opuszczenie zakładu po zakończeniu strzyżenia.\n", pid);
        exit(0);
    }
}

void handleExit() {}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: ./client <ilosc klientow> <ilosc strzyzen dla jednego klienta>\n");
        return 0;
    }
    signal(SIGUSR1, handleFinish);

    left = atoi(argv[2]);
    int client_count = atoi(argv[1]);
    key_t key = createKey("queuekey.txt");
    int shm_id = getShm(key);
    int sem_id = getSemaphores(key);
    data = (struct Queue *)getShmPtr(shm_id);
    int i = 0;
    while(i < client_count)
    {
        if (fork() == 0)
        {
            pid = getpid();
            printf("%d: Wejscie do golibrody.\n", pid);
            for(;;)
            {
                semaphoreAdd(sem_id, CLIENT_QUEUE_ID, -1);
                int x = queuePut(data, getpid());
                semaphoreAdd(sem_id, CLIENT_QUEUE_ID, 1);
                printf("%d: Zajęcie miejsca w poczekalni. Pozostalych strzyzen: %d\n", pid, left);

                if (x == -1)
                {
                    printf("%d: Kolejka pełna. Opuszczenie zakładu z powodu braku wolnych miejsc w poczekalni.\n", pid);
                    exit(0);
                }
                else if (isSemaphoreNull(sem_id, BARBER_QUEUE_ID))
                {
                    printf("%d: Budzenie golibrody.\n", pid);
                    printf("%d: Zajęcie miejsca na krześle do strzyżenia.\n", pid);
                    semaphoreAdd(sem_id, BARBER_QUEUE_ID, 2);
                }
                pause();
            }
            break;
        }
        i++;
    }
}
