#------------------------------------------------------------------------------#
#  Makefile from cbp                                                           #
#------------------------------------------------------------------------------#

WORKDIR = $(shell pwd)
INCLUDEDIR = $(shell pwd)/include

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -fopenmp -mavx2 -mfma -DNDEBUG -O3 -s -std=c++2a -I$(WORKDIR) -I$(INCLUDEDIR)
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

OBJ_COMPILE = $(OBJDIR_COMPILE)/include/alglib/alglibinternal.o $(OBJDIR_COMPILE)/include/alglib/alglibmisc.o $(OBJDIR_COMPILE)/include/alglib/ap.o $(OBJDIR_COMPILE)/include/alglib/kernels_avx2.o $(OBJDIR_COMPILE)/include/alglib/kernels_fma.o $(OBJDIR_COMPILE)/include/alglib/kernels_sse2.o $(OBJDIR_COMPILE)/include/alglib/linalg.o $(OBJDIR_COMPILE)/include/alglib/optimization.o $(OBJDIR_COMPILE)/include/alglib/solvers.o $(OBJDIR_COMPILE)/src/main.o

all: restart compile
clean: clean_compile
restart: restart_compile
rebuild: clean compile

before_compile:
	test -d $(OBJDIR_COMPILE)/ || mkdir -p $(OBJDIR_COMPILE)/
	test -d $(OBJDIR_COMPILE)/bin || mkdir -p $(OBJDIR_COMPILE)/bin
	test -d $(OBJDIR_COMPILE)/bin/Debug || mkdir -p $(OBJDIR_COMPILE)/bin/Debug
	test -d $(OBJDIR_COMPILE)/include || mkdir -p $(OBJDIR_COMPILE)/include
	test -d $(OBJDIR_COMPILE)/include/alglib || mkdir -p $(OBJDIR_COMPILE)/include/alglib
	test -d $(OBJDIR_COMPILE)/input || mkdir -p $(OBJDIR_COMPILE)/input
	test -d $(OBJDIR_COMPILE)/local_db || mkdir -p $(OBJDIR_COMPILE)/local_db
	test -d $(OBJDIR_COMPILE)/local_db/base || mkdir -p $(OBJDIR_COMPILE)/local_db/base
	test -d $(OBJDIR_COMPILE)/local_db/base/1 || mkdir -p $(OBJDIR_COMPILE)/local_db/base/1
	test -d $(OBJDIR_COMPILE)/local_db/base/16389 || mkdir -p $(OBJDIR_COMPILE)/local_db/base/16389
	test -d $(OBJDIR_COMPILE)/local_db/base/4 || mkdir -p $(OBJDIR_COMPILE)/local_db/base/4
	test -d $(OBJDIR_COMPILE)/local_db/base/5 || mkdir -p $(OBJDIR_COMPILE)/local_db/base/5
	test -d $(OBJDIR_COMPILE)/local_db/global || mkdir -p $(OBJDIR_COMPILE)/local_db/global
	test -d $(OBJDIR_COMPILE)/local_db/pg_commit_ts || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_commit_ts
	test -d $(OBJDIR_COMPILE)/local_db/pg_dynshmem || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_dynshmem
	test -d $(OBJDIR_COMPILE)/local_db/pg_logical || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_logical
	test -d $(OBJDIR_COMPILE)/local_db/pg_logical/mappings || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_logical/mappings
	test -d $(OBJDIR_COMPILE)/local_db/pg_logical/snapshots || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_logical/snapshots
	test -d $(OBJDIR_COMPILE)/local_db/pg_multixact || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_multixact
	test -d $(OBJDIR_COMPILE)/local_db/pg_multixact/members || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_multixact/members
	test -d $(OBJDIR_COMPILE)/local_db/pg_multixact/offsets || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_multixact/offsets
	test -d $(OBJDIR_COMPILE)/local_db/pg_notify || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_notify
	test -d $(OBJDIR_COMPILE)/local_db/pg_replslot || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_replslot
	test -d $(OBJDIR_COMPILE)/local_db/pg_serial || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_serial
	test -d $(OBJDIR_COMPILE)/local_db/pg_snapshots || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_snapshots
	test -d $(OBJDIR_COMPILE)/local_db/pg_stat || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_stat
	test -d $(OBJDIR_COMPILE)/local_db/pg_stat_tmp || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_stat_tmp
	test -d $(OBJDIR_COMPILE)/local_db/pg_subtrans || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_subtrans
	test -d $(OBJDIR_COMPILE)/local_db/pg_tblspc || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_tblspc
	test -d $(OBJDIR_COMPILE)/local_db/pg_twophase || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_twophase
	test -d $(OBJDIR_COMPILE)/local_db/pg_wal || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_wal
	test -d $(OBJDIR_COMPILE)/local_db/pg_wal/archive_status || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_wal/archive_status
	test -d $(OBJDIR_COMPILE)/local_db/pg_xact || mkdir -p $(OBJDIR_COMPILE)/local_db/pg_xact
	test -d $(OBJDIR_COMPILE)/obj || mkdir -p $(OBJDIR_COMPILE)/obj
	test -d $(OBJDIR_COMPILE)/obj/Debug || mkdir -p $(OBJDIR_COMPILE)/obj/Debug
	test -d $(OBJDIR_COMPILE)/obj/Debug/src || mkdir -p $(OBJDIR_COMPILE)/obj/Debug/src
	test -d $(OBJDIR_COMPILE)/obj/include || mkdir -p $(OBJDIR_COMPILE)/obj/include
	test -d $(OBJDIR_COMPILE)/obj/include/alglib || mkdir -p $(OBJDIR_COMPILE)/obj/include/alglib
	test -d $(OBJDIR_COMPILE)/obj/input || mkdir -p $(OBJDIR_COMPILE)/obj/input
	test -d $(OBJDIR_COMPILE)/obj/local_db || mkdir -p $(OBJDIR_COMPILE)/obj/local_db
	test -d $(OBJDIR_COMPILE)/obj/local_db/base || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/base
	test -d $(OBJDIR_COMPILE)/obj/local_db/base/1 || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/base/1
	test -d $(OBJDIR_COMPILE)/obj/local_db/base/16389 || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/base/16389
	test -d $(OBJDIR_COMPILE)/obj/local_db/base/4 || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/base/4
	test -d $(OBJDIR_COMPILE)/obj/local_db/base/5 || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/base/5
	test -d $(OBJDIR_COMPILE)/obj/local_db/global || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/global
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_commit_ts || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_commit_ts
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_dynshmem || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_dynshmem
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_logical || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_logical
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_logical/mappings || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_logical/mappings
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_logical/snapshots || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_logical/snapshots
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_multixact || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_multixact
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_multixact/members || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_multixact/members
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_multixact/offsets || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_multixact/offsets
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_notify || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_notify
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_replslot || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_replslot
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_serial || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_serial
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_snapshots || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_snapshots
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_stat || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_stat
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_stat_tmp || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_stat_tmp
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_subtrans || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_subtrans
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_tblspc || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_tblspc
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_twophase || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_twophase
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_wal || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_wal
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_wal/archive_status || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_wal/archive_status
	test -d $(OBJDIR_COMPILE)/obj/local_db/pg_xact || mkdir -p $(OBJDIR_COMPILE)/obj/local_db/pg_xact
	test -d $(OBJDIR_COMPILE)/obj/src || mkdir -p $(OBJDIR_COMPILE)/obj/src
	test -d $(OBJDIR_COMPILE)/src || mkdir -p $(OBJDIR_COMPILE)/src

