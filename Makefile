# Compiler
CC = g++

# Debug flags
DEBUG = -g

# Compile flags
CFLAGS = -Wall -c $(DEBUG) -std=c++0x

# Linker flags
LFLAGS = -Wall $(DEBUG) -std=c++0x

# Target objects
OBJS = main.o CommandArguments.o

#########################################

# Targets

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

clean:
	rm -f $(OBJS) main

#########################################

# Object dependencies

main.o : main.cc CommandArguments.cc
	$(CC) $(CFLAGS) main.cc

CommandArguments.o : CommandArguments.cc CommandArguments.h
	$(CC) $(CFLAGS) CommandArguments.cc
