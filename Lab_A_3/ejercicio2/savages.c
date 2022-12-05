#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define NUMITER 3
#define SEM_NAME "E2SEM"
#define SMOBJ_NAME "/sMemoryObj"

sem_t *sem_id;
int* available_servings;

int getServingsFromPot(void)
{
	sem_wait(sem_id);
	*available_servings--;
	sem_post(sem_id);
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(void)
{
	for (size_t i = 0; i < NUMITER; i++)
	{
		getServingsFromPot();
		eat();
	}
	
}

int main(int argc, char *argv[])
{

	//semaphore creation
	sem_id = sem_open(SEM_NAME, O_CREAT, 0600, 0);

	int fd = 0;//File descriptor for the shared memory obj
	if( fd = shm_open(SMOBJ_NAME, O_RDWR, 0666) == -1)
	{
		perror("Error, couldnt find memory object, please run cook first\n Exiting...");
		exit(1);
	}
	/*Remapping shared memory to process virtual memory*/
	available_servings = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANON, fd, 0);
	
	if (available_servings == MAP_FAILED)
	{
		perror("Error, failed mapping memory to virtual memory");
		exit(1);
	}
	
	
	printf("Working\n");
	int num = 5000;
	memcpy(available_servings, &num, sizeof(int));
	printf("%d", *available_servings);



	savages();
	
	return 0;
}
