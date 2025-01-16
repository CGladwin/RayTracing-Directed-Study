import os
import subprocess

def run(command):
    subprocess.run(command, shell=True, check=True)


def compile_and_run_cpp(src_dir,cpp_file_path):
    joined_dir = os.path.join(src_dir,cpp_file_path)
    # Compile the C++ program
    run(f"g++ {joined_dir}.cpp -o {joined_dir}")
    
    # Execute the compiled program
    run(f"./{joined_dir} {src_dir}/images/{cpp_file_path}.ppm")

def run_python_script(py_file_path):
    # Run the Python script
    run(f"env/bin/python {py_file_path}.py")

def delete_executables(src_dir = "src"):
    # Delete all executables in the src directory
    for file in os.listdir(src_dir):
        file_path = os.path.join(src_dir, file)
        if os.path.isfile(file_path) and os.access(file_path, os.X_OK):
            os.remove(file_path)

if __name__ == "__main__":
    src_dir = "src"
    cpp_file = "first_image"
    py_file = "PPM_image_output"
    compile_and_run_cpp(src_dir,cpp_file)
    run_python_script(os.path.join(src_dir,py_file))
    delete_executables()
