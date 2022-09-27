// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>
#include "./include/shmADT.h"

#define SHM_SIZE 1024
#define SHARED 0
#define INITIAL_SEM 1
#define NAME_SIZE 25
#define SEM_NAME "/mysem"
#define READ_MODE 'r'
#define WRITE_MODE 'w'

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct shmCDT
{
    FILE * namedpipe;
    char mode;
    int fd;
    sem_t *sem;
    void *current;
    void *ptr; // first position in shared memory
    char name[NAME_SIZE];
} shmCDT;

shmADT newShm(char *shmName, char mode)
{
    shmADT newMaster = calloc(1, sizeof(shmCDT));
    if (newMaster == NULL)
    {
        handle_error("newMaster");
    }
    newMaster->mode = mode;
    strncpy(newMaster->name, shmName, NAME_SIZE);
    // Create/Open semaphore instance
    newMaster->sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (newMaster->sem == SEM_FAILED)
    {
        handle_error("sem_open");
    }
    if (newMaster->mode == WRITE_MODE)
    {
        if (mkfifo("./tmp/fifo", 0666) == -1)
        {
            handle_error("fifo");
        }
    }
    return newMaster;
}

void openShm(shmADT shm)
{
    // Create/Open an empty shared memory object
    shm->fd = shm_open(shm->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm->fd == -1)
    {
        handle_error("shm_open");
    }

    // Asing space to the shared memory object
    if (ftruncate(shm->fd, SHM_SIZE) != 0)
    {
        handle_error("ftruncate");
    }

    if (shm->mode == WRITE_MODE)
    {
        if ((shm->namedpipe = fopen("./tmp/fifo", "w+")) == NULL)
        {
            handle_error("open1");
        }
    }
    else
    {
        if ((shm->namedpipe = fopen("./tmp/fifo", "r")) == NULL)
        {
            handle_error("open2");
        }
    }
}

// when mode is set to 'w', shared memory is mapped in write mode, and read mode when 'r'
void mapShm(shmADT shm)
{
    int permission = shm->mode == WRITE_MODE ? PROT_WRITE : shm->mode == READ_MODE ? PROT_READ
                                                                                   : PROT_NONE;
    void *res = mmap(NULL, SHM_SIZE, permission, MAP_SHARED, shm->fd, 0);
    if (res == MAP_FAILED)
    {
        perror("mmap");
    }
    shm->ptr = res;
    shm->current = res;
}

// Set the number of lines view must read
void writeQtyShm(shmADT shm, int qty)
{
    int *firstPos = (int *)shm->current;
    *firstPos = qty;
    shm->current += sizeof(int);
    if (sem_post(shm->sem) == -1)
    {
        handle_error("sem_post");
    }
}

// Write one slave output per call with its size
void writeToShm(shmADT shm, const char *input)
{
    int lenght = strlen(input);
    if (shm->current == NULL)
    {
        handle_error("mapShm should be called first");
    }
    int *firstPos = (int *)shm->current;
    *firstPos = lenght;
    shm->current += sizeof(int);
    sprintf(shm->current, "%s", input);
    shm->current += lenght;
    if (sem_post(shm->sem) == -1)
    {
        handle_error("sem_post");
    }
    return;
}

// Get int from the first shared memory position to know how many lines have to be read from master
int readQtyShm(shmADT shm)
{
    if (sem_wait(shm->sem) == -1)
    {
        handle_error("sem_wait");
    }
    int *firstPos = (int *)shm->current;
    int qty = *firstPos;
    shm->current += sizeof(int);
    return qty;
}

// Read master output from one slave per call
int readFromShm(shmADT shm, char *output)
{
    if (sem_wait(shm->sem) == -1)
    {
        handle_error("sem_wait");
    }
    size_t lenght = *((int *)shm->current);
    shm->current += sizeof(int);

    if (snprintf(output, lenght, "%s", (char *)shm->current) < 0)
    {
        handle_error("sprintf");
    }

    shm->current += lenght;
    return lenght;
}

void closeShm(shmADT shm)
{
    if (sem_close(shm->sem) == -1)
    {
        handle_error("semclose");
    }
    if (munmap(shm->ptr, SHM_SIZE) == -1)
    {
        handle_error("munmap");
    }
    if (close(shm->fd) == -1)
    {
        handle_error("close fd shm");
    }
    printf("Hola\n");
    fflush(shm->namedpipe);
    if(fclose(shm->namedpipe)==EOF){
        handle_error("close_pipe");
    }
}

// Unlink shared memory object and semaphore
void destroyShm(shmADT shm)
{
    if (sem_unlink(SEM_NAME) == -1)
    {
        handle_error("sem_unlink");
    }
    if (shm_unlink(shm->name) == -1)
    {
        handle_error("shm_unlink");
    }
}

// Function needed so master can free its shmADT
void freeShm(shmADT shm)
{
    free(shm);
}

void writeToPipe(shmADT shm, const char *buf)
{
    fwrite(buf, sizeof(char),strlen(buf), shm->namedpipe);
}

void readFromPipe(shmADT shm, char *buff)
{
    fread(buff, sizeof(char), 255, shm->namedpipe);
}
