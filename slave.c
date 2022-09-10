// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include <stdio.h>

#define HASH_LEN 33
#define MAX_LEN 256

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


void printHashWithFormat(const char * hash, const char * filename);

void computeHash(char * buffer, const char * filename){

    if (sprintf(buffer,"md5sum %s",filename)<0){
        handle_error("sprintf");
    }

    FILE * file;
    if ((file = popen(buffer,"r")) == NULL){
        handle_error("popen");
    }
    
    fread(buffer, sizeof(char),MAX_LEN,file);

    buffer[HASH_LEN-1]=0;
    
    if (pclose(file) == -1){
        handle_error("pclose");
    }

}

int main(int argc, char * argv[]){

    char hash[MAX_LEN];
    char * filename = NULL; //Después hay que hacer un free de esto
    size_t lenght = 0;
    ssize_t charsRead;
    while((charsRead = getline(&filename, &lenght, stdin)) > 0){
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
        handle_error("sprintf");
    }
    write(STDOUT_FILENO,resultToPrint,resultLen);
}