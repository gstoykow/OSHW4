#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>    
#include <sys/sem.h>    
#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>

#define P_KEY 2137894
#define bool char
#define S1 0 
#define S2 1 
#define S3 2

struct SHMemory 
{
    char acc;
    int16_t req;
    uint32_t serve;
};

struct sembuf wait = { 0, -1, 0};
struct sembuf signal = { 0, +1, 0}; 
