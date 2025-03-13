import os
import re
import argparse

def find_todo_comments(directory):
    # Regular expression to match TODO comments
    todo_pattern = re.compile(r'//\s*TODO\s*:(.*)')

    # Walk through the directory recursively
    for root, _, files in os.walk(directory):
        for file in files:
            # Check if the file has a .hpp or .cpp extension
            if file.endswith(('.hpp', '.cpp')):
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8') as f:
                    # Read each line and check for TODO comments
                    for line_num, line in enumerate(f, start=1):
                        match = todo_pattern.search(line)
                        if match:
                            # Print the file path, line number, and the TODO comment
                            print(f"File: {file_path}, Line: {line_num}, TODO: {match.group(1).strip()}")

if __name__ == "__main__":
    # Set up argument parsing
    parser = argparse.ArgumentParser(description="Find TODO comments in .hpp and .cpp files recursively.")
    parser.add_argument("directory", help="The directory to search for TODO comments.")
    
    # Parse the arguments
    args = parser.parse_args()
    
    # Call the function to find TODO comments
    find_todo_comments(args.directory)

    