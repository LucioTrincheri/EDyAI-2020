all: main

main: main.c stack.o queue.o dlist.o avltree.o
	gcc -Wall -Wextra -Werror -o main main.c avltree.o stack.o queue.o dlist.o -lm

avltree.o: avltree.c avltree.h stack.o queue.o dlist.o
	gcc -c -Wall -Wextra -Werror avltree.c

stack.o: lists/stack.c lists/stack.h dlist.o
	gcc -c -c -Wall -Wextra -Werror lists/stack.c

queue.o: lists/queue.c lists/queue.h dlist.o
	gcc -c -c -Wall -Wextra -Werror lists/queue.c

dlist.o: lists/dlist/dlist.c lists/dlist/dlist.h
	gcc -c -c -Wall -Wextra -Werror lists/dlist/dlist.c

clean:
	rm -f *.o main main.exe
