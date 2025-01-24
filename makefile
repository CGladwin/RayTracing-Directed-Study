# Makefile

SRC_DIR = src
CPP_FILE = $(SRC_DIR)/first_image.cpp
EXECUTABLE = $(SRC_DIR)/first_image
PYTHON_SCRIPT = $(SRC_DIR)/PPM_image_output.py

all: compile_cpp run_cpp run_python clean

compile_cpp:
    @g++ $(CPP_FILE) -o $(EXECUTABLE)

run_cpp: compile_cpp
    @./$(EXECUTABLE)

run_python: run_cpp
    @python $(PYTHON_SCRIPT)

clean:
    @find $(SRC_DIR) -type f -perm -u=x -exec rm -f {} +

.PHONY: all compile_cpp run_cpp run_python clean
