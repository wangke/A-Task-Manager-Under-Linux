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
#include <math.h>
#include <unistd.h>
	

#include "simple_info.h"

#define __NR_get_page_fault 347
#define size 7
#define Nsize 3
static GtkWidget* widget;
extern GtkWidget *table2[Nsize];
int seconds;
extern GtkWidget* time_label;
int cpuPoints[100], memPoints[100], lackPoints[100];



float draw_mem, draw_cpu, draw_lack;
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

void get_cpuinfo1(GString* string)
{
	int fd,i;
  	gchar buffer[1024];								
  	gchar *delim = "\n";
  	gchar *cpu_info[17];
  	gchar *tmp;
	
  	fd = open ("/proc/cpuinfo", O_RDONLY);//读取cpu相关信息的文件
  	read (fd, buffer, sizeof (buffer));
  	close (fd);

  	cpu_info[0] = strtok (buffer, delim);
  	for (i=1; i < 17 ; i++) {
		cpu_info[i] = strtok (NULL,delim);//分割字符串
  	}	  
  	for (i=0; i < 17; i++) {
  	  	tmp = strstr (cpu_info[i], ":");//忽略冒号:
		tmp += 2; 
        	cpu_info[i] = tmp;	  
 	}

  	g_string_append (string, "CPU型号       :");
  	g_string_append (string, cpu_info[4]);
  	g_string_append_c (string, '\n');
  	g_string_append (string, "CPU频率       :");
  	g_string_append (string, cpu_info[6]);
  	g_string_append_c (string, '\n');
}

void get_cpuinfo2(GString* string)
{
	int fd,i;
      	gchar buffer[128];		
      					
	gchar *os_info[8];
	gchar *delim = " ";
	
	fd = open ("/proc/version", O_RDONLY);//读取操作系统相关信息的文件
	read(fd, buffer, sizeof (buffer));
	
	
	os_info[0] = strtok (buffer, delim);
	for (i = 1; i < 8; i++) {
		os_info[i] = strtok (NULL, delim);//分割字符串
		
	}
	
	
	g_string_append (string, "版本号         :");
    g_string_append (string, os_info[2]);
    g_string_append_c (string, '\n');
    g_string_append (string, "Gcc 版本       :");
  	g_string_append (string, os_info[6]);
    g_string_append_c (string, '\n'); 	
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
		
		//int k = user-USER+nice-NICE+system-SYSTEM+iowait-IOWAIT+idle-IDLE+irq-IRQ+softirq-SOFTIRQ;
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
	
		draw_cpu = cpurate;
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
	
	//printf("%f\n", memrate);
	draw_mem = memrate;
	
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



void info_init()
{
	//GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	//show
	//gtk_table_attach_defaults(GTK_TABLE(table2[2]), vbox, 0,12,0,12);
	//GtkWidget* frame0 = gtk_frame_new("CPU : ");
	//show
	//gtk_box_pack_start(GTK_BOX(vbox), frame0, TRUE, TRUE, 10);
	
	GString* info1, *info2;
	info1 = g_string_new("");
	get_cpuinfo1(info1);
	info2 = g_string_new("");
	get_cpuinfo2(info2);
	g_string_append(info1, info2->str);
	
	GtkWidget *info_label = gtk_label_new(info1->str);
	//
	//gtk_container_add(GTK_CONTAINER(frame0), info_label);
	
	gtk_table_attach_defaults(GTK_TABLE(table2[2]), info_label, 0,12,0,12);
	
	//info = g_string_new("");
	//get_cpuinfo2(info);
	//info_label = gtk_label_new(info->str);
	//gtk_table_attach_defaults(GTK_TABLE(table2[2]), info_label, 0,7,7,12);
	//
	//gtk_container_add(GTK_CONTAINER(frame0), info_label);
	
}
int a = 0, b = 200;
int key;
gint draw_line(GtkWidget* widget, GdkGC* gc, int a,int  b)
{
 	key++;
 printf("a,b %d %d\n", a, b);
	gdk_draw_line((widget->window), gc, a, a,b,b);
	if(key%2 == 1)
		return TRUE;
	else
		return FALSE;
}

gboolean on_darea_expose ()
{
	


	GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(widget->window));
 GdkColor color;
 gdk_color_parse("RED", &color); 
 gdk_gc_set_foreground(gc, &color);
 
 	a+=10;
 	b+=10;
printf("ok");
	gdk_draw_line((widget->window), gc, a, a,b,b);
	
	return TRUE;
 
 
	
	
	//gtk_timeout_add(100, (GtkFunction)draw_line(widget, gc, a, b), NULL);

}
GdkPixmap *cpu_graph, *mem_graph, *lack_graph;

