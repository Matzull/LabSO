#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFF_SIZE 32

void openFile(int* file, char* path, int flag)
{
    if ((*file=open(path, flag)) < 0) 
    {
        fprintf(stderr, "The file %s could not be opened.\n", path);
        exit(EXIT_FAILURE);
    }
}

void cat(int src, int startingByte)
{
    int read_b;
    char* buffer = malloc(BUFF_SIZE);

    lseek(src, startingByte, SEEK_SET);    
    
    while ((read_b = read(src, buffer, BUFF_SIZE)) > 0) 
    { 
        for (int i = 0; i < read_b; i++)
        {
            printf("%c", buffer[i]);
        }
    }
    free(buffer);
    prinf("\n");
    return;   
}

int main(int argc, char *argv[])
{
    int opt;
    int NBytes = 0;
    bool starting = true;
    /*Simple arg check*/
    if (argc < 2 )
    {
        fprintf(stderr, "Usage is: %s File [-n Bytes| -e]\n", argv[0]);
        exit(1);
    }
    
    /*Parse the options*/
    while((opt = getopt(argc, argv, "n:e")) != -1) {
		switch(opt) {
        case 'n':
            NBytes = atoi(optarg);
        
        case 'e':
            starting = false;
        }
    }

    /*Open the File*/
    int src;
    openFile(&src, argv[1], O_RDONLY);

    /*Print the contents to stdout*/
    if (starting)
    {
        cat(src, NBytes);
    }
    else
    {
        /* Get File size */
        struct stat st;
        stat(argv[1], &st);
        long long int size = st.st_size;

        cat(src, size - NBytes);
    }    
    close(src);
	return 0;
}