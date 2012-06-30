task_begin:task_begin.o simple_info.o complex_info.o
	gcc -g -Wall -o $@ $^ `pkg-config --cflags --libs gtk+-2.0`
.c.o:
	gcc -g -Wall -c $< `pkg-config --cflags --libs gtk+-2.0`

