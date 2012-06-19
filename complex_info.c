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
#include"sys/file.h"
#include"math.h"



#include "complex_info.h"


#define Nsize 3
#define PMAX 300//最大进程数

enum//进程界面的枚举类型
{
	NAME_COLUMN,//进程名列
	PID_COLUMN,//进程PID列
	STATUS_COLUMN,//进程状态列
	CPU_COLUMN,//CPU信息列
	MEMORY_COLUMN,//内存信息列
	INDEX_COLUMN,//序号
	NP_COLUMNS
};

struct proinfo
{
	int Utime;
	int Stime;
	int Cutime;
	int Cstime;
	int id;
	
}pro_info[PMAX];

extern GtkListStore* process_store;
GtkWidget* ptree_view;
int selected_index;

void killbutton_clicked(gpointer data)
{
	

	GtkTreeSelection *selection;
  	GtkTreeModel *model;
  	GtkTreeIter iter;
  	gchar *pid;
  	pid_t pid_num;

  	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));//获得当前选择的项
  	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);//在树的相应列中获得该进程的PID
  		pid_num = atoi (pid);//字符串转换成长整数
	    
	    	if(kill (pid_num, SIGTERM) == -1 ) {//根据PID结束该进程
			//gchar *title = "错误";
			//gchar *content = "Termination Failed,Check UID";
			//show_dialog (title, content);//结束进程失败信息输出
			printf("oh no!");
			
		}
	    	pro_fill();
  	}
}

void tree_selected(GtkTreeSelection* selection, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel* model;
	int index;
	
	if(gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get(model, &iter, INDEX_COLUMN, &index, -1);
		//printf("jia %d\n", index);
		selected_index = index;
		
	}
}

void set_select_as_before()
{

	//printf("zhen %d\n", selected_index);
	GtkTreeSelection* select;
	GtkTreePath* path;
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));
	//path = gtk_tree_path_new_from_indices(important_index, -1);
	//gtk_tree_selection_select_path(select, path );
	//path = gtk_tree_path_new_from_indices(important_index+1, -1);
	//gtk_tree_selection_select_path(select, path);
	//gtk_tree_view_set_cursor( GTK_TREE_VIEW(ptree_view), path, NULL, FALSE ); 
	
	GtkTreeIter iter;
	GtkTreeModel* model;
	int judgeit = selected_index;
	gint i;
	int index;
	for(i = 0; i < 200; i++)
	{
		path = gtk_tree_path_new_from_indices(i, -1);
		gtk_tree_selection_select_path(select, path);
		if(gtk_tree_selection_get_selected(select, &model, &iter))
		{
			gtk_tree_model_get(model, &iter, INDEX_COLUMN, &index, -1);
			if(index == judgeit)
			{
				//printf("final %d\n", index);
				break;
				
			}
				
				
			
		}
	}
	
	
}

int USER2,NICE2,SYSTEM2,IDLE2,IOWAIT2,IRQ2,SOFTIRQ2;
long f;


float fcpu,fmem; 




