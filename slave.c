#include <stdio.h>

char * computeHash(const char * filename){

    char buffer[256];

    sprintf(buffer,"md5sum %s",filename);

    FILE * file = popen(buffer,"r");

    int readBytes = fread(buffer, sizeof(char),256,file);

    buffer[readBytes]=0;

    //e9aea3571d18e099b6295afd19487489
    
    //printf("%s",buffer);
    pclose(file);

    return buffer;
}

int main(int argc, char * argv[]){

}