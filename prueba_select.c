#include <stdio.h>
/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[]){
    int pipe1[2];// int pipe2[2];
    if(pipe(pipe1)==-1){
        printf("Error creating the pipe 1\n");
        return 1;
    }
    int nfds = pipe1[0]>pipe1[1]?pipe1[0]:pipe1[1]; 
    /*if(!pipe(pipe2)){
        printf("Error creating the pipe 2\n");
        return 1;
    }*/
    
    fd_set readset;
    fd_set writeset;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    
    if(fork() == 0){
        //Sos el hijo pa
        printf("Soy el hijo\n");
        close(pipe1[0]); //Pues no usa el extremo de lectura
        dup2(pipe1[1], STDOUT_FILENO);
        FD_SET(pipe1[1], &writeset);
        //
        sleep(2);
        //printf("El culo te llueve\n");
        return 0;
    } else{
        printf("Soy el padre\n");
        close(pipe1[1]); //Cierro el extremo de escritura al padre
        dup2(pipe1[0], STDIN_FILENO);
        FD_SET(pipe1[0], &readset);
    }
    printf("ultima devolucion: %d\n", select(nfds+1, &readset, NULL, NULL, NULL));
    
    return 0;
}