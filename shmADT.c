#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include "shmADT.h"

#define SHM_NAME "/myshm"
#define SHM_SIZE 1024

typedef struct shmCDT{
    int fd;
}shmCDT;

shmADT newMaster(int filecount, const char**filenames){
    shmADT newMaster = calloc(1,sizeof(shmADT));
    if (newMaster==NULL){
        //Handeleo de error de malloc
        //TO DO: #Gaston Alasia
    }
    return newMaster;
}

void createShm(shmADT shm){
    //creamos el shared memory object vacío
    shm->fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shm->fd == -1){
        perror("shm_open");
    }

    //le asignamos tamaño de memoria
    if(ftruncate(shm->fd, SHM_SIZE) != 0){
        perror("ftruncate");
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
