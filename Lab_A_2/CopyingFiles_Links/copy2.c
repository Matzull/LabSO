#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define Buffer_s 2

void openFile(int* file, char* path, int flag)
{
    if ((*file=open(path, flag)) < 0) 
    {
        fprintf(stderr, "The file %s could not be opened.\n", path);
        exit(EXIT_FAILURE);
    }
}

void closeFile(int file)
{
    close(file);
}

void cpyFile(int src, int dst)
{
    int read_b;
    char* buffer = malloc(Buffer_s);
    while ((read_b = read(src, buffer, Buffer_s)) > 0) 
    { 
        if (write(dst, buffer, read_b) < read_b) 
        { 
            perror("Writing...\n");
            closeFile(src); 
            closeFile(dst);
            free(buffer); 
            exit(1);
        }
    }

    if (read_b < Buffer_s)
    {
        perror("Reading...\n");
        closeFile(src); 
        closeFile(dst); 
        free(buffer);
        exit(1);
    }
    free(buffer);
    return;    
}

int main(int argc, char* argv[])
{
    struct stat stats;
    /*Check #arguments*/
    if (argc != 3)
    {
        fprintf(stderr,"Usage: %s inputFile outputFile\n",argv[0]);
		exit(0);
    }

    int src = 0;
    int dst = 0;
    lstat(argv[1], &stats);

    /*Check if it is Symbolic link*/
    if ((stats.st_mode & __S_IFMT) != __S_IFLNK)//Not a symbolic link
    {
        openFile(&src, argv[1], O_RDONLY);
        openFile(&dst, argv[2], O_TRUNC|O_WRONLY|O_CREAT);

        cpyFile(src, dst);
    
        closeFile(src); 
        closeFile(dst);
    }
    else
    {
        size_t bytes = stats.st_size + 1;//For the null byte
        char* path = malloc(bytes);

        openFile(&src, argv[1], O_RDONLY);

        readlink(argv[1], path, bytes);
        path[bytes - 1] = 0;

        symlink(path, argv[2]);
    
        closeFile(src); 
        closeFile(dst);
        free(path);

    }
    
     

    return 0;
}