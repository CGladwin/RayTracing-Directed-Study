import os
import subprocess

def run(compile_command, shell=True, check=True):
    subprocess.run(compile_command, shell, check)

def compile_and_run_cpp(cpp_file):
    # Compile the C++ program
    run(f"g++ {cpp_file}.cpp -o {cpp_file}")
    
    # Execute the compiled program
    run(f"./{cpp_file}")

def run_python_script(py_file):
    # Run the Python script
    run(f"env/bin/python src/{py_file}.py")

def delete_executables():
    # Delete all executables in the directory
    for file in os.listdir("./src"):
        if os.path.isfile(file) and os.access(file, os.X_OK):
            os.remove(file)

if __name__ == "__main__":
    compile_and_run_cpp()
    run_python_script()
    delete_executables()
