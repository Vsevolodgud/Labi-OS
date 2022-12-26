#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#define NUM_READ_THREADS 10

pthread_rwlock_t locked;//read-write object
int count = 0;

void Handler(int sig) 
{
	printf("\nProgram exit\n");
	exit(0);
}

//func used by reading threads
void* Read(void* threadNum) 
{
	while(1) 
	{
		pthread_rwlock_rdlock(&locked);//lock <locked> to read
		printf("Read: This is thread %d, tid =  %lx, count  = %d\n", 
					*(int *)threadNum, (long)pthread_self(), count);
		fflush(stdout);
		pthread_rwlock_unlock(&locked);//unlock <locked> to read/write
		sleep(4);
	}
}

//used by writng thread
void* Write(void* arg) 
{
	while(1) 
	{
		sleep(4);
		pthread_rwlock_wrlock(&locked);//lock <locked> to write
		count++;
		
		//counter
		if (count > 10000)
		{
		  count = 0;
		}
		printf("\nWrite: Now counter = %d\n", count);
		fflush(stdout);
		sleep(4);
		pthread_rwlock_unlock(&locked);//unlock <locked> to read/write
	}
}

int main() 
{
	
	signal(SIGINT, Handler);

	pthread_t tid[NUM_READ_THREADS + 1];
	int threadNums[NUM_READ_THREADS];
	pthread_rwlock_init(&locked, NULL);//read-write lock object initialization

	for (int i = 0; i < NUM_READ_THREADS; i++) 
	{
		threadNums[i] = i;//number of the each thread
		pthread_create(&tid[i], NULL, Read, &threadNums[i]);//create reading thread
	}

	pthread_create(&tid[NUM_READ_THREADS], NULL, Write, NULL);

	for (int i = 0; i < NUM_READ_THREADS + 1; i++) 
	{
		pthread_join(tid[i], NULL);//waiting for terminated thread
	}
	pthread_rwlock_destroy(&locked);//destroy read-write lock object
	
	return 0;
}
