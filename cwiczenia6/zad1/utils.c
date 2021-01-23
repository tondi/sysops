#include "utils.h"

unsigned short *initialBuffer = NULL;

int initShm(key_t k, int size)
{
    int shm_id = shmget(k, size, 0666 | IPC_CREAT);
    if (shm_id == -1)
    {
        fprintf(stderr, "Error: Cannot create shared memory!\n");
        perror(strerror(errno));
        handleExit();
    }
    return shm_id;
}

int getShm(key_t key)
{
    int shm_id = shmget(key, 0, 0);
    if (shm_id == -1)
    {
        fprintf(stderr, "Error: Cannot access shared memory\n");
        perror(strerror(errno));
        handleExit();
    }
    return shm_id;
}

key_t createKey(char *name)
{
    key_t x = ftok("queuekey.txt", 4);
    if (x == -1)
    {
        fprintf(stderr, "Error: Nie udalo sie stworzyc klucza!\n");
        perror(strerror(errno));
        handleExit();
    }
    return x;
}

int createSemaphores(key_t k, int numOfSems)
{
    int sem_id = semget(k, numOfSems, 0666 | IPC_CREAT);
    if (sem_id == -1)
    {
        fprintf(stderr, "Error: Cannot create semaphore!\n");
        perror(strerror(errno));
        handleExit();
    }
    union semun init;
    init.array = (unsigned short *)malloc(numOfSems * sizeof(unsigned short));
    initialBuffer = init.array;
    for (int i = 0; i < numOfSems; ++i)
        init.array[i] = 0;
    semctl(sem_id, 0, SETALL, init);

    return sem_id;
}

void releaseSemaphores(int sem_id)
{
    semctl(sem_id, 0, IPC_RMID, 0);
    if (initialBuffer)
    {
        free(initialBuffer);
        initialBuffer = 0;
    }
}

int getSemaphores(key_t key)
{
    int sem_id = semget(key, 0, 0);
    if (sem_id == -1)
    {
        fprintf(stderr, "Error: Cannot access semaphore\n");
        perror(strerror(errno));
        handleExit();
    }
    return sem_id;
}

int *getShmPtr(int shm_id)
{
    int *data = (int *)shmat(shm_id, NULL, 0);
    if (data == (int *)(-1))
    {
        fprintf(stderr, "Error: Cannot access shared memory!\n");
        perror(strerror(errno));
        handleExit();
    }
    return data;
}

void semaphoreAdd(int sem_id, int n, int val)
{
    struct sembuf options[1];
    options[0].sem_num = n;
    options[0].sem_op = val;
    options[0].sem_flg = 0;
    semop(sem_id, options, 1);
}

int isSemaphoreNull(int sem_id, int n)
{
    struct sembuf options[1];
    options[0].sem_num = n;
    options[0].sem_op = 0;
    options[0].sem_flg = IPC_NOWAIT;
    if (semop(sem_id, options, 1) < 0)
    {
        if (errno == EAGAIN)
            return 0;
    }
    return 1;
}

void awaitSemaphoreNull(int sem_id, int n)
{
    struct sembuf options[1];
    options[0].sem_num = n;
    options[0].sem_op = 0;
    options[0].sem_flg = 0;
    semop(sem_id, options, 1);
}
