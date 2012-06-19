#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>




#include <fcntl.h>

int main()
{
	while(1){
	int fd, i, j, MemTotal, MemFree, Buffers, Cached;
	char temp1[75], temp2[75];
	fd = open("/proc/meminfo", O_RDONLY);
	if(fd == 0)
	{
		printf("open /proc/meminfo failed\n");
		return -1;
	}
	char buffer[300];
	ssize_t rd = read(fd, buffer, sizeof(buffer));
	if(rd != sizeof(buffer))
	{
		printf("read failed\n");
		return -1;
	}
	close(fd);
	
	strcat(temp1, strtok(buffer, "\n"));
	j = 0;
	for(i = 0; i < 75; i++)
	{
		
		if(temp1[i] >= '0' && temp1[i] <= '9')
			{
				if(j != 0 && !(temp1[i-1] >= '0' && temp1[i-1] <= '9'))
					break;
					temp2[j++] = temp1[i];
			}
			
			
	}
		
	temp2[j] = '\0';
	MemTotal = atoi(temp2);
	memset(temp1, '\0', 75);
	memset(temp2, '\0', 75);
	
	strcat(temp1, strtok(NULL, "\n"));
	j = 0;
	for(i = 0; i < 75 ; i++)
	{
		if(temp1[i] >= '0' && temp1[i] <= '9')
			{
				if(j != 0 && !(temp1[i-1] >= '0' && temp1[i-1] <= '9'))
					break;
					temp2[j++] = temp1[i];
			}
	}
	temp2[j] = '\0';
	MemFree = atoi(temp2);
	memset(temp1, '\0', 75);
	memset(temp2, '\0', 75);
	
	strcat(temp1, strtok(NULL, "\n"));
	j = 0;
	for(i = 0; i < 75 ; i++)
	{
		if(temp1[i] >= '0' && temp1[i] <= '9')
			{
				if(j != 0 && !(temp1[i-1] >= '0' && temp1[i-1] <= '9'))
					break;
					temp2[j++] = temp1[i];
			}
	}
		
	temp2[j] = '\0';
	Buffers = atoi(temp2);
	memset(temp1, '\0', 75);
	memset(temp2, '\0', 75);
	
	strcat(temp1, strtok(NULL, "\n"));
	j = 0;
	for(i = 0; i < 75 ; i++)
	{
		if(temp1[i] >= '0' && temp1[i] <= '9')
			{
				if(j != 0 && !(temp1[i-1] >= '0' && temp1[i-1] <= '9'))
					break;
					temp2[j++] = temp1[i];
			}
	}
		
			
	temp2[j] = '\0';
	Cached = atoi(temp2);
	memset(temp1, '\0', 75);
	memset(temp2, '\0', 75);
	
	printf("%d %d %d %d\n", MemTotal, MemFree, Buffers, Cached);
	sleep(5);
	}
}


