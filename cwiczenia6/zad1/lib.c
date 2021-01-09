#include "lib.h"

unsigned short* initArrayBuf = NULL;


int CreateSharedMem(key_t k, int size){
    int shmid = shmget(k, size, 0666 | IPC_CREAT);
    if(shmid == -1){
        fprintf(stderr, "Error: Cannot create shared memory!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return shmid;
}


int GetSharedMem(key_t key){
    int shmid = shmget(key, 0, 0);
    if (shmid == -1) {
        fprintf(stderr, "Error: Cannot access shared memory\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return shmid;
}

key_t CreateKey(char* name, int num){
    key_t k = ftok("queuekey.txt", 4);
    if(k == -1){
        fprintf(stderr, "Error: Nie udalo sie stworzyc klucza!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return k;
}

int CreateSemaphores(key_t k, int numOfSems){
    int semid = semget(k, numOfSems, 0666 | IPC_CREAT);
    if(semid == -1){
        fprintf(stderr, "Error: Cannot create semaphore!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    union semun init;
    init.array = (unsigned short*) malloc(numOfSems*sizeof(unsigned short));
    initArrayBuf = init.array;
    for(int i = 0; i < numOfSems; ++i) init.array[i] = 0;
    semctl(semid, 0, SETALL, init);
    
    return semid;
}

void ReleaseSemaphores(int semid){
    semctl(semid, 0, IPC_RMID, 0);
    if(initArrayBuf){
        free(initArrayBuf);
        initArrayBuf = 0;
    }
}

int GetSemaphores(key_t key){
    int semid = semget(key, 0, 0);
    if (semid == -1) {
        fprintf(stderr, "Error: Cannot access semaphore\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return semid;
}

int* GetMemPointer(int shmid){
    int* data = (int*)shmat(shmid, NULL, 0);
    if(data == (int*)(-1)) {
        fprintf(stderr, "Error: Cannot access shared memory!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return data;
}

void Add(int semid, int n, int val){
    struct sembuf ops[1];
    ops[0].sem_num = n;
    ops[0].sem_op = val;
    ops[0].sem_flg = 0;
    semop(semid, ops, 1);
}

int IsZero(int semid, int n){
    struct sembuf ops[1];
    ops[0].sem_num = n;
    ops[0].sem_op = 0;
    ops[0].sem_flg = IPC_NOWAIT;
    if(semop(semid, ops, 1) < 0){
        if(errno == EAGAIN)return 0;
    }
    return 1;
}

void WaitTillZero(int semid, int n){
    struct sembuf ops[1];
    ops[0].sem_num = n;
    ops[0].sem_op = 0;
    ops[0].sem_flg = 0;
    semop(semid, ops, 1);
}














