CFLAGS = -g -Wall

rod_cutting: main.c
	gcc $(CFLAGS) -o main main.c
