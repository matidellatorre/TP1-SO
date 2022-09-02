#include <stdio.h>
#include "masterADT.h"

int main(int argc, char * argv[]){

    if (argc<2){
        printf("Error en los parametros");
    }

    masterADT masterOfPuppets = newMaster(argc-1,argv[1]);

    


}