gint pro_fill()
{
	
	gtk_list_store_clear(process_store);
	f = 0;
	int fd;
	int user, nice, system, idle, iowait, irq, softirq;

	
	GtkTreeIter iter;
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
	int totalcpu = (user-USER2+nice-NICE2+system-SYSTEM2+iowait-IOWAIT2+idle-IDLE2+irq-IRQ2+softirq-SOFTIRQ2);
	
	//printf("total2: %d\n", totalcpu);
	//printf("2a: %d, %d, %d, %d, %d, %d, %d\n", user, nice, system, iowait,idle, irq, softirq);
	
	//printf("2b: %d,%d, %d, %d, %d, %d, %d\n", USER2, NICE2, SYSTEM2, IOWAIT2, IDLE2, IRQ2, SOFTIRQ2);
	
	USER2 = user;
	NICE2 = nice;
	SYSTEM2 = system;
	IOWAIT2 = iowait;
	IDLE2 = idle;
	IRQ2 = irq;
	SOFTIRQ2 = softirq;
	
	
	
	
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
				
			//printf("size: %d\n", sizeof(proinfo[1]));
			//printf("start: %c, edn: %c\n", *(proinfo[1]), *(proinfo[1] + sizeof(proinfo[1])));
			
			char maked[30];
			memset(maked, '\0', sizeof(maked));
			for(i = 1; proinfo[1][i] != ')'; i++)
			{
				maked[i - 1] = proinfo[1][i];
			}
			//printf("maked: %d, %s\n", sizeof(maked), maked);
			
			utime = atoi(proinfo[13]);
			stime = atoi(proinfo[14]);
			cutime = atoi(proinfo[15]);
			cstime = atoi(proinfo[16]);
				
			//printf("1 : %d %d %d %d\n", utime, stime, cutime, cstime);
			//printf("2 : %d %d %d %d\n", pro_info[num].Utime, pro_info[num].Stime, pro_info[num].Cutime, pro_info[num].Cstime);
			
			float undo = (utime+stime+cutime+cstime - pro_info[num].Utime-pro_info[num].Stime-pro_info[num].Cutime-pro_info[num].Cstime)/(float)totalcpu;
			//printf("1: %d\n", (utime+stime+cutime+cstime - pro_info[num].Utime-pro_info[num].Stime-pro_info[num].Cutime-pro_info[num].Cstime));
			//printf("2: %d\n", totalcpu);
			
			
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
			//printf("process: %s  %f mem: %f\n", process->d_name, 4*proresult[num], memresult[num]);
			
			gchar cpubuf[10];
			gchar membuf[10];
			
			sprintf(cpubuf, "%-.3f%%", 400*proresult[num]);
			sprintf(membuf, "%-.3f%%", 100*memresult[num]);
			
			fcpu += 4*proresult[num];
			fmem += memresult[num];
			
			gtk_list_store_append (process_store, &iter);//增加到列表
			gtk_list_store_set (process_store, &iter,
					NAME_COLUMN,maked,
					PID_COLUMN,proinfo[0],
					STATUS_COLUMN,proinfo[2],
					CPU_COLUMN,cpubuf,
					MEMORY_COLUMN,membuf,
					INDEX_COLUMN, num,
					-1);
			num++;
			
		}
			
			
		
		
	}
	closedir(dir);
	
	//printf("allcpu: %f\n", fcpu);
	//printf("allmem: %f\n", fmem);
	
	fcpu = 0;
	fmem = 0;
	//printf("allcpu: %ld\n",4*f);
	
	
	
	/*GtkTreeSelection* select;
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));
	GtkTreePath* path;
	path = gtk_tree_path_new_from_indices(important_index, -1 );
	gtk_tree_selection_select_path(select, path );
	gtk_tree_view_set_cursor( GTK_TREE_VIEW(ptree_view), path, NULL, FALSE ); 
	*/
	set_select_as_before();
	return 1;
}


/*gint refresh(GtkListStore* store)
{
	gtk_list_store_clear(store);
	pro_fill(store);
	return 1;	
}*/

extern GtkWidget *table2[Nsize];
void pro_init()
{
	
	gchar* cloumn_name[6] = {"进程", "Pid", "状态", "cpu占用", "内存占用", "序号"};
	
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	//show
	gtk_table_attach_defaults(GTK_TABLE(table2[1]), vbox, 0, 12, 0, 10);
	

	
	GtkWidget* scroll_window = gtk_scrolled_window_new(NULL, NULL);//带滚动条的窗体
	gtk_widget_set_size_request(scroll_window, 300, 430);
	//show
	gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 0);//滚动窗口加入盒子中，可调整大小
	
	
	process_store = gtk_list_store_new(NP_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);
	
	ptree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(process_store));
	
	g_object_unref(G_OBJECT(process_store));
	
	gtk_container_add (GTK_CONTAINER (scroll_window), ptree_view);
	GtkCellRenderer* renderer;
	int i;
	GtkTreeViewColumn* column;
	for(i = 0; i < 6; i++)
	{
	
		
		renderer = gtk_cell_renderer_text_new();//用于显示文字
		column = gtk_tree_view_column_new_with_attributes(cloumn_name[i], renderer, "text", i, NULL);//新建一列
		gtk_tree_view_column_set_sort_column_id (column, i);
		gtk_tree_view_append_column(GTK_TREE_VIEW(ptree_view), column);//将该列加到树中
		
	}
	
	GtkWidget* hbox = gtk_hbox_new(FALSE, 0);//横排
	//show;
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 0);
	
	GtkWidget* killbutton = gtk_button_new();
	gtk_widget_set_size_request(killbutton, 60, 30);
	gtk_button_set_label(GTK_BUTTON(killbutton), "kill");
	
	g_signal_connect (G_OBJECT (killbutton), "clicked", G_CALLBACK(killbutton_clicked), NULL);
	
	//添加功能函数
	gtk_table_attach_defaults(GTK_TABLE(table2[1]), killbutton, 9,11,11,12);
	
	/*int timer;
	gpointer data;
	timer = gtk_timeout_add(3000, (GtkFunction)pro_fill(process_store), data);
	*/
	//pro_fill(process_store);
	

	GtkTreeSelection* select = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_BROWSE);
	g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(tree_selected), NULL); 
	

	
}














