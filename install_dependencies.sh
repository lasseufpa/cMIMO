#!/bin/bash

echo "Updating package lists..."
sudo apt-get update

echo "Installing make..."
sudo apt-get install -y make

echo "Installing GSL..."
sudo apt-get install -y libgsl-dev

echo "All dependencies have been installed."
