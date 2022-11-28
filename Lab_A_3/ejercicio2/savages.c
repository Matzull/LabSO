#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUMITER 3

int getServingsFromPot(void)
{

}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(void)
{

}

int main(int argc, char *argv[])
{

	return 0;
}
