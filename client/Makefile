TARGET=a.out
CC=gcc
CFLAGS=-g -O0
LDFLAGS=
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o $(TARGET)

