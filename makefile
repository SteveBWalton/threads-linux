#
# Makefile for the parallel program.
# 
# Little program to test C++ threading in Linux.
# 

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
HEADERS := parallel.h thread_pool.h

#
# Source objects (prefix with obj/)
#
OBJS := \
obj/parallel.o \
obj/thread_pool.o

bin/parallel : $(OBJS)
	g++ $(OPTIONS) -o bin/parallel $(OBJS) $(LIBDIR) $(LIBS)

obj/parallel.o : parallel.cpp $(HEADERS)
	g++ -c $(OPTIONS) -o obj/parallel.o parallel.cpp

obj/thread_pool.o : thread_pool.cpp $(HEADERS)
	g++ -c $(OPTIONS) -o obj/thread_pool.o thread_pool.cpp

clean:
	-rm bin/parallel 2>/dev/null
	-rm $(OBJS) 2>/dev/null

