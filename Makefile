CC := gcc
#CC := /opt/homebrew/bin/x86_64-linux-musl-gcc

.PHONY: all clean

all:
	$(CC) -std=c11 -O3 -static -o nodetach nodetach.c

clean:
	$(RM) nodetach
