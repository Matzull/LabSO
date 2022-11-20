#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	int status;
	if (pid == 0)
	{
		printf("Child process with id: %d and parent id: %d, %d\n", getpid(), getppid(), pid);
	}
	else
	{
		printf("Parent process with id: %d and parent id: %d, %d\n" , getpid(), getpid(), pid);
	}
	
	exit(0);
}