#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	int status;
	if (pid == 0)
	{
		printf("Child process with id: %d and parent id: %d\n", getpid(), getppid());
	}
	else
	{
		printf("Parent process with id: %d and parent id: %d\n" , getpid(), getpid());
	}
	fork();
	if (pid == 0)
	{
		printf("Child process with id: %d and parent id: %d\n", getpid(), getppid());
		execvp(argv[1], &argv[1]);
	}
	else
	{
		printf("Parent process with id: %d and parent id: %d\n" , getpid(), getpid());
		waitpid(pid, &status, 0);
	}
	exit(0);
}