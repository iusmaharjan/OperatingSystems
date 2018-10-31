## Compiler, tools and options
CC      = gcc
LINK    = gcc
OPT     = -g

CCFLAGS = $(OPT) -Wall
LDFLAGS = $(OPT)

## Libraries
LIBS = -lpthread
INC  = -I.

## FILES
OBJECTS1 = main.o do_thread.o producer_consumer.o semaphore.o ring_buffer.o
TARGET1  = test_producer_consumer

## Implicit rules
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CCFLAGS) $(INC) $<

## Build rules
all: $(TARGET1)

$(TARGET1): $(OBJECTS1)
	$(LINK) -o $@ $(OBJECTS1) $(LDFLAGS) $(LIBS)


clean:
	rm -f $(OBJECTS1) $(TARGET1)
	rm -f *~ core
