#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>

#define M 10
#define SEM_NAME "E2SEM"
#define SMOBJ_NAME "/sMemoryObj"


bool finish = false;
int* available_servings;
sem_t *sem_id;

void putServingsInPot(int servings)
{
	printf("Refilling pot with %d servings...\n", M);
	*available_servings += servings;
}

void cook(void)
{
	while(!finish){
		sem_wait(sem_id);
		if (*available_servings == 0)
		{
			putServingsInPot(M);
		}
		sem_post(sem_id);
	}
}

void handler(int signo)
{
	finish = true;
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	//semaphore creation
	sem_id = sem_open(SEM_NAME, O_CREAT, 0700, 1);
	if (sem_id == SEM_FAILED){
        perror("Parent  : [sem_open] Failed\n");
	}

	int fd = 0;//File descriptor for the shared memory obj
	if((fd = shm_open(SMOBJ_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
	{
		perror("Error, couldnt create memory object\n Exiting...");
		exit(1);
	}

	/*Resizing shared memory obj*/
	ftruncate(fd, sizeof(int));

	/*Remapping shared memory to process virtual memory*/
	available_servings = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (available_servings == MAP_FAILED)
	{
		perror("Error, failed mapping memory to virtual memory");
		exit(1);
	}
	
	//cooking...
	cook();

	/*Unmapping shared memory*/
	munmap(available_servings, sizeof(int));
	shm_unlink(SMOBJ_NAME);
	close(fd);
	sem_unlink(SEM_NAME);
	
	return 0;
}
