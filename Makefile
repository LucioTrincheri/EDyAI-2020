all: main

main: main.c stack.o queue.o dlist.o avltree.o
	gcc -std=c99 -Wall -Wextra -Werror -o main main.c avltree.o stack.o queue.o dlist.o -lm

avltree.o: avltree/avltree.c avltree/avltree.h stack.o queue.o dlist.o
	gcc -c -Wall -Wextra -Werror avltree/avltree.c

stack.o: avltree/lists/stack.c avltree/lists/stack.h dlist.o
	gcc -c -c -Wall -Wextra -Werror avltree/lists/stack.c

queue.o: avltree/lists/queue.c avltree/lists/queue.h dlist.o
	gcc -c -c -Wall -Wextra -Werror avltree/lists/queue.c

dlist.o: avltree/lists/dlist/dlist.c avltree/lists/dlist/dlist.h
	gcc -c -c -Wall -Wextra -Werror avltree/lists/dlist/dlist.c

clean:
	rm -f *.o main main.exe
