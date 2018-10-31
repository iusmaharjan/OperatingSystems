########################################
#
#  IUPUI CSCI 503 -- Operating Systems
#
########################################

## Compiler, tools and options
cc      = gcc

## FILES
OBJECTS = memblockallocator_ayush_os_lab1.o test
TARGET  = main_ayush_os_lab1

memblockallocator_ayush_os_lab1.o: memblockallocator_ayush_os_lab1.h
	cc -std=c99 -o memblockallocator_ayush_os_lab1.o -c memblockallocator_ayush_os_lab1.c

test: main_ayush_os_lab1.c memblockallocator_ayush_os_lab1.c
	cc -std=c99 -o test main_ayush_os_lab1.c memblockallocator_ayush_os_lab1.c

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f *~ core