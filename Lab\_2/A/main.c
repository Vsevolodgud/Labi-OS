#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>

int main()
{	//create pipe
	int mypipe[2];
	if(pipe(mypipe) < 0){
		perror("Pipe failed\n");
		return -1;
	}
	
	//create child process
	pid_t pid = fork();
	if(pid == (pid_t) 0)
    	{	
		//child process
		close(mypipe[1]);
		long int fromParent[2] = {0};
		read(mypipe[0], fromParent, sizeof(long int) * 2);
		time_t tm = time(0);
		printf("I'm child my time = %s", ctime(&tm));
		tm = fromParent[0];
		printf("Parent time = %s and pid = %ld\n", ctime(&tm), fromParent[1]);
		close(mypipe[0]);
	}
	else if(pid < (pid_t) 0)
	{
		perror("Fork failed\n");
		return -1;
	}
    	else
    	{	
		//parent process
		close(mypipe[0]);
		long int toChildren[2];
		toChildren[0] = time(0);//parent time
		toChildren[1] = getpid();//parent pid
		sleep(5);
		write(mypipe[1], toChildren, sizeof(long int) * 2);
		close(mypipe[1]);
	}

	return 0;
}
