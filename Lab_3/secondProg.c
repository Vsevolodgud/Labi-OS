#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#define SHM "sharedMem"

//exit handler
void Handler(int sig)
{
    printf("\nSecondProgram exit\n");
    exit(0);
}

int main()
{
    signal(SIGINT, Handler);

    char* address;
    int shmid;
    key_t key = ftok(SHM, 'a');//transform filename to shared memory key
    
    //create shared memory identifier
    if( (shmid = (shmget(key, 32, IPC_EXCL)) ) == -1 )
    {
        perror("Error with shmget func\n");
        return -1;
    }

    //connect shmid to the address space of the process
    if((address = shmat(shmid, NULL, 0 )) == (char*) -1)
    {
	      perror("SecondProgram: error with shmat func\n");
        return -2;
    }

    while(1)
    {   
        time_t currTime = time(NULL);//second program time
        printf("Second program time = %sand pid = %d\n", ctime(&currTime), getpid());
	      printf("%s", address);//first process data string
	      printf("////////////////////////////////////\n");
        sleep(3);
    }

    return 0;
}
