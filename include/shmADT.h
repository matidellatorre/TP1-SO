#ifndef shmADT_h

#define shmADT_h

typedef struct shmCDT * shmADT;

shmADT newShm(char* shmName, char mode);
void openShm(shmADT shm);
void mapShm(shmADT shm);
void writeQtyShm(shmADT shm, int qty);
void writeToShm(shmADT shm, const char* input);
int readQtyShm(shmADT shm);
size_t readFromShm(shmADT shm, char* output);
void freeResources(shmADT shm);

#endif