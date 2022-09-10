// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "include/shmADT.h"

#define MAX_NAME_LEN 50
#define MAX_LEN 256
#define READ_MODE 'r'

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char * argv[]){

    char shmName[MAX_NAME_LEN+1];

    switch(argc){
        case 1:{ //La info de la shm no se pasó por argumento
            //Buscamos la información por el pipe contra master
            int count=read(STDIN_FILENO, shmName, MAX_NAME_LEN);
            if(count == -1){
                handle_error("newMaster");
            }
            shmName[count]='\0';
            break;
        }
        case 2:{ //La info se pasó por argumento
            strncpy(shmName, argv[1], MAX_NAME_LEN);
            break;
        }
        default:{
            printf("Cantidad incorrecta de argumentos");
            exit(EXIT_FAILURE);
        }
    }
    

    shmADT sharedMemory = newShm(shmName, READ_MODE);
    openShm(sharedMemory);
    mapShm(sharedMemory);
    int filecount = readQtyShm(sharedMemory);
    
    char buf[MAX_LEN];
    while(filecount>0){
        memset(buf, 0, MAX_LEN); //Clear the array before reading
        readFromShm(sharedMemory, buf);
        printf("%s\n", buf);
        filecount--;
    }
    closeShm(sharedMemory);
    freeShm(sharedMemory);
    return 0;

}
