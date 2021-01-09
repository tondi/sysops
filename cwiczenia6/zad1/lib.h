#ifndef _LIB_H_
#define _LIB_H_

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BARBER_ID (0)
#define QUEUE_ID (1)

// union semun {
//     int val;
//     struct semid_ds *buf;
//     unsigned short *array;
//     struct seminfo *__buf;
// };

extern unsigned short* initArrayBuf;
extern void handleExit(int);

key_t CreateKey(char* name, int num);
int CreateSemaphores(key_t k, int numOfSems);
void ReleaseSemaphores(int semid);
int GetSemaphores(key_t key);
int CreateSharedMem(key_t k, int size);
int GetSharedMem(key_t key);
int* GetMemPointer(int shmid);
void Add(int semid, int n, int val);
int IsZero(int semid, int n);
void WaitTillZero(int semid, int n);

#endif
