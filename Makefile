# Compiler
CC = g++

# Debug flags
DEBUG = -g

# Compile flags
CFLAGS = -Wall -c $(DEBUG) -std=c++0x

# Linker flags
LFLAGS = -Wall $(DEBUG) -std=c++0x

# Target objects
OBJS = main.o cmdargmanager.o cmdargparser.o

#########################################

# Targets

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

clean:
	rm -f $(OBJS) main

#########################################

# Object dependencies

main.o : main.cc cmdargmanager.cc
	$(CC) $(CFLAGS) main.cc

cmdargmanager.o : cmdargmanager.cc cmdargmanager.h
	$(CC) $(CFLAGS) cmdargmanager.cc

cmdargparser.o : cmdargparser.cc cmdargparser.h
	$(CC) $(CFLAGS) cmdargparser.cc
