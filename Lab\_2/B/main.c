#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define FIFONAME "temp.fifo"

int main () {
    char buff[100];
    memset(buff, '\0', 100);

    unlink(FIFONAME);//delete if exists
    //create fifo
    if((mkfifo(FIFONAME, S_IRWXU)) < 0)
    {
	perror("Creating FIFO error\n");
	return -1;
    }
	
    pid_t pid = fork();//create child process
    if (pid == (pid_t) 0)
    {
        int rfd = open(FIFONAME, O_RDONLY);//open reading from fifo
	if(rfd < 0)
        {
		perror("Read FIFO error\n");
		return -2;
	}
		
        read(rfd, buff, 100);
	close(rfd);
        time_t childTime = time(NULL);
        printf("Child time = %s%s\n", ctime(&childTime), buff);
    }
    else if(pid < (pid_t) 0)
    {
	perror("Fork failed\n");
	return -4;
    }
    else
    {
        int wfd = open(FIFONAME, O_WRONLY);//open writing to fifo
	if(wfd < 0)
        {
		perror("Write FIFO error\n");
		return -3;
	}
		
	time_t parentTime = time(NULL);
        sleep(5);
	sprintf(buff, "Parent time = %s and pid =  %d", ctime(&parentTime), getpid());
        write(wfd, buff, 100);
        close(wfd);
    }

    return 0;
}
