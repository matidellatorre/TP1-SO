#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "include/shmADT.h"

#define MAX_LEN 50

int main(int argc, char * argv[]){

    char shmName[MAX_LEN];
    switch(argc){
        case 1:{ //La info de la shm no se pasó por argumento
            //Buscamos la información por el pipe contra master
            int count=read(STDIN_FILENO, shmName, MAX_LEN);
            if(count == -1){
                perror("newMaster");
                exit(EXIT_FAILURE);
            }
            shmName[count]='\0';
            break;
        }
        case 2:{ //La info se pasó por argumento
            strncpy(shmName, argv[1], MAX_LEN);
            break;
        }
        default:{
            printf("Cantidad incorrecta de argumentos");
            exit(EXIT_FAILURE);
        }
    }
    

    shmADT sharedMemory = newShm(shmName, 'r');
    openShm(sharedMemory);
    mapShm(sharedMemory);
    int filecount = readQtyShm(sharedMemory);
    
    char buf[256];
    while(filecount>0){
        memset(buf, 0, 256); //Clear the array before reading
        readFromShm(sharedMemory, buf);
        printf("%s\n", buf);
        filecount--;
    }
    //freeResources(sharedMemory);
    closeShm(sharedMemory);
    freeShm(sharedMemory);
    return 0;

}
