#include <gtk/gtk.h>
#include <stdlib.h>

   /*创建一个指向GtkWidget类型的指针（创建文本框使用）*/
static GtkWidget *entry;

   /*创建全局变量:名为“fnum”（双精度，第一个输入量）；
                     “snum”（双精度，第二个输入量）*/
gdouble fnum = 0;
gdouble snum = 0;

   /*创建全局控制变量：名为“num_count”（整型，控制输入位数）；
                          “operator”（整型，控制输入的运算符号）；
                          “first_num”（布尔型，控制输入的次数）；
                          “have_result”(布尔型，为重新进行计算作好准备)*/
gint     num_count =0;
gint     operator = 0;
gboolean first_num = TRUE;
//gboolean first_num_equal = FALSE;
gboolean have_dot = FALSE; //小数点
gboolean have_result = FALSE;
gchar    number[100];



/*........................................................................ */
//清除函数：ClearReset（）
//1：清除屏幕上的内容（TURE）;2：初始化变量并清屏（FALSE）

void ClearReset(gboolean clear_only)
        {
            /*初始化number[100]等，分配内存空间*/
            gint i = 0;
            for(i = 0;i<100;i++)
                number[i] = '\0';
            fnum = 0;
            snum = 0;
            operator = 0;
            num_count = 0;
            first_num = TRUE;
            have_dot = FALSE;
            have_result = FALSE;
            //first_num_equal = FALSE;

            /*清除屏幕*/
            if(clear_only = TRUE)
               gtk_entry_set_text(GTK_ENTRY(entry),"0");
            }       



/*---------------------------------------------------------------------------------- */
//按下数字时的回调函数：PressNum（）

void PressNum(GtkButton *button,gpointer data)
        {
            /*创建一个指向字符的指针变量："num"(用来操作输入量)*/
            const gchar *num;             
            int i;
                                 
            /*控制输入数字的位数*/
            if(num_count == 9)
            return;
          
            /*输入位数记数*/
            num_count++;
          
            /*通过button的label获取每次输入的数值（字符串型）*/
            num=gtk_button_get_label(GTK_BUTTON(button));
          
            /*g_strlcat() 可以用来组合每次输入数字（字符串）起到累加作用*/
            i=g_strlcat(number,num,100);

                                
            /*输入第一个数和第二个数的控制开关,strtod()是把字符串转换为gdouble型*/
            if(first_num )
            {
            /*第一次输入*/
            fnum=strtod(number,NULL);

            }         
            else
            {
                    /*第二次输入*/
                    /*防止除法时除数为0*/
                    if(num_count == 1);
                    snum=strtod(number,NULL);
    
                    if(num_count == 1 && operator ==4 && snum == 0)
                    //gtk_entry_set_text(GTK_ENTRY(entry),"ERROR");
                    return;
            }

           if (number[0]=='0' && number[1]!='.' &&num_count>=2 )
            {
                   gint i;
               for (i =0 ; i<100; i++)
                   {   
                      number[i] = number[i+1];
                   }
            }
                  
            /*把输入的数字显示出来*/
            gtk_entry_set_text(GTK_ENTRY(entry),number);

            //g_print("F:%f\n",fnum);
            //g_print("S:%f\n",snum);

        }



/*----------------------------------------------------------------------------------*/
//按下小数点时的回调函数：PressDot（）

void PressDot(GtkButton *button,gpointer data)
        {
           gint i;
            /*重复计算的切换开关*/
            if(have_result)
                ClearReset(FALSE);
              
            /*如果小数点在第一位则不显示*/
            if(num_count == 0)
             {
              ClearReset(TRUE);
             return;
             }
            /*把数加上小数点进行显示.have_dot防止输入两次小数点*/
           if(have_dot == FALSE)
                {
                    have_dot = TRUE;
                    i=g_strlcat(number,".",100);
                if(first_num)
                   /*第一个数字输入*/
                  fnum=strtod(number,NULL);
                else
                    {
                       /*第二个数字输入*/
                      snum=strtod(number,NULL);
                       /*把输入的数字显示出来*/
                      gtk_entry_set_text(GTK_ENTRY(entry),number);
                    }
                
                }
        
        }



/*------------------------------------------------------------------------------------*/
//按下清零键时的回调函数：PressClear（）

void PressClear(GtkButton *button,gpointer data)
        {
            ClearReset(FALSE);
        }