static gboolean cpu_expose_event(GtkWidget *widget, GdkEventExpose *event,  
        gpointer data)  
{  
	//gdk_draw_line((widget->window), widget->style->fg_gc[GTK_WIDGET_STATE(  
           // widget)], a, a,b,b);

    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(  
            widget)], cpu_graph, event->area.x, event->area.y, event->area.x,  
            event->area.y, event->area.width, event->area.height);  
    return FALSE;  
}  
static gboolean mem_expose_event(GtkWidget *widget, GdkEventExpose *event,  
        gpointer data)  
{  
	//gdk_draw_line((widget->window), widget->style->fg_gc[GTK_WIDGET_STATE(  
           // widget)], a, a,b,b);

    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(  
            widget)], mem_graph, event->area.x, event->area.y, event->area.x,  
            event->area.y, event->area.width, event->area.height);  
    return FALSE;  
}  

static gboolean lack_expose_event(GtkWidget *widget, GdkEventExpose *event,  
        gpointer data)  
{  
	//gdk_draw_line((widget->window), widget->style->fg_gc[GTK_WIDGET_STATE(  
           // widget)], a, a,b,b);

    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(  
            widget)], lack_graph, event->area.x, event->area.y, event->area.x,  
            event->area.y, event->area.width, event->area.height);  
    return FALSE;  
}  


static gboolean cpu_configure_event(GtkWidget *widget,  
        GdkEventConfigure *event, gpointer data)  
{  
    if (cpu_graph)  
    {  
        g_object_unref(cpu_graph);  
    }  
    cpu_graph = gdk_pixmap_new(widget->window, widget->allocation.width,  
            widget->allocation.height, -1);  
    gdk_draw_rectangle(cpu_graph, widget->style->white_gc, TRUE, 0, 0,  
            widget->allocation.width, widget->allocation.height);  
    return TRUE;  
}  

static gboolean mem_configure_event(GtkWidget *widget,  
        GdkEventConfigure *event, gpointer data)  
{  
    if (mem_graph)  
    {  
        g_object_unref(mem_graph);  
    }  
    mem_graph = gdk_pixmap_new(widget->window, widget->allocation.width,  
            widget->allocation.height, -1);  
    gdk_draw_rectangle(mem_graph, widget->style->white_gc, TRUE, 0, 0,  
            widget->allocation.width, widget->allocation.height);  
    return TRUE;  
}  

static gboolean lack_configure_event(GtkWidget *widget,  
        GdkEventConfigure *event, gpointer data)  
{  
    if (lack_graph)  
    {  
        g_object_unref(lack_graph);  
    }  
    lack_graph = gdk_pixmap_new(widget->window, widget->allocation.width,  
            widget->allocation.height, -1);  
    gdk_draw_rectangle(lack_graph, widget->style->white_gc, TRUE, 0, 0,  
            widget->allocation.width, widget->allocation.height);  
    return TRUE;  
}  

