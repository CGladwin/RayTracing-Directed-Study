# Makefile

SRC_DIR = src
CPP_FILE = $(SRC_DIR)/main.cpp
EXECUTABLE = $(SRC_DIR)/main.out
OUTPUT = $(SRC_DIR)/images/output.png
INPUT = example.json

# default target
all: compile_cpp run_cpp clean

total: compile_cpp run_cpp clean doxy

# Compile the program with optimizations
compile_cpp:
	@g++ $(CPP_FILE) -o $(EXECUTABLE)

# run compiled code
run_cpp: compile_cpp
	@./$(EXECUTABLE) $(OUTPUT) $(INPUT)

# Compile the program with debugging symbols and no optimizations
debug:
	@g++ -g -O0 $(CPP_FILE) -o $(EXECUTABLE)
	@echo "Debug build complete. Use a debugger (e.g., gdb) to run the program."

# remove executable
clean:
	@rm -f $(EXECUTABLE) 

# remove png
png_clean:
	@rm -f $(OUTPUT) 

# recompile doxygen
doxy:
	@doxygen $(SRC_DIR)/doxyfile

perf: compile_cpp
	@valgrind --tool=callgrind --callgrind-out-file=callgrind.out $(EXECUTABLE)
	@python3 ./gprof2dot.py -f callgrind callgrind.out | dot -Tsvg -o output.svg
	@rm -rf callgrind.out
	$(MAKE) clean

todo: 
	@python3 ./todo_finder.py ./

# Tells Make that these are not files
.PHONY: all compile_cpp run_cpp clean png_clean doxy perf