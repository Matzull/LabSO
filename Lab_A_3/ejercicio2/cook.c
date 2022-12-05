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
#include <sys/stat.h>

#define M 10
#define SEM_NAME "E2SEM"
#define SMOBJ_NAME "/sMemoryObj"

int finish = 0;
int* available_servings;
sem_t *sem_id;

void putServingsInPot(int servings)
{
}

void cook(void)
{
	while(!finish) {
		putServingsInPot(M);
	}
}

void handler(int signo)
{
	finish = 1;
}

int main(int argc, char *argv[])
{
	//semaphore creation
	sem_id = sem_open(SEM_NAME, O_CREAT, 0600, 0);

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
	printf("%p", available_servings);
	if (available_servings == MAP_FAILED)
	{
		perror("Error, failed mapping memory to virtual memory");
		exit(1);
	}
	
	
	printf("Working\n");
	int num = 5000;
	memcpy(available_servings, &num, sizeof(int));
	printf("%d", *available_servings);
	/*Unmapping shared memory*/
	munmap(available_servings, sizeof(int));
	//shm_unlink(SMOBJ_NAME);
	close(fd);
	sem_unlink(SEM_NAME);
	
	return 0;
}
