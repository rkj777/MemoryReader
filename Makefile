CC = gcc
CFLAGS = -m32


run: build
	./a.out

build:
	$(CC) $(CFLAGS) test.c memchunk.c

tar:
	tar cvf submit.tar *.c *.h Makefile 


