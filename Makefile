CC = gcc
all:
		$(CC) -o task_begin task_begin.c `pkg-config --cflags --libs gtk+-2.0`
