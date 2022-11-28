#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define M 10

int finish = 0;

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

	return 0;
}
