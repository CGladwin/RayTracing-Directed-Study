# Makefile

SRC_DIR = src
CPP_FILE = $(SRC_DIR)/main.cpp
EXECUTABLE = $(SRC_DIR)/main.out
OUTPUT = $(SRC_DIR)/images/output.png

# default target
all: compile_cpp run_cpp

# Compile the program with optimizations
compile_cpp:
	@g++ $(CPP_FILE) -o $(EXECUTABLE)

# run compiled code
run_cpp: compile_cpp
	@./$(EXECUTABLE) $(OUTPUT)

# Compile the program with debugging symbols and no optimizations
debug:
	@g++ -g -O0 $(CPP_FILE) -o $(EXECUTABLE)

# remove executable
clean:
	@rm -f $(EXECUTABLE) 

# remove png
png_clean:
	@rm -f $(OUTPUT) 

# Tells Make that these are not files
.PHONY: all compile_cpp run_cpp clean png_clean