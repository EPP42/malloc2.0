CC= gcc 
CFLAGS= -shared -pedantic -Wall -Werror -Wextra  -std=c99 -fPIC -I"include"
LD_FLAGS= -fPIC   
SRC= malloc.c tools.c   
LIB= libmalloc.so 
VPATH= tests
LD_VISIBILITY= -fvisibility=hidden

all: libmalloc.so

$(LIB): $(SRC)
	@$(CC) $(LD_VISIBILITY) $(CFLAGS) $(SRC) -o $@

check: all
	@cp -f  $(LIB) tests;cd tests; LD_PRELOAD=./$(LIB); LD_LIBRARY_PATH=.; ./tests_suit.sh;   
clean: 
	@rm -rf $(LIB)

.PHONY: clean

