#ifndef shmADT_h

#define shmADT_h

typedef struct shmCDT * shmADT;

shmADT newMaster(int filecount, const char**filenames);

#endif