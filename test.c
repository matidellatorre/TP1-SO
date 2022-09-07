#include <stdio.h>
#include <sys/select.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char * argv[]){
    int pipe1[2];// int pipe2[2];
    if(pipe(pipe1)==-1){
        printf("Error creating the pipe 1\n");
        return 1;
    }

    //int nfds = pipe1[0]>pipe1[1]?pipe1[0]:pipe1[1];

    //fd_set readset;
    //fd_set writeset;
    //FD_ZERO(&readset);
    //FD_ZERO(&writeset);
    int i;
    for(i=1;i<=8;i++){
        pid_t pid = fork();
        if (pid==0){
            printf("Soy el hijo %d\n",i);
            close(pipe1[0]); //Pues no usa el extremo de lectura
            dup2(pipe1[1], STDOUT_FILENO);
            //FD_SET(pipe1[1], &writeset);
            //
            //sleep(3);
            printf("%d - El culo te llueve\n",i);
            return 0;
        }
    }
        printf("%s","Soy el padre\n");
        close(pipe1[1]); //Cierro el extremo de escritura al padre
        dup2(pipe1[0], STDIN_FILENO);
        sleep(3);
        char readBuffer[1000];
        read(pipe1[0],readBuffer,1000);
        printf("El padre recibio: \n%s", readBuffer);
        
        return 0;
    
}