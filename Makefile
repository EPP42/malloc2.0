CC= gcc 
CFLAGS= -g -shared -pedantic -std=c99 -fPIC 
LD_FLAGS= -fPIC   
SRC= malloc.c  
LIB= libmalloc.so 
VPATH= tests
LD_VISIBILITY= -fvisibility=hidden

all: libmalloc.so

$(LIB): $(SRC)
	LD_LIBRARY_PATH=./; $(CC) $(LD_VISIBILITY) $(CFLAGS) $(SRC) -o $@; LD_PRELOAD=./libmalloc.so  

main: main.c 
	gcc -g main.c -L. -lmalloc -o my_prog

clean: 
	@rm -rf $(OBJS)

mrproper: 
	@rm -rf $(LIB)

.PHONY: clean mrproper

