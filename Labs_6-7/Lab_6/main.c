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

pthread_mutex_t mutex;
pthread_cond_t cond;
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
		pthread_mutex_lock(&mutex);//mutex was locked by <reading thread threadNum> 
		pthread_cond_wait(&cond, &mutex);//used to block a condition variable
		printf("Read: This is thread %d, tid =  %lx, count  = %d\n", 
					*(int *)threadNum, (long)pthread_self(), count);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);//only <reading thread threadNum> can unlock mutex
		sleep(4);
	}
}

//used by writng thread
void* Write(void* arg) 
{
	while(1) 
	{
		sleep(4);
		pthread_mutex_lock(&mutex);//mutex was locked by writing thread
		count++;
		
		//counter
		if (count > 10000)
		{
		  count = 0;
		}
		printf("\nWrite: Now counter = %d\n", count);
		
		if(count % 10 == 0)
		{
			//unblock threads that were blocked by this condition variable
			pthread_cond_broadcast(&cond);
		}
		else
		{
			/*unblock at least one of threads 
			that were blocked by this condition variable*/
			pthread_cond_signal(&cond);
		}
		
		fflush(stdout);
		sleep(4);
		pthread_mutex_unlock(&mutex);//only writing thread can unlock mutex
	}
}

int main() 
{
	
	signal(SIGINT, Handler);

	pthread_t tid[NUM_READ_THREADS + 1];
	int threadNums[NUM_READ_THREADS];
	pthread_mutex_init(&mutex, NULL);//mutex initialization
	pthread_cond_init(&cond, NULL);//condition variable initialization

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
	
	pthread_cond_destroy(&cond);//destroy condition variable
	pthread_mutex_destroy(&mutex);//destroy mutex
	
	return 0;
}
