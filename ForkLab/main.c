#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


pid_t Pid;//child process pid

void ChildExit(void)
{
	printf("CHILD: Exit\n");
}

void SigTermHandler(int sig)
{
    if(Pid == 0)
    {
        printf("\nSigterm %d received from child process\n", sig);
    }
}

void SigIntHandler(int sig)
{
    if(Pid == 0)
    {
        printf("\nSigint %d received from child process\n", sig);
        kill(getpid(), SIGTERM);//going to run SigTermHandler
    }
}

int main(int argc, char** argv)
{
		int exitCode;
		int exitError;
		
		struct sigaction newAction;//setting sigterm handler by sigaction
		newAction.sa_handler = SigTermHandler;
		sigemptyset(&newAction.sa_mask);
		newAction.sa_flags = 0;
		sigaction(SIGTERM, &newAction, NULL);
		
		signal(SIGINT, SigIntHandler);//setting sigint handler by signal
		
		switch(Pid = fork())
		{
				case -1:
					perror("Fork error");
					exit(1);
				case 0:
					printf("CHILD: My PID is %d\n", getpid());
					printf("CHILD: My parent's PID is %d\n", getppid());
					printf(">>> You can stop child process"
					        " immediatly by using Ctrl+C (or wait 15 sec)\n");
					sleep(15);
					
					printf("CHILD: Enter child's exit code: ");
					scanf("%d", &exitCode);
					exitError = atexit(ChildExit);
					
					if(exitError != 0)
					{
						printf("Child process exit error\n");
					}
					
					exit(exitCode);
				default:
					printf("PARENT: My PID is %d\n", getpid());
					printf("PARENT: My child's PID is %d\n", Pid);
					printf("PARENT: Waiting...\n");
					wait(&exitCode);
					
					printf("PARENT: Real child's exit code is %d\n", WEXITSTATUS(exitCode));
					printf("PARENT: Exit\n");
		}
		
		return 0;
}
