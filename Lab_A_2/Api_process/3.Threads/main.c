#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define USER_N 10
struct s_user
{
    int user_id;
    char priority;
};

struct s_targ
{
    int thread_n;
    struct s_user* arg;
};


void *thread_usuario(void *arg)
{
    struct s_user th_user;
    struct s_targ* targ = (struct s_targ*) arg;
    th_user.priority = (targ->arg[targ->thread_n]).priority;
    th_user.user_id = (targ->arg[targ->thread_n]).user_id;
    //free(arg);
    printf("Thread id: %lu; User: %d; Priority: %s\n", pthread_self(), th_user.user_id, (th_user.priority == 'y') ? "Yes" : "No");
    pthread_exit(0);
}

void initUsers(struct s_user** users, int count)
{
    *users = malloc(sizeof(struct s_user) * count);
    for (size_t i = 0; i < count; i++)
    {
        (*users)[i].user_id = i;
        if (i%2==0)
        {
            (*users)[i].priority = 'y';
        }
        else
        {
            (*users)[i].priority = 'n';
        }   
    }
}

int main(int argc, char* argv[])
{
    struct s_user* users;
    pthread_t thread_id[USER_N];
    initUsers(&users, USER_N);
    struct s_targ args[USER_N]; 
    for (int i = 0; i < USER_N; i++)
    {
        args[i].arg = users;
        args[i].thread_n = i;
        //printf("Starting thread %d\n", i);
        pthread_create(&thread_id[i], NULL, thread_usuario, &args[i]);
    }

    for (size_t i = 0; i < USER_N; i++)
    {
        pthread_join(thread_id[i], NULL);
    }

	return 0;
}