// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "./include/masterADT.h"

int main(int argc, const char * argv[]){

    if (argc<2){
        printf("Error en los parametros");
    }

    masterADT masterOfPuppets = newMaster(argc-1,argv+1);

    initializeSlaves(masterOfPuppets);

    sleep(2);

    sendInitialTask(masterOfPuppets);

    monitorSlaves(masterOfPuppets);
    
    freeAllResources(masterOfPuppets);

    freeMaster(masterOfPuppets);

    return 0;
    
}
