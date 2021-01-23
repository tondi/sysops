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

#define BARBER_QUEUE_ID 0
#define CLIENT_QUEUE_ID 1

extern unsigned short* initialBuffer;
extern void handleExit();
int createSemaphores(key_t k, int numOfSems);
key_t createKey(char* name);
void releaseSemaphores(int sem_id);
int initShm(key_t k, int size);
int getShm(key_t key);
void awaitSemaphoreNull(int sem_id, int n);
int getSemaphores(key_t key);
int* getShmPtr(int shm_id);
int isSemaphoreNull(int sem_id, int n);
void semaphoreAdd(int sem_id, int n, int val);

#endif