after_compile:

compile: before_compile out_compile after_compile

out_compile: before_compile $(OBJ_COMPILE) $(DEP_COMPILE)
	$(LD) $(LIBDIR_COMPILE) -o $(OUT_COMPILE) $(OBJ_COMPILE) $(LDFLAGS_COMPILE) $(LIB_COMPILE)

$(OBJDIR_COMPILE)/include/alglib/alglibinternal.o: include/alglib/alglibinternal.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/alglibinternal.cpp -o $(OBJDIR_COMPILE)/include/alglib/alglibinternal.o

$(OBJDIR_COMPILE)/include/alglib/alglibmisc.o: include/alglib/alglibmisc.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/alglibmisc.cpp -o $(OBJDIR_COMPILE)/include/alglib/alglibmisc.o

$(OBJDIR_COMPILE)/include/alglib/ap.o: include/alglib/ap.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/ap.cpp -o $(OBJDIR_COMPILE)/include/alglib/ap.o

$(OBJDIR_COMPILE)/include/alglib/kernels_avx2.o: include/alglib/kernels_avx2.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/kernels_avx2.cpp -o $(OBJDIR_COMPILE)/include/alglib/kernels_avx2.o

$(OBJDIR_COMPILE)/include/alglib/kernels_fma.o: include/alglib/kernels_fma.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/kernels_fma.cpp -o $(OBJDIR_COMPILE)/include/alglib/kernels_fma.o

$(OBJDIR_COMPILE)/include/alglib/kernels_sse2.o: include/alglib/kernels_sse2.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/kernels_sse2.cpp -o $(OBJDIR_COMPILE)/include/alglib/kernels_sse2.o

$(OBJDIR_COMPILE)/include/alglib/linalg.o: include/alglib/linalg.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/linalg.cpp -o $(OBJDIR_COMPILE)/include/alglib/linalg.o

$(OBJDIR_COMPILE)/include/alglib/optimization.o: include/alglib/optimization.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/optimization.cpp -o $(OBJDIR_COMPILE)/include/alglib/optimization.o

$(OBJDIR_COMPILE)/include/alglib/solvers.o: include/alglib/solvers.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c include/alglib/solvers.cpp -o $(OBJDIR_COMPILE)/include/alglib/solvers.o

$(OBJDIR_COMPILE)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c src/main.cpp -o $(OBJDIR_COMPILE)/src/main.o

restart_compile:
	rm -f $(OUT_COMPILE)
	rm -rf $(OBJDIR_COMPILE)/src

clean_compile:
	rm -f $(OUT_COMPILE)
	rm -rf $(OBJDIR_COMPILE)

.PHONY: before_compile after_compile clean_compile
