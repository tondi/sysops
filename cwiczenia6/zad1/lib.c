#include "lib.h"

unsigned short* initArrayBuf = NULL;

key_t CreateKey(char* name, int num){
    key_t k = ftok("newkey.txt", 4);
    if(k == -1){
        fprintf(stderr, "Nie udalo sie stworzyc klucza!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return k;
}

int CreateSemaphores(key_t k, int numOfSems){
    int semid = semget(k, numOfSems, 0666 | IPC_CREAT);
    if(semid == -1){
        fprintf(stderr, "Nie udalo sie stworzyc semafor!\n");
        perror(strerror(errno));
        onExit(0);
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
        fprintf(stderr, "Nie uzyskano dostepu do semafor\n");
        perror(strerror(errno));
        onExit(0);
    }
    return semid;
}

int CreateSharedMem(key_t k, int size){
    int shmid = shmget(k, size, 0666 | IPC_CREAT);
    if(shmid == -1){
        fprintf(stderr, "Nie udalo sie stworzyc pamieci wspoldzielonej!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return shmid;
}


int GetSharedMem(key_t key){
    int shmid = shmget(key, 0, 0);
    if (shmid == -1) {
        fprintf(stderr, "Nie uzyskano dostepu do pamieci wspoldzielonej\n");
        perror(strerror(errno));
        onExit(0);
    }
    return shmid;
}

//void ReleaseSharedMem(

int* GetMemPointer(int shmid){
    int* data = (int*)shmat(shmid, NULL, 0);
    if(data == (int*)(-1)) {
        fprintf(stderr, "Nie uzyskano dostepu do pamieci wspoldzielonej!\n");
        perror(strerror(errno));
        onExit(0);
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














