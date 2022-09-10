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
    
}
