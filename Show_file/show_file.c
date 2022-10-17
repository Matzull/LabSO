#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file = NULL;
	int ret;
	size_t block_size = 1;
	char buffer[1];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}
	else if(argc == 3)
	{
		block_size = atoi(argv[2]);
	}

	/* Open file */
	if ((file = fopen(argv[1], "rb")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	/* Read file byte by byte */
	while (fread(buffer, sizeof(char), block_size, file) > 0) {
		/* Print byte to stdout */
		ret = fwrite(buffer, sizeof(char), block_size, stdout);
		if (ret == EOF) {
			fclose(file);
			err(3, "putc() failed!!");
		}
	}

	fclose(file);
	return 0;
}

