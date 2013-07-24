src = $(wildcard *.c)
obj = $(patsubst %.c, %, $(src))

CC = gcc
CFLAGS = -g -lpthread

all:$(obj)
$(obj):%:%.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm $(obj)

.PHONY:clean

