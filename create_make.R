library(rstudioapi)

dir = dirname(rstudioapi::getSourceEditorContext()$path)
setwd(dir)

### Output file
output <- c()
row_ID <- 0

## Header
row_ID <- row_ID + 1
output[row_ID] <- "#------------------------------------------------------------------------------#"
row_ID <- row_ID + 1
output[row_ID] <- "#  Makefile from cbp                                                           #"
row_ID <- row_ID + 1
output[row_ID] <- "#------------------------------------------------------------------------------#"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Working directory in shell
row_ID <- row_ID + 1
output[row_ID] <- "WORKDIR = $(shell pwd)"
row_ID <- row_ID + 1
output[row_ID] <- "SEARCHDIR = $(shell pwd)/lib"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler settings
row_ID <- row_ID + 1
output[row_ID] <- "CC = gcc"
row_ID <- row_ID + 1
output[row_ID] <- "CXX = g++"
row_ID <- row_ID + 1
output[row_ID] <- "AR = ar"
row_ID <- row_ID + 1
output[row_ID] <- "LD = g++"
row_ID <- row_ID + 1
output[row_ID] <- "WINDRES = windres"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler flags general
row_ID <- row_ID + 1
output[row_ID] <- "INC = "
row_ID <- row_ID + 1
output[row_ID] <- "CFLAGS = -Wall -fopenmp -mavx2 -mfma -DNDEBUG -O3 -s -std=c++2a -I$(WORKDIR) -I$(SEARCHDIR)"
row_ID <- row_ID + 1
output[row_ID] <- "RCFLAGS = "
row_ID <- row_ID + 1
output[row_ID] <- "RESINC = "
row_ID <- row_ID + 1
output[row_ID] <- "LIBDIR = "
row_ID <- row_ID + 1
output[row_ID] <- "LIB = "
row_ID <- row_ID + 1
output[row_ID] <- "LDFLAGS = -lpqxx -lpq -fopenmp -s"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler flags specific
row_ID <- row_ID + 1
output[row_ID] <- "INC_COMPILE = $(INC)"
row_ID <- row_ID + 1
output[row_ID] <- "CFLAGS_COMPILE = $(CFLAGS)"
row_ID <- row_ID + 1
output[row_ID] <- "RESINC_COMPILE = $(RESINC)"
row_ID <- row_ID + 1
output[row_ID] <- "RCFLAGS_COMPILE = $(RCFLAGS)"
row_ID <- row_ID + 1
output[row_ID] <- "LIBDIR_COMPILE = $(LIBDIR)"
row_ID <- row_ID + 1
output[row_ID] <- "LIB_COMPILE = $(LIB)"
row_ID <- row_ID + 1
output[row_ID] <- "LDFLAGS_COMPILE = $(LDFLAGS)"
row_ID <- row_ID + 1
output[row_ID] <- "OBJDIR_COMPILE = obj"
row_ID <- row_ID + 1
output[row_ID] <- "DEP_COMPILE = "
row_ID <- row_ID + 1
output[row_ID] <- "OUT_COMPILE = program"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler flags specific
src_files <- list.files(paste(dir, "/src", sep = ""), recursive = TRUE)
text_temp <- "OBJ_COMPILE ="
for(file_iter in 1:length(src_files)){
  if(strsplit(src_files[file_iter], ".cpp")[[1]][1] != src_files[file_iter]){
    text_temp <- paste(text_temp, " $(OBJDIR_COMPILE)/src/", src_files[file_iter], ".o", sep = "")
  }
}
row_ID <- row_ID + 1
output[row_ID] <- text_temp
row_ID <- row_ID + 1
output[row_ID] <- ""

## Define commands
row_ID <- row_ID + 1
output[row_ID] <- "all: compile"
row_ID <- row_ID + 1
output[row_ID] <- "clean: clean_compile"
row_ID <- row_ID + 1
output[row_ID] <- "refresh: clean compile"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Source directory list
src_dirs <- list.dirs(paste(dir, "/src", sep = ""), recursive = TRUE, full.names = FALSE)
for(dir_iter in 1:length(src_dirs)){
  src_dirs[dir_iter] <- paste("/src", src_dirs[dir_iter], sep = "")
}

## Before compile, make sure directories already exist
row_ID <- row_ID + 1
output[row_ID] <- "before_compile:"
for(dir_iter in 1:length(src_dirs)){
  text_temp <- "\ttest -d $(OBJDIR_COMPILE)"
  text_temp <- paste(text_temp, src_dirs[dir_iter], sep = "")
  text_temp <- paste(text_temp, " || mkdir -p $(OBJDIR_COMPILE)", sep = "")
  text_temp <- paste(text_temp, src_dirs[dir_iter], sep = "")
  
  row_ID <- row_ID + 1
  output[row_ID] <- text_temp
}
row_ID <- row_ID + 1
output[row_ID] <- ""

## After compile
row_ID <- row_ID + 1
output[row_ID] <- "after_compile:"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compile coomands
row_ID <- row_ID + 1
output[row_ID] <- "compile: before_compile out_compile after_compile"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Object compiling
row_ID <- row_ID + 1
output[row_ID] <- "out_compile: before_compile $(OBJ_COMPILE) $(DEP_COMPILE)"
row_ID <- row_ID + 1
output[row_ID] <- "\t$(LD) $(LIBDIR_COMPILE) -o $(OUT_COMPILE) $(OBJ_COMPILE) $(LDFLAGS_COMPILE) $(LIB_COMPILE)"
row_ID <- row_ID + 1
output[row_ID] <- ""

for(file_iter in 1:length(src_files)){
  if(strsplit(src_files[file_iter], ".cpp")[[1]][1] != src_files[file_iter]){
    text_temp <- "$(OBJDIR_COMPILE)/src/"
    text_temp <- paste(text_temp, src_files[file_iter], ".o: src/", src_files[file_iter], sep = "")
    row_ID <- row_ID + 1
    output[row_ID] <- text_temp    
    text_temp <- "\t$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c src/"
    text_temp <- paste(text_temp, src_files[file_iter], " -o ", sep = "")
    text_temp <- paste(text_temp, "$(OBJDIR_COMPILE)/src/", src_files[file_iter], ".o", sep = "")
    row_ID <- row_ID + 1
    output[row_ID] <- text_temp
    row_ID <- row_ID + 1
    output[row_ID] <- ""
  }
}

## Clean compile
row_ID <- row_ID + 1
output[row_ID] <- "clean_compile:"
row_ID <- row_ID + 1
output[row_ID] <- "\trm -f $(OUT_COMPILE)"
row_ID <- row_ID + 1
output[row_ID] <- "\trm -rf $(OBJDIR_COMPILE)"
row_ID <- row_ID + 1
output[row_ID] <- ""

row_ID <- row_ID + 1
output[row_ID] <- ".PHONY: before_compile after_compile clean_compile"

write(output, "make")