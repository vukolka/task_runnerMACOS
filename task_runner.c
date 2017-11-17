#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int		files_changed()
{
	FILE *size_file;
	FILE *size_current;
	char *current_size = (char*)malloc(50);
	char *previous_size = (char*)malloc(50);
	
	size_current = popen("find SRCs -type f -print0 | xargs -0 stat -f%z | awk '{b+=$1} END {print b}'", "r");
	size_file = fopen("size_file.txt", "r+");
	fseek(size_file, 0, SEEK_SET);
	fseek(size_current, 0, SEEK_SET);
	fscanf(size_file, "%s", previous_size);
	fscanf(size_current, "%s", current_size);
	fseek(size_file, 0, SEEK_SET);
	fputs(current_size, size_file);
	pclose(size_current);
	fclose(size_file);
		 // printf("pre size%s\n", previous_size);
		 // printf("curr size%s\n", current_size);
	if (strcmp(previous_size, current_size) != 0)
	{
		printf("diff%d\n",strcmp(previous_size, current_size));
		free(current_size);
		free(previous_size);
		return 1;
	}
	free(current_size);
	free(previous_size);
	return (0);
}

int		main(int argc, char const *argv[])
{
	printf("\n\nWatching...\n");
	while(1)
	{
		if (files_changed())
		{
			printf("the directory has been changed, compiling\n\n");
			int status = system("make test");
			printf("\n\nWatching...\n");
		}
		sleep(1);
	}
	return 0;
}