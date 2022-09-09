
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "./include/masterADT.h"
#include "./include/shmADT.h"

#define SHM_NAME "/myshm"
#define MAX_SLAVES 5
#define ANS_PATH "./result.txt"
#define MAX_LEN 256
#define MAX_FILES_PER_SLAVE 2

typedef struct masterCDT {
    int filecount;
    const char ** filenames;
    int activeSlaves;
    int sendPipes[MAX_SLAVES][2];
    int receivePipes[MAX_SLAVES][2];
    int currentTask;
    pid_t slavePids[MAX_SLAVES];
    shmADT sharedMemory;
    
} masterCDT;

void manageResult(masterADT master, int *taskFinished, FILE * resultFile, fd_set readFds);

masterADT newMaster(int filecount, const char**filenames){
    masterADT newMaster = calloc(1,sizeof(masterCDT));
    if (newMaster==NULL){
        perror("calloc");
    }
    newMaster->filecount = filecount;
    newMaster->filenames = filenames;
    newMaster->sharedMemory = newShm(SHM_NAME);
    openShm(newMaster->sharedMemory);
    mapShm(newMaster->sharedMemory, 'w');
    return newMaster;
}

void initializeSlaves(masterADT master){

    int slaveCount = 0;
    pid_t forkRes;
 
    while(slaveCount<MAX_SLAVES && slaveCount<master->filecount){
        pipe(master->sendPipes[slaveCount]);
        pipe(master->receivePipes[slaveCount]);

        if ((forkRes=fork())<0){
            perror("fork");
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

            close(master->receivePipes[slaveCount][1]);
            close(master->sendPipes[slaveCount][0]);
            

            char * args[] = {"slave",NULL};
            execv(args[0],args);

            perror("execv");            
      
        } else {
            master->slavePids[slaveCount]=forkRes;
            close(master->sendPipes[slaveCount][0]);
            close(master->receivePipes[slaveCount][1]);
            slaveCount++;
        }
        
    }

  master->activeSlaves = slaveCount;
}

void giveTask(int endPipe,const char * file){

  write(endPipe, file, strlen(file));
  write(endPipe, "\n", 1);

}

void sendInitialTask(masterADT master){
  int taskNum = 0;

  for(int i = 0; i < master->activeSlaves;i++){
    for (int j = 0; j < MAX_FILES_PER_SLAVE && taskNum < master->filecount; j++) {
      
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
            manageResult(master, &taskFinished, resultsFile, readFds);
        }
    }
    if(fclose(resultsFile) == EOF){
        perror("fclose");
    }
}

void manageResult(masterADT master, int *taskFinished, FILE * resultFile, fd_set readFds){
    for(int i=0; i<master->activeSlaves; i++){
        if(FD_ISSET(master->receivePipes[i][0], &readFds)){
            (*taskFinished)++;
            char buff[MAX_LEN];
            int bytesRead = read(master->receivePipes[i][0], buff, MAX_LEN);
            if(bytesRead == -1){
                perror("read");
            }
            buff[bytesRead]='\0';

            if(fwrite(buff, sizeof(char),bytesRead, resultFile) == 0){
                perror("fwrite");
            }

            //Escribo lo mismo en shared memory
            writeToShm(master->sharedMemory,buff);

        }
        //Se puede mover, esta parte de la funcion asigna la nueva tarea
        if(master->currentTask < master->filecount){
            giveTask(master->sendPipes[i][1], master->filenames[(master->currentTask)++]);
        }
    }
}

void closePipes(masterADT master){
    for(int i=0; i<master->activeSlaves; i++){
        close(master->sendPipes[i][1]);
        close(master->receivePipes[i][0]);
    }
}

void freeMaster(masterADT master){
    free(master);
}

void freeAllResources(masterADT master){
    closePipes(master);
    freeMaster(master);
}

