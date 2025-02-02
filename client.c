#include "comm.h"

int get_sems(key_t ShmKEY)
{
     int SemID = semget(ShmKEY, 3, 0666);

     if( SemID == -1 )
          err(1,"Error on sem buff init!");

     return SemID;
}

int get_shm(key_t ShmKEY)
{
     int ShmID = shmget(ShmKEY, sizeof(struct SHMemory), 0666);
     if ( ShmID == -1 )
          err(1,"Error on shm require!"); 
          
     printf("Client has received shared memory...\n");
     return ShmID;
}

void clr_shm(struct SHMemory * ShmPTR)
{
     if(shmdt((void *) ShmPTR) == -1)
          err(1,"Error detatching shared mem!");
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

int  main(int argc, char* argv[])
{
	 if( argc != 2 ) 
         err(1,"Bad arguments!(run like ./client [A-H])");

     key_t          ShmKEY;
     int            ShmID;
     struct SHMemory  *ShmPTR;

     int            SemID;

     ShmKEY = ftok(".", P_KEY);

     ShmID = get_shm(ShmKEY);
     SemID = get_sems(ShmKEY);

     printf("%d %d\n", SemID, ShmID );

     ShmPTR = (struct SHMemory *) shmat(ShmID, NULL, 0);

     //s1 wait
     sem_decr(SemID,S1);

     ShmPTR->acc = argv[1][0];
   	 //printf("%c", ShmPTR->acc);
     
     //s2 signal
     sem_incr(SemID,S2);

     //s3 wait
     sem_decr(SemID,S3);

     printf("%d\n", ShmPTR->req );

     scanf("%" SCNd16,&(ShmPTR->req));

     //s2 signal
     sem_incr(SemID,S2);

     //s3 wait
     sem_decr(SemID,S3);

     printf("%d\n", ShmPTR->req );

     //s1 signal
     sem_incr(SemID,S1);

     clr_shm(ShmPTR);
 
     exit(0);
}
