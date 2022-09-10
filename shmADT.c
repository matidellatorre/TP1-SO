// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include "./include/shmADT.h"

#define SHM_SIZE 1024
#define SHARED 0
#define INITIAL_SEM 1
#define NAME_SIZE 25
#define SEM_NAME "/mysem"
#define READ_MODE 'r'
#define WRITE_MODE 'w'

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct shmCDT{
    char mode;
    int fd;
    sem_t * sem;
    void * current;
    void * ptr; //first position in shared memory
    char name[NAME_SIZE];
}shmCDT;

shmADT newShm(char* shmName, char mode){
    shmADT newMaster = calloc(1,sizeof(shmCDT));
    if (newMaster==NULL){
        handle_error("newMaster");
    }
    newMaster->mode = mode;
    strncpy(newMaster->name,shmName,NAME_SIZE);
    //abrimos el semáforo
    newMaster->sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR|S_IWUSR, 0);
    if(newMaster->sem == SEM_FAILED){
        handle_error("sem_open");
    }
    return newMaster;
}

void openShm(shmADT shm){
    //creamos el shared memory object vacío
    shm->fd = shm_open(shm->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shm->fd == -1){
        handle_error("shm_open");
    }

    if(ftruncate(shm->fd, SHM_SIZE) != 0){
        handle_error("ftruncate");
    }
}

//when mode is set to 'w', shared memory is mapped in write mode, and read mode when 'r'
void mapShm(shmADT shm){
    int permission = shm->mode==WRITE_MODE?PROT_WRITE:shm->mode==READ_MODE?PROT_READ:PROT_NONE;
    void *res = mmap(NULL, SHM_SIZE, permission, MAP_SHARED,shm->fd, 0);
    if(res == MAP_FAILED){
        perror("mmap");
    }
    shm->ptr = res;
    shm->current = res;
}

//Funcion para escribir la cantidad de archivos a procesar en la primera posicion
void writeQtyShm(shmADT shm, int qty){
    int *firstPos = (int *)shm->current;
    *firstPos = qty;
    shm->current+=sizeof(int);
    if(sem_post(shm->sem)==-1){
        handle_error("sem_post");
    }
}

//Escribe el output de un slave a la vez
void writeToShm(shmADT shm, const char* input){
    int lenght = strlen(input);
    if(shm->current==NULL){
        handle_error("mapShm should be called first");
    }
    int *firstPos = (int *)shm->current;
    *firstPos = lenght;
    shm->current+=sizeof(int);
    sprintf(shm->current, "%s", input);
    shm->current+=lenght;
    if(sem_post(shm->sem)==-1){
        handle_error("sem_post");
    }
    return;
}

int readQtyShm(shmADT shm){
    if(sem_wait(shm->sem) == -1){
        handle_error("sem_wait");
    }
    int *firstPos = (int *)shm->current;
    int qty = *firstPos;
    shm->current+=sizeof(int);
    return qty;
}

//Lee el output de un slave a la vez (escrito por master en la SHM)
int readFromShm(shmADT shm, char* output){
    if(sem_wait(shm->sem)==-1){
        handle_error("sem_wait");
    }
    size_t lenght = *((int*)shm->current);
    shm->current+=sizeof(int);

    if(snprintf(output, lenght, "%s", (char *)shm->current)<0){
        handle_error("sprintf");
    }

    shm->current+=lenght;
    return lenght;
}

void closeShm(shmADT shm){

  if(sem_close(shm->sem) == -1){
    handle_error("semclose");
  }
  if(munmap(shm->ptr,SHM_SIZE) == -1){
    handle_error("munmap");
  }
  if(close(shm->fd)==-1){
    handle_error("close fd shm");
  }

}

void destroyShm(shmADT shm){

  if(sem_unlink(SEM_NAME)==-1){
    handle_error("sem_unlink");
  }
  if(shm_unlink(shm->name)==-1){
    handle_error("shm_unlink");
  }

}

void freeShm(shmADT shm){
  free(shm);
}