gint clear()
{
	gtk_widget_queue_draw(widget);
	return 1;
}
GtkWidget* cpu_draw_area, *mem_draw_area, *lack_draw_area;
void drawlines_init()
{
	
    GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	//show
	gtk_table_attach_defaults(GTK_TABLE(table2[0]), vbox, 0,12,0,12);
	
	GtkWidget *cpu_frame = gtk_frame_new("cpu曲线：");  
    
	//show
	gtk_box_pack_start(GTK_BOX(vbox), cpu_frame, TRUE, TRUE, 10);
	
	
    
   // gtk_widget_show(CPU_frame1);  
    
    cpu_draw_area = gtk_drawing_area_new();  
    gtk_widget_set_app_paintable(cpu_draw_area, TRUE);  
    gtk_drawing_area_size(GTK_DRAWING_AREA(cpu_draw_area), 40, 40);  
    gtk_container_add(GTK_CONTAINER(cpu_frame), cpu_draw_area);  
  
    g_signal_connect(cpu_draw_area, "expose_event",  
            G_CALLBACK(cpu_expose_event), NULL);  
    g_signal_connect(cpu_draw_area, "configure_event", G_CALLBACK(  
            cpu_configure_event), NULL);  
   // gtk_widget_show(cpu_draw_area);  
    int i;  
    for (i = 0; i < 100; i++)  
    {  
        
       cpuPoints[i] = (rand() % 30 + 50);  
       memPoints[i] = (rand() % 30 + 50);
       lackPoints[i] = (rand() % 30 + 50);
    }  
   
    //  gtk_timeout_add(100, (GtkFunction) drawGraph, NULL);  */
   
             
    GtkWidget* mem_frame = gtk_frame_new("mem曲线：");
    gtk_box_pack_start(GTK_BOX(vbox), mem_frame, TRUE, TRUE, 10);
    
    mem_draw_area = gtk_drawing_area_new();  
    gtk_widget_set_app_paintable(mem_draw_area, TRUE);  
    gtk_drawing_area_size(GTK_DRAWING_AREA(mem_draw_area), 40, 40);  
    gtk_container_add(GTK_CONTAINER(mem_frame), mem_draw_area);  
    
    g_signal_connect(mem_draw_area, "expose_event",  
            G_CALLBACK(mem_expose_event), NULL);  
    g_signal_connect(mem_draw_area, "configure_event", G_CALLBACK(  
            mem_configure_event), NULL);  
    
    
   	GtkWidget* lack_frame = gtk_frame_new("缺页曲线：");
   	gtk_box_pack_start(GTK_BOX(vbox), lack_frame, TRUE, TRUE, 10);
   	
   	lack_draw_area = gtk_drawing_area_new();  
    gtk_widget_set_app_paintable(lack_draw_area, TRUE);  
    gtk_drawing_area_size(GTK_DRAWING_AREA(lack_draw_area), 40, 40);  
    gtk_container_add(GTK_CONTAINER(lack_frame), lack_draw_area);
    
    g_signal_connect(lack_draw_area, "expose_event",  
            G_CALLBACK(lack_expose_event), NULL);  
    g_signal_connect(lack_draw_area, "configure_event", G_CALLBACK(  
            lack_configure_event), NULL);    
    
   	/*mem_draw_area = gtk_drawing_area_new();
   	gtk_widget_set_app_paintable(mem_draw_area, TRUE);
   	gtk_drawing_area_size(GTK_DRAWING_AREA(cpu_draw_area), 40, 40);
   	
    gtk_container_add(GTK_CONTAINER(memframe), mem_draw_area);  
  
    g_signal_connect(mem_draw_area, "expose_event",  
            G_CALLBACK(cpu_expose_event), NULL);  
    g_signal_connect(mem_draw_area, "configure_event", G_CALLBACK(  
            cpu_configure_event), NULL);  
               */
}

int PAGEFAULT;
     
