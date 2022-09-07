#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "include/masterADT.h"

int main(int argc, char * argv[]){
    if (argc<2){
        printf("Error en los parametros");
    }

    sleep(2);

    masterADT master = newMaster(argc-1,argv[1]);

    initializeSlave(master);

    sendInitialTask(master);

    
}