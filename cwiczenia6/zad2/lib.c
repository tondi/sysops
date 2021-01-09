#include <sys/mman.h>
#include <unistd.h>
#include "lib.h"
#include <dispatch/dispatch.h>
dispatch_semaphore_t semaphore;

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

struct rk_sema {
#ifdef __APPLE__
    dispatch_semaphore_t    sem;
#else
    sem_t                   sem;
#endif
};


static inline void
rk_sema_init(struct rk_sema *s, uint32_t value)
{
#ifdef __APPLE__
    dispatch_semaphore_t *sem = &s->sem;

    *sem = dispatch_semaphore_create(value);
#else
    sem_init(&s->sem, 0, value);
#endif
}

static inline void
rk_sema_wait(struct rk_sema *s)
{

#ifdef __APPLE__
    dispatch_semaphore_wait(s->sem, DISPATCH_TIME_FOREVER);
#else
    int r;

    do {
            r = sem_wait(&s->sem);
    } while (r == -1 && errno == EINTR);
#endif
}

static inline void
rk_sema_post(struct rk_sema *s)
{

#ifdef __APPLE__
    dispatch_semaphore_signal(s->sem);
#else
    sem_post(&s->sem);
#endif
}

void* CreateSharedMem(const char* name, off_t size){
    int fd;
    void* ret;
    if((fd = shm_open(name, O_CREAT | O_RDWR, 0666)) == -1){
        fprintf(stderr, "Error: Cannot create shared memory!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    if(ftruncate(fd, size) == -1){
        fprintf(stderr, "Error: Cannot clean shared memory!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    if((ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        fprintf(stderr, "Error: Cannot map memory to process!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return ret;
}

void* GetSharedMem(const char* name, off_t size){
    int fd;
    void* ret;
    if((fd = shm_open(name, O_RDWR, 0666)) == -1){
        fprintf(stderr, "Error: Cannot load shared memory!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    if((ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        fprintf(stderr, "Error: Cannot map memory to process!\n");
        perror(strerror(errno));
        handleExit(0);
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

sem_t* CreateSemaphore(const char* name, unsigned int initVal){
    sem_t* s = sem_open(name, O_CREAT | O_RDWR, 0666, initVal);
    if(s == SEM_FAILED){
        fprintf(stderr, "Error: Cannot create semaphore!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return s;
}

sem_t* GetSemaphore(const char* name){
    sem_t* s = sem_open(name, O_RDWR);
    if(s == SEM_FAILED){
        fprintf(stderr, "Error: Cannot load semaphore!\n");
        perror(strerror(errno));
        handleExit(0);
    }
    return s;
}

void Take(sem_t* s){
    if(sem_wait(s) == -1){
        fprintf(stderr, "Error: Cannot load semaphore!\n");
        perror(strerror(errno));
        handleExit(0);
    }
}

void Release(sem_t* s){
    if(sem_post(s) == -1){
        fprintf(stderr, "Error: Cannot release semaphore!\n");
        perror(strerror(errno));
        handleExit(0);
    }
}

void CloseSemaphore(sem_t* s){
    if(sem_close(s))
        perror(strerror(errno));
}

void RemoveSemaphore(const char* name){
    if(sem_unlink(name) == -1){
        fprintf(stderr, "Error: Cannot remove semaphore!.\n");
        perror(strerror(errno));
    }
}

int GetValue(sem_t* sem){
    int ret;
    // if(sem_getvalue(sem, &ret) == -1){
    //     fprintf(stderr, "Error: Cannot get semaphore value!\n");
    //     perror(strerror(errno));
    //     handleExit(0);
    // }
    // return ret;
    return 0;
}




































