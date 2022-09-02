#ifndef masterADT_h

#define masterADT_h

typedef struct masterCDT * masterADT;

masterADT newMaster(int filecount, const char**filenames);

#endif
