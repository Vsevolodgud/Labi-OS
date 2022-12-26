#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define SHM "sharedMem"

int shmid;

//clean and exit
void Handler(int sig)
{
    printf("\nFirstProgram exit\n");

    struct shmid_ds* buf = 0;//clean
    shmctl(shmid, IPC_RMID, buf);//mark as deleted shared memory segment

    exit(0);
}

int main()
{
    signal(SIGINT, Handler);

    char* address;
    key_t key = ftok(SHM, 'a');//transform filename to shared memory key
    shmid = (shmget(key, 32, IPC_CREAT | 0666));//create shared memory identifier

    if(shmid == -1)
    {
        perror("Failed to create shared memory\n");
        return -1;
    }
    
    //connect shmid to the address space of calling process
    if((address = shmat(shmid, NULL, 0 )) == (char*) -1)
    {
        perror("FirstProgram: error with shmat function\n");
	      return -2;
    }


    if (strlen(address) != 0)
    {
        printf("Error! First process already exists\n");
        return -3;
    }
	
    time_t newTime = time(NULL);
    time_t oldTime = newTime;
    while(1)
    {
        newTime = time(NULL);
	      //avoid repeating the time
        if (newTime != oldTime)
        {
            oldTime = newTime;
            sprintf(address, "First program time = %sand pid = %d\n", ctime(&newTime), getpid());
        }
    }

    return 0;
}
