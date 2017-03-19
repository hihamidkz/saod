bin/task1: obj/task1.o obj/bstree.o obj/hashtab.o
	gcc -o $@ $^

bin/task2: obj/task2.o obj/bstree.o obj/hashtab.o
	gcc -o $@ $^
	
bin/task3: obj/task3.o obj/hashtab.o
	gcc -o $@ $^
	
gdb: obj/task3.o obj/hashtab.o
	gcc -g -O0 -o $@ $^
	
obj/task1.o: task1.c
	gcc -Wall -g -O0 -c $^ -o $@
	
obj/task2.o: task2.c	
	gcc -Wall -g -O0 -c $^ -o $@
	
obj/task3.o: task3.c
	gcc -Wall -g -O0 -c $^ -o $@

obj/bstree.o: bstree.c
	gcc -Wall -g -O0 -c $^ -o $@
	
obj/hashtab.o: hashtab.c
	gcc -Wall -g -O0 -c $^ -o $@

