#include <stdio.h>
#include "include/shmADT.h"

int main(int argc, char * argv[]){

    shmADT sharedMemory = newShm();
    createShm(sharedMemory);
    mapShm(sharedMemory, 0);

    readFromShm(sharedMemory,NULL);

    return 0;

}