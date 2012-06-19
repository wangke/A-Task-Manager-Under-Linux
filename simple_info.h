#ifndef SIMPLEINFO_H
#define SIMPLEINFO_H
#include <gtk/gtk.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>

gint get_time();
void get_cpuinfo1(GString* string);
void get_cpuinfo2(GString* string);
gint get_cpu_rate();
gint get_mem_rate();
gint get_process_num();
void info_init();
gboolean on_darea_expose();
void drawlines_init();
gint lines_refresh();

#endif 
