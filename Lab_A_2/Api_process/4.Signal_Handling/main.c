#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*In this exercise we are going to experiment with signals between processes. A process create a child, wait for a signal
from a timer and, upon receiving it, terminate the execution of the child.
As in exercise 1, the main program will receive as argument the executable that you want the child process to execute.
The parent process will create a child, which will change its executable with a call to execvp.
The parent will then set the SIGALRM signal handler to be a function that sends a SIGKILL signal to the child process
and will set an alarm to send the signal after 5 seconds.
Before terminating, the parent will wait for the child to terminate and will check the cause of the child termination
(normal termination or signal reception), printing a message on the screen.
The student should consult the manual pages of: sigaction, alarm, sigalarm, kill, wait.
To check the correct operation of our program we can use as argument an executable that finishes in less than 5 seconds
(like ls or echo) and one that does not finish until a signal arrives (like xeyes).
Once the program is running, modify the parent to ignore the SIGINT signal and check that it does so.*/


int main(int argc, char* argv[])
{
    //argc check
    if (argc != 2)
    {
        printf("Usage: %s program", argv[0]);
    }
    
    char* path = argv[1];

    pid_t pid;
    pid = fork();
    char* args;

    if (pid == 0)
    {
        printf("Started process with id: %d\n", getpid());
        execvp(path, args);
    }
    if(pid > 0)
    {
        /* code */
    }
    else
    {
        perror("Couldnt create thread.");
    }
    


    return 0;
}