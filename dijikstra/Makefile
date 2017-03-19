SOURCE_DIRS = src
SRC = ./src
OBJ = ./obj
BIN = ./bin
BIN_SEARCH = $(addsuffix /*.c, src)
OBJ_SRC = $(patsubst src/%.c, obj/src/%.o, $(wildcard $(BIN_SEARCH)))

DEPENDS = $(OBJ_SRC:.o=.d)

.PHONY: make
make: dirs
	@$(MAKE) all
	
.PHONY: dirs
dirs:
	@mkdir -p $(dir $(OBJ_SRC))
	@mkdir -p $(BIN)

.PHONY: clean
clean:
	rm -r obj
	
.PHONY: all
all: $(BIN)/dij

$(BIN)/dij: $(OBJ_SRC)
	gcc $^ -o $@ -lm

-include $(DEPENDS)

$(OBJ)/src/%.o: $(SRC)/%.c
	gcc -c -Wall -g -O0 -MP -MMD $< -o $@
