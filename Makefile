tumor2d: tumor2d.c
	gcc tumor2d.c -o bin/tumor2d `pkg-config --cflags --libs gtk+-3.0`