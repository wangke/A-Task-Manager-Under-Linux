#include<gtk/gtk.h>
//画统计图
gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer user_data)
{
	cairo_t *cr;
	double *data;
	data=user_data;
	cr = gdk_cairo_create (widget->window);

	
	
	cairo_set_line_width(cr,1);//线条宽度
	cairo_set_font_size(cr, 13);//字体大小
	//字体编码格式
	cairo_select_font_face (cr, "WenQuanYi Micro Hei", CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_NORMAL);
	
	/*//Y轴
	cairo_move_to(cr,20,30);
	cairo_line_to(cr,20,400);
	//X轴
	cairo_move_to(cr,20,400);
	cairo_line_to(cr,400,400);*/
	
	//显示字符y
	cairo_move_to(cr,20,20);
	cairo_show_text(cr, "y"); 
	//显示字符x
	cairo_move_to(cr,410,400);
	cairo_show_text(cr, "x"); 
 
	//矩形    
	cairo_set_source_rgb (cr, 0, 0.3, 0);
        cairo_rectangle (cr, 20, 80, *data, 20);//x,y,宽，高
        cairo_move_to(cr, *data+20, 80+15);
        cairo_show_text(cr, "13％");
                
        cairo_rectangle (cr, 20, 110, *(data+1), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+1)+20, 110+15);
        cairo_show_text(cr, "17％");
        
        cairo_rectangle (cr, 20, 140, *(data+2), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+2)+20, 140+15);
        cairo_show_text(cr, "25％");
        
        cairo_rectangle (cr, 20, 170, *(data+3), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+3)+20, 170+15);
        cairo_show_text(cr, "15％");
        
        cairo_rectangle (cr, 20, 200, *(data+4), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+4)+20, 200+15);
        cairo_show_text(cr, "5％");
        
        cairo_rectangle (cr, 20, 230, *(data+5), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+5)+20, 230+15);
        cairo_show_text(cr, "2％");
        
        cairo_rectangle (cr, 20, 260, *(data+6), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+6)+20, 260+15);
        cairo_show_text(cr, "3％");
        
        cairo_rectangle (cr, 20, 290, *(data+7), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+7)+20, 290+15);
        cairo_show_text(cr, "10％");
        
        cairo_rectangle (cr, 20, 320, *(data+8), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+8)+20, 320+15);
        cairo_show_text(cr, "9％");

        cairo_rectangle (cr, 20, 350, *(data+9), 20);//x,y,宽，高
        cairo_move_to(cr, *(data+9)+20, 350+15);
        cairo_show_text(cr, "1％");
        cairo_fill (cr);
    
	cairo_set_source_rgb(cr,0,0,0);//线条颜色
    	//Y轴
        cairo_move_to(cr,20,30);
	cairo_line_to(cr,20,400);
	
	//X轴
	cairo_move_to(cr,20,400);
	cairo_line_to(cr,400,400);
	
	cairo_move_to(cr,10,400);
	cairo_show_text(cr, "0");
    	
	cairo_stroke_preserve (cr);
	cairo_fill (cr);
    	cairo_destroy(cr);
	return FALSE;
	
}

int main(int argc , char *argv[])
{
	GtkWidget *window;
	double user_data[10]={13,17,25,15,5,2,3,10,9,1};
	gtk_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(GTK_OBJECT(window),"expose-event",G_CALLBACK(on_expose_event),user_data);
	g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_title (GTK_WINDOW (window), "统计图");
	gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);
	
	gtk_widget_set_app_paintable(window, TRUE);//画图有效
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
