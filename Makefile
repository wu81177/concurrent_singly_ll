# Variables
CC = gcc
CFLAGS += --std=gnu99 -pedantic -Wall
CFLAGS += -fno-strict-aliasing
CFLAGS += -D_GNU_SOURCE
CFLAGS += -D_REENTRANT
CFLAGS += -I include
CFLAGS += -g
OBJFILES = singly_ll.o test.o
TARGET = test

# Rules
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

singly_ll.o: singly_ll.c singly_ll.h atomics.h
	$(CC) $(CFLAGS) -c singly_ll.c

test.o: test.c singly_ll.h atomics.h
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -f $(OBJFILES) $(TARGET)

.PHONY: all clean
