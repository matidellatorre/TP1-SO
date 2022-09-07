#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include "include/masterADT.h"

#define MAX_SLAVES 5
#define ANS_PATH "./result.txt"

typedef struct masterCDT {
    int filecount;
    char ** filenames;
    int activeSlaves;
    int sendPipes[MAX_SLAVES][2];
    int receivePipes[MAX_SLAVES][2];
    int currentTask;
    pid_t slavePids[MAX_SLAVES];
    //Shared memory
    
} masterCDT;

masterADT newMaster(int filecount, const char**filenames){
    masterADT newMaster = calloc(1,sizeof(masterCDT));
    if (newMaster==NULL){
        perror("calloc");
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

            execv("slave.out",NULL);

            perror("execv");            
      
        } else {
            //Padre
            master->slavePids[slaveCount]=forkRes;
            close(master->sendPipes[slaveCount][0]);
            close(master->receivePipes[slaveCount][1]);
            slaveCount++;
        }
        
    }

  master->activeSlaves = slaveCount;
}

void giveTask(int endPipe,const char * file){

  write(endPipe, file, sizeof(file));
  write(endPipe, "\n", 1);

}

void sendInitialTask(masterADT master){

  int taskNum = 0;

  for(int i = 0; i < master->activeSlaves;i++){
    for (int j = 0; j < 1 && taskNum < master->filecount; j++) {
      
      giveTask(master->sendPipes[i][1], master->filenames[taskNum]);
      taskNum++;
    }

  }
  
  master->currentTask = taskNum;

}

void monitorSlaves(masterADT master){
    fd_set readFds;
    int taskFinished = 0;
    FILE * resultsFile = fopen(ANS_PATH,"w"); //Chusmear el w
    if (resultsFile==NULL){
        perror("fopen");
    }
    while(taskFinished<master->filecount){
        FD_ZERO(&readFds);
        for(int i=0; i<master->activeSlaves; i++){
            FD_SET(master->receivePipes[i][0],&readFds);
        }
        if(select(master->receivePipes[master->activeSlaves-1][0]+1,&readFds,NULL,NULL,NULL) == -1){
            perror("select");
        } else {
            //manageResult(master);
        }
    }
}


