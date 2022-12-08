#include "includes.h"


bool finish = false;
int* available_servings;
sem_t *sem_items;
sem_t *sem_gap;

void putServingsInPot(int servings)
{
	printf("Refilling pot with %d servings...\n", M);
	*available_servings += servings;
}

void cook(void)
{
	while(!finish){
		sem_wait(sem_gap);
		if ((*available_servings) == 0)
		{
			putServingsInPot(M);
		}
		sem_post(sem_items);
	}
}

void handler(int signo)
{
	finish = true;
	/*Unmapping shared memory*/
	munmap(available_servings, sizeof(int));
	shm_unlink(SMOBJ_NAME);
	sem_unlink(SEMITEM);
	sem_unlink(SEMGAP);
	exit(0);
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	//semaphore creation
	sem_items = sem_open(SEMITEM, O_CREAT, 0700, 0);
	sem_gap = sem_open(SEMGAP, O_CREAT, 0700, 1);


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

	close(fd);
	
	return 0;
}
