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
    //Shared memory
    
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
    pid_t forkRes;
 
    while(slaveCount<MAX_SLAVES && slaveCount<master->filecount){
        pipe(master->sendPipes[slaveCount]);
        pipe(master->receivePipes[slaveCount]);

        if ((forkRes=fork())<0){
            //Error
        } else if(forkRes==0){
            //Hijo
            for (int i = 0; i < slaveCount; i++) {
              close(master->sendPipes[slaveCount][1]);
              close(master->receivePipes[slaveCount][0]);
            }

            close(master->sendPipes[slaveCount][1]);
            close(master->receivePipes[slaveCount][0]);
          
            dup2(master->receivePipes[slaveCount][1], STDOUT_FILENO);
            dup2(master->sendPipes[slaveCount][0],STDIN_FILENO);

            
      
        } else {
            //Padre
            master->slavePids[slaveCount]=forkRes;
        }


        
    }

}
