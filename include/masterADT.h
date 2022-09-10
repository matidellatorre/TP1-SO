#ifndef masterADT_h

#define masterADT_h


typedef struct masterCDT * masterADT;

masterADT newMaster(int filecount, const char**filenames);

void initializeSlaves(masterADT master);

void giveTask(int endPipe,const char * file);

void sendInitialTask(masterADT master);

void monitorSlaves(masterADT master);

void freeAllResources(masterADT master);

void freeMaster(masterADT);

#endif
