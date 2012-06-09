#ifndef SIMPLEINFO_H
#define SIMPLEINFO_H
#include <gtk/gtk.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>

gint get_time();
void cpuinfo();
gint get_cpu_rate();
gint get_mem_rate();
gint get_process_num();

#endif 
