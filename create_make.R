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
output[row_ID] <- "ENVDIR = <path to the conda environment>"
row_ID <- row_ID + 1
output[row_ID] <- "WORKDIR = $(shell pwd)"
row_ID <- row_ID + 1
output[row_ID] <- "INCLUDEDIR = $(shell pwd)/include"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler settings
row_ID <- row_ID + 1
output[row_ID] <- "CC = $(ENVDIR)/bin/gcc"
row_ID <- row_ID + 1
output[row_ID] <- "CXX = $(ENVDIR)/bin/g++"
row_ID <- row_ID + 1
output[row_ID] <- "AR = $(ENVDIR)/bin/ar"
row_ID <- row_ID + 1
output[row_ID] <- "LD = $(ENVDIR)/bin/g++"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Compiler flags general
row_ID <- row_ID + 1
output[row_ID] <- "INC = "
row_ID <- row_ID + 1
output[row_ID] <- "CFLAGS = -Wall -fopenmp -mavx2 -mfma -DNDEBUG -O3 -s -std=c++2a -I$(WORKDIR) -I$(INCLUDEDIR) -I$(ENVDIR)/include/"
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
sub_dir_name <- c("include", "src")
src_files <- c()
for(sub_dir_iter in 1:length(sub_dir_name)){
  src_files_temp <- list.files(paste(dir, "/", sub_dir_name[sub_dir_iter], sep = ""), recursive = TRUE)
  src_files_temp <- paste(sub_dir_name[sub_dir_iter], "/", src_files_temp, sep = "")
  
  src_files <- c(src_files, src_files_temp)
}
text_temp <- "OBJ_COMPILE ="
for(file_iter in 1:length(src_files)){
  if(strsplit(src_files[file_iter], ".cpp")[[1]][1] != src_files[file_iter]){
    obj_name_temp = strsplit(src_files[file_iter], ".cpp")[[1]][1]
    text_temp <- paste(text_temp, " $(OBJDIR_COMPILE)/", obj_name_temp, ".o", sep = "")
  }else if(strsplit(src_files[file_iter], "\\.c")[[1]][1] != src_files[file_iter]){
    obj_name_temp = strsplit(src_files[file_iter], ".c")[[1]][1]
    text_temp <- paste(text_temp, " $(OBJDIR_COMPILE)/", obj_name_temp, ".o", sep = "")
  }
}
row_ID <- row_ID + 1
output[row_ID] <- text_temp
row_ID <- row_ID + 1
output[row_ID] <- ""

## Define commands
row_ID <- row_ID + 1
output[row_ID] <- "all: restart compile"
row_ID <- row_ID + 1
output[row_ID] <- "clean: clean_compile"
row_ID <- row_ID + 1
output[row_ID] <- "restart: restart_compile"
row_ID <- row_ID + 1
output[row_ID] <- "rebuild: clean compile"
row_ID <- row_ID + 1
output[row_ID] <- ""

## Source directory list
sub_dir_name <- c("include", "src")
src_dirs <- c()
for(sub_dir_iter in 1:length(sub_dir_name)){
  src_dir_temp <- list.dirs(paste(dir, "/", sub_dir_name[sub_dir_iter], sep = ""), recursive = TRUE, full.names = FALSE)
  
  for(temp_dir_iter in 1:length(src_dir_temp)){
    src_dir_temp[temp_dir_iter] <- paste(sub_dir_name[sub_dir_iter], "/", src_dir_temp[temp_dir_iter], sep = "")
  }
  
  src_dirs <- c(src_dirs, src_dir_temp)
}

## Before compile, make sure directories already exist
row_ID <- row_ID + 1
output[row_ID] <- "before_compile:"
for(dir_iter in 1:length(src_dirs)){
  text_temp <- "\ttest -d $(OBJDIR_COMPILE)/"
  text_temp <- paste(text_temp, src_dirs[dir_iter], sep = "")
  text_temp <- paste(text_temp, " || mkdir -p $(OBJDIR_COMPILE)/", sep = "")
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
    obj_name_temp <- strsplit(src_files[file_iter], ".cpp")[[1]][1]
    text_temp <- "$(OBJDIR_COMPILE)"
    text_temp <- paste(text_temp, "/", obj_name_temp, ".o: ", src_files[file_iter], sep = "")
    row_ID <- row_ID + 1
    output[row_ID] <- text_temp    
    text_temp <- "\t$(CXX) $(CFLAGS_COMPILE) $(INC_COMPILE) -c "
    text_temp <- paste(text_temp, src_files[file_iter], " -o ", sep = "")
    text_temp <- paste(text_temp, "$(OBJDIR_COMPILE)/", obj_name_temp, ".o", sep = "")
    row_ID <- row_ID + 1
    output[row_ID] <- text_temp
    row_ID <- row_ID + 1
    output[row_ID] <- ""
  }
}

## Clean only src dir
row_ID <- row_ID + 1
output[row_ID] <- "restart_compile:"
row_ID <- row_ID + 1
output[row_ID] <- "\trm -f $(OUT_COMPILE)"
row_ID <- row_ID + 1
output[row_ID] <- "\trm -rf $(OBJDIR_COMPILE)/src"
row_ID <- row_ID + 1
output[row_ID] <- ""

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