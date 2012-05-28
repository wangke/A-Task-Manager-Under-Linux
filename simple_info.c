#include <gtk/gtk.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simple_info.h"

#define size 7

extern GtkWidget* time_label;
gint get_time()
{
	time_t t;
    struct tm *p;//储存时间的结构体
   	t = time(NULL);
    p = localtime(&t); //取得当地时间
    
    char buf[15]="时间 ";
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






