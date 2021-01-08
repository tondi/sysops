#include <sys/mman.h>
#include <unistd.h>

#include "lib.h"

sem_t* CreateSemaphore(const char* name, unsigned int initVal){
    sem_t* s = sem_open(name, O_CREAT | O_RDWR, 0666, initVal);
    if(s == SEM_FAILED){
        fprintf(stderr, "Nie udalo sie stworzyc semafory!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return s;
}

sem_t* GetSemaphore(const char* name){
    sem_t* s = sem_open(name, O_RDWR);
    if(s == SEM_FAILED){
        fprintf(stderr, "Nie udalo sie pobrac semafory!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return s;
}

void Take(sem_t* s){
    if(sem_wait(s) == -1){
        fprintf(stderr, "Nie udalo sie pobrac semafory!\n");
        perror(strerror(errno));
        onExit(0);
    }
}

void Release(sem_t* s){
    if(sem_post(s) == -1){
        fprintf(stderr, "Nie udalo sie porzucic semafory!\n");
        perror(strerror(errno));
        onExit(0);
    }
}

void CloseSemaphore(sem_t* s){
    if(sem_close(s))
        perror(strerror(errno));
}

void RemoveSemaphore(const char* name){
    if(sem_unlink(name) == -1){
        fprintf(stderr, "Nie udalo sie usunac semafory!.\n");
        perror(strerror(errno));
    }
}

void* CreateSharedMem(const char* name, off_t size){
    int fd;
    void* ret;
    if((fd = shm_open(name, O_CREAT | O_RDWR, 0666)) == -1){
        fprintf(stderr, "Nie udalo sie stworzyc pamieci wspoldzielonej!\n");
        perror(strerror(errno));
        onExit(0);
    }
    if(ftruncate(fd, size) == -1){
        fprintf(stderr, "Nie udalo sie wyczyscic pamieci wspoldzielonej!\n");
        perror(strerror(errno));
        onExit(0);
    }
    if((ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        fprintf(stderr, "Nie udalo sie zmapowac pamieci na proces!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return ret;
}

void* GetSharedMem(const char* name, off_t size){
    int fd;
    void* ret;
    if((fd = shm_open(name, O_RDWR, 0666)) == -1){
        fprintf(stderr, "Nie udalo sie pobrac pamieci wspoldzielonej!\n");
        perror(strerror(errno));
        onExit(0);
    }
    if((ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        fprintf(stderr, "Nie udalo sie zmapowac pamieci na proces!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return ret;
}

void ReleaseSharedMem(void* addr, size_t len, const char* name){
    munmap(addr, len);
    shm_unlink(name);
}

void CloseSharedMem(void* addr, size_t len){
    munmap(addr, len);
}

int GetValue(sem_t* sem){
    int ret;
    if(sem_getvalue(sem, &ret) == -1){
        fprintf(stderr, "Nie udalo sie pobrac wartosci semafory!\n");
        perror(strerror(errno));
        onExit(0);
    }
    return ret;
}




































