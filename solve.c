#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "./include/masterADT.h"

int main(int argc, const char * argv[]){
    if (argc<2){
        printf("Error en los parametros");
    }

    //sleep(2);

    //printf("filename %s", argv[1]);


    masterADT masterOfPuppets = newMaster(argc-1,argv+1);

    initializeSlaves(masterOfPuppets);

    sendInitialTask(masterOfPuppets);

    monitorSlaves(masterOfPuppets);

    freeAllResources(masterOfPuppets);

    
}