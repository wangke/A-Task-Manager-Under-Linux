#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/types.h>

int USER,NICE,SYSTEM,IDLE,IOWAIT,IRQ,SOFTIRQ;
int STEALSTOLEN,GUEST;//主要存在虚拟机时，可省
int main()
{

while(1){

		int fd;
		int user, nice, system, idle, iowait, irq, softirq;
		double cpurate;
		fd = open("/proc/stat", O_RDONLY);
		if(fd == 0)
		{
			printf("open /proc/stat failed\n");
			return -1;
		}
		char buffer[50*1024];
		ssize_t rd = read(fd, buffer, sizeof(buffer));
		if(rd == 0 || rd == sizeof(buffer))
		{
			printf("read /proc/stat failed\n");
			return -1;
		}
		close(fd);
		buffer[rd] = '\0';
		char *find = strstr(buffer, "cpu");
		if(find == NULL)
		{
			printf("find error\n");
			return -1;
		}
		sscanf(find, "cpu %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);
		cpurate = 100*(user-USER+nice-NICE+system-SYSTEM+iowait-IOWAIT+irq-IRQ+softirq-SOFTIRQ)/(float)(user-USER+nice-NICE+system-SYSTEM+iowait-IOWAIT+idle-IDLE+irq-IRQ+softirq-SOFTIRQ);
		USER = user;
		NICE = nice;
		SYSTEM = system;
		IDLE = idle;
		IOWAIT = iowait;
		IRQ = irq;
		SOFTIRQ = softirq;
	
	
		
		char cpuinfo[15] = "cpu ： ";
		char output[5];
		sprintf(output, "%.3f", cpurate);
		strcat(cpuinfo, output);
		strcat(cpuinfo, "%");
		printf("%s\n", cpuinfo);
	sleep(3);
	}
return 0;
}
