#include <gtk/gtk.h>
gboolean on_darea_expose (GtkWidget *widget,
     GdkEventExpose *event,
     gpointer user_data);
int
main (int argc, char *argv[])
{
 GtkWidget *window;
 gtk_init (&argc, &argv);
 window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 g_signal_connect (GTK_OBJECT (window), "expose-event",
               GTK_SIGNAL_FUNC (on_darea_expose), NULL);
 gtk_widget_show_all (window);
 gtk_main ();
 return 0;
}
gboolean
on_darea_expose (GtkWidget *widget,
   GdkEventExpose *event,
   gpointer user_data)
{
 GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(widget->window));
 GdkColor color;
 gdk_color_parse("darkred", &color); 
 gdk_gc_set_foreground(gc, &color);
 //gdk_gc_set_line_attributes (gc, 1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER); 
 gdk_draw_line((widget->window), gc, 10, 10,100,100);
 //gdk_draw_line((widget->window), widget->style->fg_gc[0], 10, 10,50,50);
 return TRUE;
}
