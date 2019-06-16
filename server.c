#include "comm.h"

const uint32_t empt_arr[8] = {0}; 

int init_shm(key_t ShmKEY)
{
     int ShmID = shmget(ShmKEY, sizeof(struct SHMemory), IPC_CREAT | 0666);
     if ( ShmID == -1 )
          err(1,"Error on shm init!"); 
          
     printf("Server has received a shared memory...\n");
     return ShmID;
}

void clr_shm(struct SHMemory* ShmPTR, int ShmID)
{
     if( shmdt((void *) ShmPTR) == -1 )
          err(1,"Error on detatching shared memory");

     printf("Server has detached its shared memory...\n");

     if( shmctl(ShmID, IPC_RMID, NULL) == -1)
          err(1,"Error on removing shared memory");

     printf("Server has removed its shared memory...\n");
}

int init_sems(key_t ShmKEY)
{
     int SemID = semget(ShmKEY, 3, IPC_CREAT | 0666);

     if( SemID == -1 )
          err(1,"Error on sem buff init!");

     if ( semctl( SemID,S1,SETVAL, 1 ) == -1 )
          err(1,"Error on sem init!");

     if ( semctl( SemID,S2,SETVAL, 0 ) == -1 )
          err(1,"Error on sem init!");

     if ( semctl( SemID,S3,SETVAL, 0 ) == -1 )
          err(1,"Error on sem init!");

     return SemID;
}

void clr_sems(int SemID)
{
     if( semctl(SemID,0,IPC_RMID) == -1)
          err(1,"Error on sem removal!");
}

void sem_incr(int SemID, int sem)
{
     signal.sem_num = sem;
     semop(SemID, &signal, 1);
}

void sem_decr(int SemID, int sem)
{
     wait.sem_num = sem;
     semop(SemID, &wait, 1);
}

int file_init(const char* filename)
{
     int br_fd = open(filename, O_RDWR | O_CREAT, 666);

     if( br_fd == -1 )
          err(1,"Error on file open!");

     struct stat file_stat;
     if( stat(filename, &file_stat) == -1 )
          err(1,"Error on file stat creation!");

     if( file_stat.st_size == 0 )
     {
          if( write(br_fd, empt_arr, sizeof(empt_arr)) != sizeof(empt_arr) )
               err(1,"Error on write!");
     }

     return br_fd;
}

int main(int argc, char *argv[])
{
     key_t          ShmKEY;
     int            ShmID;
     struct SHMemory  *ShmPTR;

     int            SemID;

     if( argc != 2 ) 
          err(1,"Bad arguments!(run like ./server [FILENAME])");

     //file descriptor for bank records file
     int br_fd = file_init(argv[1]);

     //init shared memory
     ShmKEY = ftok(".", P_KEY);

     ShmID = init_shm(ShmKEY);
     SemID = init_sems(ShmKEY);

     ShmPTR = (struct SHMemory *) shmat(ShmID, NULL, 0);
     
	//must check

     ShmPTR->acc = 'a';

     printf("Server has attached the shared memory...\n");

     //ShmPTR->status  = NOT_READY;
     
     //fill shared mem

     // ShmPTR->status = FILLED;

     printf("Please start the client...\n");

     //while (1)
     //{
     //     sleep(1);
     //}
     printf("%d %d %d\n",SemID,ShmID,br_fd);

     sleep(75);

     clr_shm(ShmPTR,ShmID);
     exit(0);
}