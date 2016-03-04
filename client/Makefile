TARGET = a.out

CC = gcc
CFLAGS = -O3 -Wall -Wno-unused-result
LDFLAGS =
RM = rm -rf
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) *.o $(TARGET)

