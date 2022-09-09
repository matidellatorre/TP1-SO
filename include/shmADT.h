#ifndef shmADT_h

#define shmADT_h

typedef struct shmCDT * shmADT;

shmADT newShm(char* shmName);
void openShm(shmADT shm);
void mapShm(shmADT shm, char mode);
void writeToShm(shmADT shm, const char* input);
size_t readFromShm(shmADT shm, char* output);
void freeResources(shmADT shm);

#endif