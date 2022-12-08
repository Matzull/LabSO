#include "includes.h"

#define NUMITER 7
#define SEMPROC "SEMPROC"

sem_t *sem_items;
sem_t *sem_gaps;
sem_t *sem_producers;

int* available_servings;

int getServingsFromPot(void)
{
	printf("Getting seving from pot\n");
	(*available_servings)--;
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 2);
}

void savages(void)
{
	int i;
	for (i = 0; i < NUMITER; i++)
	{
		sem_wait(sem_items);
		sem_wait(sem_producers);
		if ((*available_servings) != 0)
		{
			getServingsFromPot();
			sem_post(sem_items);
		}
		else
		{
			sem_post(sem_gaps);
		}
		sem_post(sem_producers);
		eat();
	}
}

int main(int argc, char *argv[])
{

	//semaphore creation
	sem_items = sem_open(SEMITEM, O_RDWR, 0700, 0);
	sem_gaps = sem_open(SEMGAP, O_RDWR, 0700, M);
	sem_producers = sem_open(SEMPROC, O_CREAT, 0700, 1);

	int fd = 0;//File descriptor for the shared memory obj
	if((fd = shm_open(SMOBJ_NAME, O_RDWR, 0666)) == -1)
	{
		perror("Error, couldnt find memory object, please run cook first\n Exiting...\n");
		exit(1);
	}
	/*Remapping shared memory to process virtual memory*/
	available_servings = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if (available_servings == MAP_FAILED)
	{
		perror("Error, failed mapping memory to virtual memory");
		exit(1);
	}
	
	savages();

	/*Unmapping shared memory*/
	munmap(available_servings, sizeof(int));
	close(fd);
	// sem_unlink(SEMGAP);
	// sem_unlink(SEMITEM);
	sem_unlink(SEMPROC);
	
	return 0;
}
