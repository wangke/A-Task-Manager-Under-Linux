#include <gtk/gtk.h>
#include "simple_info.h"
#define Nsize 3

int timer;
gpointer data;
GtkWidget *time_label;

//创建根菜单项
GtkWidget *CreateMenuItem(GtkWidget *MenuBar, char *test)
{
    GtkWidget *MenuItem;
    MenuItem = gtk_menu_item_new_with_label(test);
   	gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), MenuItem);//根菜单项关联根菜单栏
    return MenuItem;
}

//创建子菜单项
void *CreateMenuTask(GtkWidget *MenuItem)
{
    GtkWidget *Menu;
   	Menu = gtk_menu_new();
   	GtkWidget *Exit;
   	Exit = CreateMenuItem(Menu,"退出");
    gtk_signal_connect(GTK_OBJECT(Exit),"activate",G_CALLBACK(gtk_main_quit),NULL);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem),Menu);//菜单栏关联根菜单项
}

//进程内容填充
void notebook_process_init()
{
	
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *table1;
	GtkWidget *menuBar;
	GtkWidget *noteBook;
	GtkWidget *MenuItemTask;
	const char *title[3] = {"系统信息","进程信息","总体信息"};
	GtkWidget *label[Nsize];//页面标题
	GtkWidget *frame[Nsize];//页面框架
	GtkWidget *table2[Nsize];//页面装入table,方便数据填充	
	gtk_init(&argc, &argv);//初始化命令行参数 虽不使用但是必需
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"任务管理器");
	gtk_widget_set_usize(window, 600, 600); 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	
	table1 = gtk_table_new(12,11,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table1);
	
	menuBar = gtk_menu_bar_new();
	gtk_table_attach_defaults(GTK_TABLE(table1), menuBar,0,11,0,1);
	MenuItemTask = CreateMenuItem(menuBar, "进程");
  	CreateMenuTask(MenuItemTask);
  	
   	noteBook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(noteBook),GTK_POS_TOP);//设置notebook的格式
	gtk_table_attach_defaults(GTK_TABLE(table1), noteBook, 0, 11, 1, 11);
	gint i;
	for(i = 0; i < Nsize; i++)
	{
		label[i] = gtk_label_new(title[i]);
		frame[i] = gtk_frame_new(NULL);
		gtk_container_set_border_width(GTK_CONTAINER(frame[i]), 10);
		gtk_widget_set_size_request(frame[i],450,450);
		gtk_notebook_append_page(GTK_NOTEBOOK(noteBook),frame[i],label[i]);
		table2[i] = gtk_table_new(11,11,TRUE);
		gtk_container_add(GTK_CONTAINER(frame[i]),table2[i]);
	}
	
	time_label = gtk_label_new("");
	timer = gtk_timeout_add(1000, (GtkFunction)get_time, data);
	gtk_table_attach_defaults(GTK_TABLE(table1), time_label, 8,10,1,2);
	
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);//可关闭
	gtk_widget_show_all(window);
	gtk_main();//主事件循环
	return 0;
}



