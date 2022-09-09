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

typedef struct shmCDT{
    char mode;
    int fd;
    size_t writeIdx, readIdx;
    sem_t sem;
    void * current;
    void * ptr; //first position in shared memory
    char name[NAME_SIZE];
}shmCDT;

shmADT newShm(char* shmName, char mode){
    shmADT newMaster = calloc(1,sizeof(shmCDT));
    if (newMaster==NULL){
        perror("newMaster");
        exit(EXIT_FAILURE);
    }
    newMaster->mode = mode;
    strncpy(newMaster->name,shmName,NAME_SIZE);
    return newMaster;
}

void openShm(shmADT shm){
    //creamos el shared memory object vacío
    shm->fd = shm_open(shm->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shm->fd == -1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    //le asignamos tamaño de memoria
    //if(shm->mode=='w'){
        if(ftruncate(shm->fd, SHM_SIZE) != 0){
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
    //}

    /*if(sem_init(&(shm->sem), SHARED, INITIAL_SEM) == -1){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }*/
}

//when mode is set to 'w', shared memory is mapped in write mode, and read mode when 'r'
void mapShm(shmADT shm){
    int permission = shm->mode=='w'?PROT_WRITE:shm->mode=='r'?PROT_READ:PROT_NONE;
    void *res = mmap(NULL, SHM_SIZE, permission, MAP_SHARED,shm->fd, 0);
    if(res == MAP_FAILED){
        perror("mmap");
    }
    shm->ptr = res;
    shm->current = res;
}

void writeToShm(shmADT shm, const char* input){
    int lenght = strlen(input);
    if(shm->current==NULL){
        perror("mapShm should be called first");
        exit(EXIT_FAILURE);
    } 
    int *firstPos = (int *)shm->current;
    *firstPos = lenght;
    shm->current+=sizeof(int);
    sprintf(shm->current, "%s", input);
    shm->current+=lenght;
    return;
}

size_t readFromShm(shmADT shm, char* output){
    size_t lenght = *((int*)shm->current);
    printf("%d\n", lenght);
    shm->current+=sizeof(int);

    if(snprintf(output, lenght, "%s", (char *)shm->current)<0){
        perror("sprintf");
        exit(EXIT_FAILURE);
    }

    printf("%s\n",output);

    shm->current+=lenght;
    return lenght;
}

void freeResources(shmADT shm){
    if(munmap(shm->ptr, SHM_SIZE)==-1){
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    if(shm_unlink(shm->name)==-1){
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    if(close(shm->fd)==-1){
        perror("close");
        exit(EXIT_FAILURE);
    }
    free(shm);
    return;
}

