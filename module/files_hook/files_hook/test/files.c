#include <stdio.h>
#include <dirent.h>

int main() {
	DIR *d = opendir(".");
	struct dirent *dire;
	while ((dire = readdir(d)) != NULL)
	{
		printf("%s\n", dire->d_name);
	}
	closedir(d);
}
