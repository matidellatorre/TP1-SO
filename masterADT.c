#include <stdio.h>
#include <unistd.h>
#include "masterADT.h"

#define MAX_SLAVES 5

typedef struct masterCDT {
    int filecount;
    char ** filenames;
    int activeSlaves;
    int sendPipes[MAX_SLAVES][2];
    int receivePipes[MAX_SLAVES][2];
    int resultFd;
    pid_t slavePids[MAX_SLAVES];
    //falta shared memory
} masterCDT;

masterADT newMaster(int filecount, const char**filenames){
    masterADT newMaster = calloc(1,sizeof(masterCDT));
    if (newMaster==NULL){
        //Handeleo de error de malloc
        //TO DO: #Gaston Alasia
    }
    newMaster->filecount = filecount;
    newMaster->filenames = filenames;
    return newMaster;
}

void initializeSlaves(masterADT master){

    int slaveCount = 0;
    while(slaveCount<MAX_SLAVES && slaveCount<master->filecount){
        pipe(master->sendPipes[slaveCount]);
        pipe(master->receivePipes[slaveCount]);
        pid_t forkRes;
        if ((forkRes=fork())<0){
            //Error
        } else if(forkRes==0){
            //Hijo 
        } else {
            //Padre
            master->slavePids[slaveCount]=forkRes;
        }


        
    }

}