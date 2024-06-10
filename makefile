#------------------------------------------------------------------------------#
#  Makefile from cbp                                                           #
#------------------------------------------------------------------------------#

WORKDIR = $(shell pwd)
SEARCHDIR = $(shell pwd)/lib

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -fopenmp -mavx2 -mfma -DNDEBUG -O3 -s -std=c++2a -I$(WORKDIR) -I$(SEARCHDIR)
RCFLAGS = 
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lpqxx -lpq -fopenmp -s

INC_COMPILE = $(INC)
CFLAGS_COMPILE = $(CFLAGS)
RESINC_COMPILE = $(RESINC)
RCFLAGS_COMPILE = $(RCFLAGS)
LIBDIR_COMPILE = $(LIBDIR)
LIB_COMPILE = $(LIB)
LDFLAGS_COMPILE = $(LDFLAGS)
OBJDIR_COMPILE = obj
DEP_COMPILE = 
OUT_COMPILE = program

OBJ_COMPILE = $(OBJDIR_COMPILE)/src/main.cpp.o

all: compile
clean: clean_compile
refresh: clean compile

before_compile:
	test -d $(OBJDIR_COMPILE)/src || mkdir -p $(OBJDIR_COMPILE)/src

after_compile:

compile: before_compile out_compile after_compile

out_compile: before_compile $(OBJ_COMPILE) $(DEP_COMPILE)
	$(LD) $(LIBDIR_COMPILE) -o $(OUT_COMPILE) $(OBJ_COMPILE) $(LDFLAGS_COMPILE) $(LIB_COMPILE)

$(OBJDIR_COMPILE)/src/main.cpp.o: src/main.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c src/main.cpp -o $(OBJDIR_COMPILE)/src/main.cpp.o

clean_compile:
	rm -f $(OUT_COMPILE)
	rm -rf $(OBJDIR_COMPILE)

.PHONY: before_compile after_compile clean_compile
