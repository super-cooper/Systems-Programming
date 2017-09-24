

CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: hello rot128 diamond

hello: hello.c
	gcc $(CFLAGS) -o hello hello.c

rot128: rot128.c
	gcc $(CFLAGS) -o rot128 rot128.c

diamond: diamond.c
	gcc $(CFLAGS) -o diamond diamond.c

clean:
	rm -f hello rot128 diamond
