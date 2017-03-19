VPATH := src
BIN := ./bin
OBJ := ./obj
search := $(addsuffix /*.c, src)

./bin/rbtree: $(patsubst src/%.c, obj/%.o, $(wildcard $(search)))
ifeq ( , $(wildcard $(BIN)))
		mkdir bin
endif
		gcc -o ./bin/rbtree $^

./obj/%.o: %.c
		mkdir -p obj
		gcc -c -Wall -g -O0 $^ -o $@

.PHONY: clean

clean:
		rm -r bin
		rm -r obj
