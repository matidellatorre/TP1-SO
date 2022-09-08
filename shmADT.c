#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include "include/shmADT.h"

#define SHM_NAME "/myshm"
#define SHM_SIZE 1024
#define SHARED 0
#define INITIAL_SEM 1

typedef struct shmCDT{
    int fd;
    ssize_t writeIdx, readIdx;
    sem_t sem;
}shmCDT;

shmADT newShm(void){
    shmADT newMaster = calloc(1,sizeof(shmADT));
    if (newMaster==NULL){
        //Handeleo de error de malloc
        //TODO: @Gaston Alasia
    }
    return newMaster;
}

void createShm(shmADT shm){
    //creamos el shared memory object vacío
    shm->fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shm->fd == -1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    //le asignamos tamaño de memoria
    if(ftruncate(shm->fd, SHM_SIZE) != 0){
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    if(sem_init(&(shm->sem), SHARED, INITIAL_SEM) == -1){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}

//when mode is set to 1, shared memory is mapped in write mode, and read mode when 0
void *mapShm(shmADT shm, int mode){
    int permission = mode?PROT_WRITE:PROT_READ;
    void *res = mmap(NULL, SHM_SIZE, permission, MAP_SHARED,shm->fd, 0);
    if(res == MAP_FAILED){
        perror("mmap");
    }
    return res;
}

void writeToShm(shmADT shm, const char* input){
    int lenght = strlen(input);
    ssize_t count = write(shm->fd, input, lenght);
    if(count < lenght){     //En caso de error write devuelve -1, si no se llega a escribir todo el input quiere decir que también se produjo un error
        perror("writeToShm");
        exit(EXIT_FAILURE);
    }
    shm->writeIdx += count;
}

void readFromShm(shmADT shm, char**output){
    
    getline();
}