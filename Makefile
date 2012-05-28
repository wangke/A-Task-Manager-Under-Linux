task_begin:task_begin.o simple_info.o
	gcc -o $@ $^ `pkg-config --cflags --libs gtk+-2.0`
.c.o:
	gcc -c $< `pkg-config --cflags --libs gtk+-2.0`

