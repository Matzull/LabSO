#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
    struct stat sb;
    lstat(fname ,&sb);
    if((sb.st_mode & __S_IFMT) == __S_IFDIR)
    {   
        size_t dir_size = 0;
        get_size_dir(fname, &dir_size);
        printf("d %s: %zu kilobytes, %zu blocks.\n", fname, (dir_size * 512)/1024, dir_size);
        *blocks += dir_size;
    }
    else
    {
        printf("f %s: %zu kilobytes, %zu blocks.\n", fname, (sb.st_blocks * 512)/1024, sb.st_blocks);
        *blocks += sb.st_blocks;
    }
    return 0;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dname);
    char path[256];
    while ((de = readdir(dr)) != NULL)
    {
        strcpy(path, dname);
        strcat(path, "/");
        strcat(path, de->d_name);
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
        {
            get_size(path, blocks);
        }  
    }          
  
    closedir(dr);
    return 0;    
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
    size_t blocks = 0;

    for (size_t i = 1; i < argc; i++)
    {
        get_size(argv[i], &blocks);        
    }
    
    printf("Total file size is %zu Kilobytes, %zu blocks.\n", (blocks *  512) / 1024, blocks);

	return 0;
}
