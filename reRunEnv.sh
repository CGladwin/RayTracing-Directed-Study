#!/bin/bash
sudo apt-get install python3
sudo apt-get install -y libxcb-cursor-dev


# This script allows for quick restarting of the virtual environment

# Remove the existing environment directory
rm -rf env

# Install virtualenv if it's not already installed
python3 -m venv env

# Activate the virtual environment
source env/bin/activate

# # Install the required packages
env/bin/pip install -r requirements.txt