#ifndef shmADT_h

#define shmADT_h

typedef struct shmCDT * shmADT;

shmADT newShm(char* shmName, char mode);
void openShm(shmADT shm);
void mapShm(shmADT shm);
void writeQtyShm(shmADT shm, int qty);
void writeToShm(shmADT shm, const char* input);
int readQtyShm(shmADT shm);
int readFromShm(shmADT shm, char* output);
void freeResources(shmADT shm);
void closeShm(shmADT);
void freeShm(shmADT);
void destroyShm(shmADT);
void writeToPipe(shmADT shm, const char* buf);
void readFromPipe(shmADT shm, char*);

#endif
