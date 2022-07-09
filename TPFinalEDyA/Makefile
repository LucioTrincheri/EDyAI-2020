all: main

main: main.c stack.o queue.o dlist.o avltree.o hash.o
	gcc -g -Wall -Wextra -Werror -std=c99 -o main main.c hash.o avltree.o stack.o queue.o dlist.o -lm

hash.o: hash/hash.c hash/hash.h
	gcc -c -Wall -Wextra -Werror -std=c99 hash/hash.c

avltree.o: avltree/avltree.c avltree/avltree.h stack.o queue.o dlist.o
	gcc -c -Wall -Wextra -Werror -std=c99 avltree/avltree.c

stack.o: avltree/lists/stack.c avltree/lists/stack.h dlist.o
	gcc -c -c -Wall -Wextra -Werror -std=c99 avltree/lists/stack.c

queue.o: avltree/lists/queue.c avltree/lists/queue.h dlist.o
	gcc -c -c -Wall -Wextra -Werror -std=c99 avltree/lists/queue.c

dlist.o: avltree/lists/dlist/dlist.c avltree/lists/dlist/dlist.h
	gcc -c -c -Wall -Wextra -Werror -std=c99 avltree/lists/dlist/dlist.c 

clean:
	rm -f *.o main main.exe
