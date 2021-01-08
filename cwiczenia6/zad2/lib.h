#ifndef _LIB_H_
#define _LIB_H_

#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>    

extern void onExit(int);

sem_t* CreateSemaphore(const char* name, unsigned int initVal);
sem_t* GetSemaphore(const char* name);
void Take(sem_t* sem);
void Release(sem_t* sem);
void CloseSemaphore(sem_t* sem);
void RemoveSemaphore(const char* name);
void* CreateSharedMem(const char* name, off_t len);
void* GetSharedMem(const char* name, off_t len);
void ReleaseSharedMem(void* addr, size_t len, const char* name);
void CloseSharedMem(void* addr, size_t len);
int GetValue(sem_t* sem);

#endif
