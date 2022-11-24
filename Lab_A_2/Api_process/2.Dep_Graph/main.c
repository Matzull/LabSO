#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8
/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */

int main(int argc, char **argv)
{
    pid_t proc[9];//8 processes + master
    pid_t cpid;
    int status;

    //Master process --P0
    proc[0] = getpid();
    printf("P0- Master Process with id: %d\n", proc[0]);

    //Slave process --P1
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P1");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P1 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P1\n");
        exit(EXIT_SUCCESS);
    }
    proc[1] = cpid;
    waitpid(cpid, &status, WUNTRACED);
    
    //Slave process --P2
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P2");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P2 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P2\n");
        exit(EXIT_SUCCESS);
    }
    proc[2] = cpid;

    //Slave process --P5
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P5");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P5 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P5\n");
        exit(EXIT_SUCCESS);
    }
    proc[5] = cpid;

    //Slave process --P7
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P7");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P7 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P7\n");
        exit(EXIT_SUCCESS);
    }
    proc[7] = cpid;

    //P3, P4 wait for P2
    waitpid(proc[2], &status, WUNTRACED);

    //Slave process --P3
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P3");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P3 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P3\n");
        exit(EXIT_SUCCESS);
    }
    proc[3] = cpid;

    //Slave process --P4
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P4");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P4 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P4\n");
        exit(EXIT_SUCCESS);
    }
    proc[4] = cpid;
    
    //P6 waits for P5
    waitpid(proc[5], &status, WUNTRACED);

    //Slave process --P6
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P6");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P6 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P6\n");
        exit(EXIT_SUCCESS);
    }
    proc[6] = cpid;

    //P8 waits for P3, P6, P7
    waitpid(proc[3], &status, WUNTRACED);
    waitpid(proc[6], &status, WUNTRACED);
    waitpid(proc[7], &status, WUNTRACED);

    //Slave process --P8
    cpid = fork();
    if (cpid < 0)
    {
        printf("Couldnt create thread P8");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("Started P8 with id: %d\n", getpid());
        sleep(1);
        printf("Stoping P8\n");
        exit(EXIT_SUCCESS);
    }
    proc[8] = cpid;

    waitpid(proc[8], &status, WUNTRACED);
    
    printf("Stopping Master Process");
	return 0;
}
