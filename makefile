#
# Makefile for the keysync program.
# 
# The interface for the program is written in Python
# 
# This program was written in C/C++ to test communication between Python and C/C++

#
# The processor that this computer is using
#
PROCESSOR = $(shell uname -m)

#
# Libraries 
# 	prefix LIBS with -l
# 	prefix LIBDIR with -L
#
LIBS :=  
LIBDIR := 

OPTIONS := -Wall -std=c++17

#
# Header files.
#
HEADERS := parallel.h

#
# Source objects (prefix with obj/)
#
OBJS := \
obj/parallel.o 

bin/keysync : $(OBJS)
	g++ $(OPTIONS) -o bin/parallel $(OBJS) $(LIBDIR) $(LIBS)

obj/parallel.o : parallel.cpp $(HEADERS)
	g++ -c $(OPTIONS) -o obj/parallel.o parallel.cpp

clean:
	-rm bin/parallel 2>/dev/null
	-rm $(OBJS) 2>/dev/null

