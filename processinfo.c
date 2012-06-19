#include<stdio.h>
#include"dirent.h"
#include"unistd.h"
#include"sys/file.h"
#include"fcntl.h"
#include"string.h"
#include"time.h"
#include"math.h"
#include<stdlib.h>
#include<sys/time.h>


#define PMAX 300//最大进程数

struct proinfo
{
	int Utime;
	int Stime;
	int Cutime;
	int Cstime;
	int id;
	
}pro_info[PMAX];

int USER,NICE,SYSTEM,IDLE,IOWAIT,IRQ,SOFTIRQ;
long f;
int main()
{
	while(1){
	f = 0;
	int fd;
	int user, nice, system, idle, iowait, irq, softirq;
	float cpurate;
	fd = open("/proc/stat", O_RDONLY);
	if(fd == 0)
	{
		printf("open /proc/stat failed\n");
		return -1;
	}
	char buffer1[50*1024];
	ssize_t rd = read(fd, buffer1, sizeof(buffer1));
	if(rd == 0 || rd == sizeof(buffer1))
	{
		printf("read /proc/stat failed\n");
		return -1;
	}
	close(fd);
	buffer1[rd] = '\0';
	char *find = strstr(buffer1, "cpu");
	if(find == NULL)
	{
		printf("find error\n");
		return -1;
	}
	sscanf(find, "cpu %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq);	
	int totalcpu = (user-USER+nice-NICE+system-SYSTEM+iowait-IOWAIT+idle-IDLE+irq-IRQ+softirq-SOFTIRQ);
	
	//printf("total: %d\n", user+nice+system+iowait+idle+irq+softirq);
	USER = user;
	NICE = nice;
	SYSTEM = system;
	IOWAIT = iowait;
	IDLE = idle;
	IRQ = irq;
	SOFTIRQ = softirq;
	
	
	
	
	DIR* dir;
	struct dirent* process;
	
	
	char prodir[128];
	char memdir[128];
	char probuffer[128];
	char membuffer[1024];
	int i, num = 0, fd1, fd2;
	char* delim = " ";
	int utime, stime, cutime, cstime;
	float proresult[PMAX];
	float memresult[PMAX];
	char killable[PMAX];
		
	char* proinfo[26];
	char* meminfo[20];
	
	dir = opendir("/proc");
	while((process = readdir(dir)) != NULL)
	{
		
		if((process->d_name[0] >= '0') && (process->d_name[0] <= '9'))
		{
		
			
			sprintf(prodir, "/proc/%s/stat", process->d_name);
			sprintf(memdir, "/proc/%s/status", process->d_name);
			
			fd1 = open(prodir, O_RDONLY);
			read(fd1, probuffer, sizeof(probuffer));
			close(fd1);
			
			
			proinfo[0] = strtok(probuffer, delim);
			for(i = 1; i < 26; i++)
			{
				proinfo[i] = strtok(NULL, delim);
			}
				
			//printf("%s\n", proinfo[0]);
			
			utime = atoi(proinfo[13]);
			stime = atoi(proinfo[14]);
			cutime = atoi(proinfo[15]);
			cstime = atoi(proinfo[16]);
				
			//printf("%d %d %d %d\n", utime, stime, cutime, cstime);
			
			float undo = (utime+stime+cutime+cstime - pro_info[num].Utime-pro_info[num].Stime-pro_info[num].Cutime-pro_info[num].Cstime)/(float)totalcpu;
			
			undo = (undo >= 0) ? undo : ((-1)*undo);
			proresult[num] = undo;
			
			
			
			f += (utime+stime+cutime+cstime);
			pro_info[num].Utime = utime;
			pro_info[num].Stime = stime;
			pro_info[num].Cutime = cutime;
			pro_info[num].Cstime = cstime;
			 
			//printf(" %f\n", proresult[num]); 
			
			fd2 = open(memdir, O_RDONLY);
			read(fd2, membuffer, sizeof(membuffer));
			close(fd2);
			
			meminfo[0] = strtok(membuffer, "\n");
			
			
			for(i = 1; i < 20; i++)
				meminfo[i] = strtok(NULL, "\n");
				
			char* s = meminfo[14];
			char mem[10];
			memset(mem, '\0', sizeof(mem));
			int j = 0;
			//printf("%s\n", meminfo[14]);
			//printf("%d\n", sizeof(meminfo[14]));
			
			if(s[0] == 'S')
			{
				mem[0] = 0;
				killable[num] = 0;
			}
			else if(s[0] == 'V')
			{
				
				killable[num] = 1;
				//printf("%s\n", s);
				for(i = 0; i < 30 && s[i] != 'k'; i++)
				{
					
					if(s[i] >= '0' && s[i] <= '9')
					 {
					 	
						mem[j++] = s[i];
						//printf("%c ", s[i]);
					 }
					 	
				}
				//printf("%d\n", j);
				//printf("%s\n", mem);
			
			}
			
			int rss = atoi(mem);
			rss = (rss >= 0) ? rss : ((-1)*rss);
			memresult[num] = rss/(float)2052156;
			printf("process: %s  %f mem: %f\n", process->d_name, 4*proresult[num], memresult[num]);
			
			}
			
			num++;
		
		
	}
	closedir(dir);
	//printf("allcpu: %ld\n",4*f);
	sleep(3);
	}
}
