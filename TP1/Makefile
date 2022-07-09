all: master personas

master: dlist.o datopersona.o main.c
	gcc -o master main.c dlist.o datopersona.o

datopersona.o: cabeceras/dlist.h cabeceras/datopersona.h datopersona.c
	gcc -c datopersona.c

dlist.o: dlist.c cabeceras/dlist.h
	gcc -c dlist.c

personas: personas.c cabeceras/personas.h
	gcc -o personas personas.c

clean:
	rm *.o
