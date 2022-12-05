#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "VIP" : "Normal")

int currentCap;

pthread_cond_t Free;
pthread_mutex_t m_mutex;

struct client
{
	int id;
	bool isvip;
};


void enter_normal_client(int id)
{
	pthread_mutex_lock(&m_mutex);
	printf("Normal client with id: %d entering club\n", id);
	if (currentCap == CAPACITY)
	{
		pthread_cond_wait(&Free, &m_mutex);
	}
	currentCap++;
	pthread_mutex_unlock(&m_mutex);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&m_mutex);
	printf("Vip client with id: %d entering club\n", id);
	if (currentCap == CAPACITY)
	{
		pthread_cond_wait(&Free, &m_mutex);
	}
	currentCap++;
	pthread_mutex_unlock(&m_mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, bool isvip)
{
	printf("%s client with id %d exiting\n", VIPSTR(isvip), id);
	pthread_cond_broadcast(&Free);
}

void *client(void *arg)
{
	struct client* cli = (struct client*) arg;

	bool isvip = cli->isvip;
	int id = cli->id;

	if (isvip)
	{
		enter_vip_client(id);
	}
	else
	{
		enter_normal_client(id);
	}
	dance(id, isvip);
	disco_exit(id, isvip);
	return NULL;
}

void loadClients(char* path, struct client** clients, int* client_count)
{
	int entry_count = 0;
	char buffer[3];//maximum clients to load
	FILE* fd = fopen(path, "r");
	if (fd == NULL)
	{
		printf("Could not open file\n");
		exit(1);
	}
	
	fgets(buffer, 3, fd);
	fgetc(fd);
	entry_count = atoi(buffer);
	*clients = malloc(sizeof(struct client) * entry_count);
	char viped[3];
	int i = 0;
	while (fgets(viped, 3, fd) != NULL)
	{
		if (viped[0] != '\n')
		{
			(*clients)[i].id = i;
			(*clients)[i].isvip = ((viped[0] == '1') ? true : false);
			i++;
		}	
	}
	*client_count = entry_count;
}

int main(int argc, char *argv[])
{
	/*Init mutex*/
	pthread_mutex_init(&m_mutex, NULL);

	/*Checking args*/
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s path\n", argv[0]);
		exit(1);
	}
	
	struct client* clients;
	int client_count;

	loadClients(argv[1], &clients, &client_count);

	bool mvips = true;//var indicating if there are more vips to be processed
	//Lauch threads
	
	pthread_t* threads = malloc(sizeof(pthread_t) * client_count);
	
	for (int i = 0; i < client_count; i++)
	{
		if (clients[i].isvip == true && mvips)
		{
			pthread_create(&threads[i], NULL, client, &clients[i]);
		}
		else if(!mvips && !clients[i].isvip)
		{
			pthread_create(&threads[i], NULL, client, &clients[i]);
		}
		if (i == client_count - 1 && mvips)
		{
			mvips = false;
			i = -1;
		}
	}
	for (int i = 0; i < client_count; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}
