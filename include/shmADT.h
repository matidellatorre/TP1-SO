#ifndef shmADT_h

#define shmADT_h

typedef struct shmCDT * shmADT;

shmADT newShm(void);
void createShm(shmADT shm);
void *mapShm(shmADT shm, int mode);

#endif