gint lines_refresh()
{

	int page_fault = syscall(__NR_get_page_fault);
	int value = page_fault - PAGEFAULT;
	PAGEFAULT = page_fault;
	draw_lack = (float)(100*value/15000);
	//printf("lack %f\n", draw_lack);
	 /*gdk_draw_rectangle(cpu_graph ,

              cpu_draw_area->style->white_gc ,

              TRUE , 0 , 0 ,

              cpu_draw_area->allocation.width ,

              cpu_draw_area->allocation.height);*/

     // gtk_widget_queue_draw(cpu_draw_area);


	//gtk_widget_queue_draw(cpu_draw_area);
	/* cpu */
	/* 建一个矩形绘图区 */
	//GdkGC *gc_chart = gdk_gc_new(cpu_draw_area->window);
	//	update_widget_bg(gc_chart, BACK_IMAGE);

	/* 背景颜色 */
	GdkColor color;
	//color.red = 0x0000;
	//color.green = 0x0000;
	//color.blue = 0x0000;
	//gdk_gc_set_rgb_fg_color(gc_chart, &color);
	int width, height, curPoint, step, curPoint1, curPoint2;
	gdk_color_parse("green", &color);
	gdk_draw_rectangle(cpu_graph,cpu_draw_area->style->white_gc, TRUE, 0, 0,
			cpu_draw_area->allocation.width, cpu_draw_area->allocation.height);
	gdk_draw_rectangle(mem_graph, mem_draw_area->style->white_gc, TRUE,0,0,
			mem_draw_area->allocation.width, mem_draw_area->allocation.height);
	gdk_draw_rectangle(lack_graph, lack_draw_area->style->white_gc, TRUE,0,0,
			lack_draw_area->allocation.width, lack_draw_area->allocation.height);
	width = cpu_draw_area->allocation.width;
	height = cpu_draw_area->allocation.height;
	
	
	curPoint = (int) ((draw_cpu/100) * (double) height) + 10;
	curPoint1 = (int)((draw_mem/100) * (double) height);
	curPoint2 = (int)((draw_lack/100) * (double) height);
	
	int i;
	for (i = 0; i < 99; i++)
	{
		/* 后一时刻的为前面取代 */
		cpuPoints[i] = cpuPoints[i + 1];
		memPoints[i] = memPoints[i + 1];
		lackPoints[i] = lackPoints[i + 1];
	}
	cpuPoints[99] = height - curPoint;
	memPoints[99] = height - curPoint1;
	lackPoints[99] = height - curPoint2;
	
	step = width / 99;
	GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(cpu_graph));
	gdk_color_parse("green", &color);
	//if ((draw_mem/100) > 0.1)
		//gdk_color_parse("red", &color);
	gdk_gc_set_foreground(gc, &color);
	/*	gdk_gc_set_line_attributes(GdkGC *gc,//
	 line_width,
	 GdkLineStyle line_style,
	 GdkCapStyle cap_style,
	 GdkJoinStyle join_style);
	 */
	gdk_gc_set_line_attributes(gc, 1, GDK_LINE_SOLID, GDK_CAP_ROUND,
			GDK_JOIN_MITER);

	for (i = 99; i >= 1; i--)
	{
		gdk_draw_line(GDK_DRAWABLE(cpu_graph), cpu_draw_area->style->black_gc, i * step, cpuPoints[i], /* 第一个点坐标 */
		(i - 1) * step, cpuPoints[i - 1]); /* 第二个点坐标 */
		gdk_draw_line(GDK_DRAWABLE(mem_graph), mem_draw_area->style->black_gc, i*step, memPoints[i],
		(i-1)*step, memPoints[i-1]);
		gdk_draw_line(GDK_DRAWABLE(lack_graph), lack_draw_area->style->black_gc, i*step, lackPoints[i],
		(i-1)*step, lackPoints[i-1]);
	}

	gtk_widget_queue_draw(cpu_draw_area);
	gtk_widget_queue_draw(mem_draw_area);
	gtk_widget_queue_draw(lack_draw_area);
	
	return TRUE;

}



