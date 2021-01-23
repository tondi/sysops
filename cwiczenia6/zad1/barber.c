#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "queue_utils.h"

key_t key;
int sem_id;
int shm_id;
struct Queue *data;
int queueSize;

void handleExit()
{
    releaseSemaphores(sem_id);
    shmctl(shm_id, IPC_RMID, NULL);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./barber <ilosc miejsc w kolejce>\n");
        return 0;
    }
    signal(SIGTERM, handleExit);
    signal(SIGINT, handleExit);
    key = createKey("queuekey.txt");
    shm_id = initShm(key, sizeof(struct Queue));
    sem_id = createSemaphores(key, 2);
    queueSize = atoi(argv[1]);
    data = (struct Queue *) getShmPtr(shm_id);
    queueInit(data, queueSize);
    semaphoreAdd(sem_id, BARBER_QUEUE_ID, 1);
    semaphoreAdd(sem_id, CLIENT_QUEUE_ID, 1);
    printf("Queue size: %d.\n", data->size);

    for(;;)
    {
        semaphoreAdd(sem_id, CLIENT_QUEUE_ID, -1);
        int x = queueEmpty(data);
        semaphoreAdd(sem_id, CLIENT_QUEUE_ID, 1);
        if (x == 1)
        {
            printf("Zaśnięcie.\n");
            semaphoreAdd(sem_id, BARBER_QUEUE_ID, -1);
            semaphoreAdd(sem_id, BARBER_QUEUE_ID, -1);
            printf("Obudzenie\n");
        }
        semaphoreAdd(sem_id, CLIENT_QUEUE_ID, -1);
        printf("Queue in: %d, Queue out: %d\n", data->in, data->out);
        int pid = queueGet(data);
        semaphoreAdd(sem_id, CLIENT_QUEUE_ID, 1);
        printf("Zaproszenie klienta %d z poczekalni do strzyżenia.\n", pid);
        printf("Rozpoczęcie strzyżenia klienta %d.\n", pid);
        sleep(5);
        printf("Zakończenie strzyzenia %d.\n", pid);
        kill(pid, SIGUSR1);
    }
    handleExit();
}
