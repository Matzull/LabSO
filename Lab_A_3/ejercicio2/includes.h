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
#define SEMITEM "SEMITEM"
#define SEMGAP "SEMGAP"
#define SMOBJ_NAME "/sMemoryObj"