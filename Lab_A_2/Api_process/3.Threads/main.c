#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define USER_N 10
struct s_user
{
    int user_id;
    char priority;
};


void *thread_usuario(void *arg)
{

}

void initUsers(struct s_user** users, int count)
{
    *users = malloc(sizeof(struct s_user) * count);
    for (size_t i = 0; i < count; i++)
    {
        users[i]->user_id = i;
        if (i%2==0)
        {
            users[i]->priority = 'y';
        }
        else
        {
            users[i]->priority = 'n';
        }   
    }
}

int main(int argc, char* argv[])
{
    struct s_user users;
    pthread_t thread_id[USER_N];
    initUsers(&users, USER_N);
    for (size_t i = 0; i < USER_N; i++)
    {
        pthread_create(thread_id[i]);
    }
    
    c
	return 0;
}