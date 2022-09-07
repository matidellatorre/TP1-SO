#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define HASH_LEN 33
#define MAX_LEN 256


void printHashWithFormat(const char * hash, const char * filename);

void computeHash(char * buffer, const char * filename){

    if (sprintf(buffer,"md5sum %s",filename)<0){
        perror("sprintf");
    }

    FILE * file;
    if ((file = popen(buffer,"r")) == NULL){
        perror("popen");
        // Hay que hacer algo aca, a decidir
    }
    
    fread(buffer, sizeof(char),MAX_LEN,file);

    buffer[HASH_LEN-1]=0;

    //e9aea3571d18e099b6295afd19487489
    
    if (pclose(file) == -1){
        perror("pclose");
        //Posible exit, a decidir. Capaz no vale pena seguir procesando archivos.
    }

}

int main(int argc, char * argv[]){

    char hash[MAX_LEN];
    char * filename = NULL; //Después hay que hacer un free de esto
    size_t lenght = 0;
    ssize_t charsRead;
    while((charsRead = getline(&filename, &lenght, STDIN_FILENO)) > 0){
        filename[charsRead-1]=0;
        computeHash(hash,filename);
        printHashWithFormat(hash,filename);
    }
    free(filename);
    return 0;
}

void printHashWithFormat(const char * hash, const char * filename){
    pid_t pid = getpid();
    char resultToPrint[MAX_LEN];
    int resultLen;
    resultLen = sprintf(resultToPrint,"File: %s Hash: %s Slave PID: %d\n",filename,hash,pid);
    if (resultLen<0){
        perror("sprintf");
    }
    write(STDOUT_FILENO,resultToPrint,resultLen);
}