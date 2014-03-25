CFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)

tumor2d: tumor2d.c interface.o cell.o statistic.o
	gcc tumor2d.c interface.o cell.o statistic.o -o bin/tumor2d $(CFLAGS)

interface.o: interface.c
	gcc -c interface.c $(CFLAGS)

cell.o: cell.c
	gcc -c cell.c $(CFLAGS)

statistic.o: statistic.c
	gcc -c statistic.c $(CFLAGS)

clean:
	rm *.o

