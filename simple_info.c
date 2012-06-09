#include <gtk/gtk.h>
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
#include <ctype.h>
#include <dirent.h>
#include "simple_info.h"


#define size 7
int seconds;
extern GtkWidget* time_label;
gint get_time()
{
	
	time_t t;
    struct tm *p;//储存时间的结构体
   	t = time(NULL);
    p = localtime(&t); //取得当地时间
    
    char buf[15]=" ";
	char temp[5];
    
    sprintf(temp,"%d",p->tm_hour);
	strcat(buf,temp);
   	sprintf(temp,"%d",p->tm_min);
    strcat(buf,":");
    strcat(buf,temp);
    sprintf(temp,"%d",p->tm_sec);
    strcat(buf,":");
   	strcat(buf,temp);
    gtk_label_set_text(GTK_LABEL(time_label), buf);
   	seconds++;
   	//if(seconds % 30 == 0)
   		//printf("%d\n", seconds);
    
    return 1;
}

void cpuinfo()
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
	char temp[10];
	for(i = 1; i <= strlen(cpuinfo[5]); i++)
		temp[i-1] = *(cpuinfo[5] + i);
	char linuxinfo[60];
	char gccinfo[60];
	char *empty = " ";
	for(i = 0; i < size; i++)
	{
		if(i < 3)
		{
			strcat(linuxinfo, cpuinfo[i]);
			strcat(linuxinfo, empty);
		}
		else if (i == 4)
		{
			strcat(gccinfo, temp);
			strcat(gccinfo, empty);
		}
		else if (i > 4)
		{
			strcat(gccinfo, cpuinfo[i]);
			strcat(gccinfo, empty);
		}
	}
	
}

int USER,NICE,SYSTEM,IDLE,IOWAIT,IRQ,SOFTIRQ;
//int STEALSTOLEN,GUEST;//主要存在虚拟机时，可省略
extern GtkWidget* cpu_label;

gint get_cpu_rate()
{
		
		int fd;
		int user, nice, system, idle, iowait, irq, softirq;
		float cpurate;
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
		
		int k = user-USER+nice-NICE+system-SYSTEM+iowait-IOWAIT+idle-IDLE+irq-IRQ+softirq-SOFTIRQ;
		//printf("all cpu1: %d\n", k);
		//printf("1a: %d, %d, %d, %d, %d, %d, %d\n", user, nice, system, iowait,idle, irq, softirq);
		//printf("1b: %d, %d, %d, %d, %d, %d, %d\n", USER, NICE, SYSTEM, IOWAIT,IDLE, IRQ, SOFTIRQ);
		
		USER = user;
		NICE = nice;
		SYSTEM = system;
		IDLE = idle;
		IOWAIT = iowait;
		IRQ = irq;
		SOFTIRQ = softirq;
	
		
		char cpuinfo[15] = "cpu： ";
		char output[10];
		sprintf(output, "%.3f", cpurate);
		strcat(cpuinfo, output);
		strcat(cpuinfo, "%");
		
		gtk_label_set_text(GTK_LABEL(cpu_label), cpuinfo);
		
		return 1;
		
}

extern GtkWidget* mem_label;
gint get_mem_rate()
{
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
	
	
	float memrate = 100*(MemTotal - MemFree - Buffers - Cached)/(float)MemTotal;
	
	char meminfo[15] = "mem： ";
	char output[10];
	sprintf(output, "%.3f", memrate);
	strcat(meminfo, output);
	strcat(meminfo, "%");
		
	gtk_label_set_text(GTK_LABEL(mem_label), meminfo);
	return 1;
}

extern GtkWidget* process_num_label;
gint get_process_num(void){
	DIR* dir;
    struct dirent* ptr;
    char show[5];
    int total=0;
  	char processnum[20]="process：";

   	dir = opendir("/proc");
   	while((ptr = readdir(dir))!=NULL){
		if(ptr->d_name[0] >= '0' && ptr->d_name[0] <= '9')
			total++;
	}
 	sprintf(show,"%3d",total);
   	strcat(processnum, show);
  	closedir(dir);
    gtk_label_set_text(GTK_LABEL(process_num_label), processnum);
    return 1;
}