/*------------------------------------------------------------------------------------*/
//按下运算符时的回调函数：PressOperator（）

void PressOperator(GtkButton *button,gpointer data)
        {
            gint i;
            switch(GPOINTER_TO_INT(data))
                        {
                            case 1: operator = 1;//加法
                            break;
                           
                            case 2: operator = 2;//减法
                            break;
                           
                            case 3: operator = 3;//乘法
                            break;
                          
                            case 4: operator = 4;//除法
                            break;
                        }

            // g_print("F:%f\n",fnum);
            // g_print("S:%f\n",snum);

            /*切换输入第二个数*/
            first_num = FALSE;
            num_count = 0;
            have_dot =FALSE;
            for( i = 0;i<100;i++)
               number[i] = '\0';
        }


      
/*-------------------------------------------------------------------------------------*/
//按下等于号时的回调函数：PressEqual（）
          
void PressEqual(GtkButton *button,gpointer data)
      {
            gdouble numb;
            gchar *result;
            gchar num[100];
            gint e = 0;
            g_print("F:%f\n",fnum);
            g_print("S:%f\n",snum);
            /*进行运算*/
            switch(operator)
                {
                    case 1: numb = fnum + snum ;
                             break;
                    case 2: numb = fnum - snum;
                             break;
                    case 3: numb = fnum * snum;
                             break;
                    case 4: numb = fnum / snum;
                             break;
               //防一开始什么也不按，就按一个= 会档机的问题
                    default: gtk_entry_set_text(GTK_ENTRY(entry),number);
                             e = 1;
                             break;
               }

           if (e==0)
            {    /*把结果转换成字符串*/
            result = g_ascii_dtostr(num,100,numb);
            fnum = numb;
            //输出结果
            gtk_entry_set_text(GTK_ENTRY(entry),result);  
            have_result=TRUE;
          //first_num_equal = TRUE;
            }
          
         /*  
         result = g_ascii_dtostr(num,100,numb);
         fnum = numb;
         gtk_entry_set_text(GTK_ENTRY(entry),result);
         have_result=TRUE;
         first_num_equal = TRUE;     */

    }


/*-------------------------------------------------------------------------------------*/
//主函数:main()
int main(int argc,char* argv[])
        {
           /*创建指向GtkWidget类型的指针*/
            GtkWidget *window;
            GtkWidget *button;
            GtkWidget *vbox;
            GtkWidget *hbox;
            GtkWidget *table;
            GtkBuilder *builder = NULL;
            /*初始化*/
            gtk_init(&argc,&argv);

            builder=gtk_builder_new();
            gtk_builder_add_from_file(builder,"counter.glade",NULL);
            gtk_builder_connect_signals(builder, NULL);
           // 根据 ID 获取子构件 
          window=GTK_WIDGET(gtk_builder_get_object(builder,"window")); 
          entry=GTK_ENTRY(gtk_builder_get_object(builder,"entry"));
            gtk_entry_set_text(GTK_ENTRY(entry),"0");
            gtk_editable_set_editable(GTK_EDITABLE(entry),FALSE);
            gtk_widget_set_direction(entry,GTK_TEXT_DIR_RTL);
         // text = GTK_LABEL(gtk_builder_get_object(builder, "label-main")); 

button=GTK_BUTTON(gtk_builder_get_object(builder, "button1")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressClear),NULL);
button=GTK_BUTTON(gtk_builder_get_object(builder, "button2")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button3")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button4")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button5")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressOperator),(gpointer)1);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button6")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button7")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button8")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button9")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressOperator),(gpointer)2);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button10")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button11")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button12")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button13")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressOperator),(gpointer)3);

button=GTK_BUTTON(gtk_builder_get_object(builder, "button14")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressNum),NULL);
button=GTK_BUTTON(gtk_builder_get_object(builder, "button15")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressDot),NULL);
button=GTK_BUTTON(gtk_builder_get_object(builder, "button16")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressEqual),NULL);
button=GTK_BUTTON(gtk_builder_get_object(builder, "button17")); 
          g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressOperator),(gpointer)4);
  
         // 获取到 UI 对象后，GtkBuilder 对象已没有作用，释放了 
  
         g_object_unref(G_OBJECT(builder)); 

                   
            gtk_widget_show_all(window);
            gtk_main();
            return 0;
        }