#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8
/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */

void printProcess(int p)
{
    printf("Process with name P%d\n", p);
    fflush(stdout);
}

int createChild(int *child_n)
{
    int pid = fork();
    printProcess(*child_n);
    child_n++;
    return pid;
}

int main(int argc, char **argv)
{
    int child_n = 2;
    int pid;
    printProcess(1);
    for (size_t i = 0; i < 3; i++)
    {
        pid = createChild(&child_n);
        if (i == 0 && pid != 0)
        {
            pid = createChild(&child_n);
            if (pid != 0)
            {
                createChild(&child_n);
            }            
        }  
    }
        
	return 0;
}
