# Basic build process
# 1. compiler produces an object file (.o or .obj) for each source file (.c)
# 2. linker takes the object files and creates an executable
# (gcc is both a compiler and a linker)
#
# The Makefile contains a list of rules for creating the executable.
# make takes care of recompiling/linking, if a dependency has changed.
# If a dependency is newer than the target, 
# then the target is out-of-date and must be rebuilt.
# 
# Basic Makefile rule syntax:
# target: dependencies
# [tab] command
#
# Variables are defined as:
# VAR = value
# Variables are referenced as:
# $(VAR)
#
# Automatic variables:
# $@ is the file name of the target of the rule
# $< is the name of the first dependency of the rule
# $^ is the list of all dependencies of the rule
# $? is the list of dependencies that are newer than the target
#
# Substitution references:
# OBJS = $(SRCS:.c=.o) 
# example: if $SRCS is "foo.c bar.c" then $OBJS will be "foo.o bar.o"  
# http://www.gnu.org/software/make/manual/make.html#Substitution-Refs
#
# GNU make documentation
# http://www.gnu.org/software/make/manual/make.html
#
# GCC options
# http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html#Option-Summary

CC = gcc
CFLAGS = -Wall # all warnings on, could also add: -pedantic
DEBUG = -g      # produce debugging information
LINKER = gcc
# Windows:
#LDFLAGS = -lws2_32     # -lwsock32
# OS X, Linux:
LDFLAGS =

EXECUTABLE = spreadsheet
SRCS = main.c Gui.c Object.c Formula.c List.c ObjectFunctions.c Parser.c
OBJS = $(SRCS:.c=.o)

# disable old-fashioned suffix rules
.SUFFIXES:      # delete all default suffixes

# pattern rule to produce object files from c files
%.o: %.c
	@echo "$@ is older than these dependencies: $?"
	@echo "Compiling $< to $@:"
	$(CC) -c $(CFLAGS) $(DEBUG) $<
	$(CC) -MM $< > $(<:.c=.d)
#@todo: check pattern for Windows

# all is the first target here (not a special target starting with a ".")
# the first target is invoked when "make" is called without any arguments
$(EXECUTABLE): $(OBJS)
	@echo "$@ is older than these dependencies: $?"
	@echo "Linking $(OBJS) to $@:"
	$(LINKER) -o $(EXECUTABLE) $(OBJS) $(LDFLAGS)

# list of dependencies (aka. prerequisites)
# main.o: main.c main.h Object.h List.h Formula.h Parser.h Gui.h
# Gui.o: Gui.c Gui.h
# Object.o: Object.c Object.h List.h
# List.o: List.c List.h Object.h
# Formula.o: Formula.c Formula.h Object.h List.h
# Parser.o: Parser.c Parser.h Object.h List.h Formula.h

-include $(OBJS:.o=.d)

# automatically create the above list of dependencies (gcc option -MM or -M)
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
# http://scottmcpeak.com/autodepend/autodepend.html
# http://mad-scientist.net/make/autodep.html
.PHONY: depend   # do not treat "depend" as a file name
depend:
	$(CC) -MM *.c

# remove produced files, invoke as "make clean"
.PHONY: clean   # do not treat "clean" as a file name
clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)
	rm -f $(SRCS:.c=.d)
