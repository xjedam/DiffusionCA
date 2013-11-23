CFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)

tumor2d: tumor2d.c buttons.o cell.o
	gcc tumor2d.c buttons.o cell.o -o bin/tumor2d $(CFLAGS)

buttons.o: buttons.c
	gcc -c buttons.c $(CFLAGS)

cell.o: cell.c
	gcc -c cell.c $(CFLAGS)

clean:
	rm *.o

