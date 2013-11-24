CFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)

tumor2d: tumor2d.c interface.o cell.o
	gcc tumor2d.c interface.o cell.o -o bin/tumor2d $(CFLAGS)

interface.o: interface.c
	gcc -c interface.c $(CFLAGS)

cell.o: cell.c
	gcc -c cell.c $(CFLAGS)

clean:
	rm *.o

