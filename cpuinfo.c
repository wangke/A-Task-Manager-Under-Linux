#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define size 7
int main()
{
	char* apart = " " ;
	char buffer[128];
	char* cpuinfo[size];
	int fd = open("/proc/version", O_RDONLY);
	read(fd, buffer, sizeof(buffer));
	int i;
	cpuinfo[0] = strtok(buffer, apart);
	for(i = 1; i < size; i++)
	{
		cpuinfo[i] = strtok(NULL, apart);
	}
	for(i = 0; i < size; i++)
	{
		printf(cpuinfo[i]);
		printf("\n");
	}
	
} 
