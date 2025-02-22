#!/bin/bash

# Exit on error
set -e

SCRIPT_DIR=$(dirname "$0")
INCLUDE_DIR="/usr/local/include/cbrew/"
CC=gcc

# Compiling cbrew
echo "Compiling cbrew..."
$CC -Wall -Wextra -Werror -O3 -I$SCRIPT_DIR/include/ $SCRIPT_DIR/src/main.c -o$SCRIPT_DIR/cbrew

# Installing cbrew
echo "Installing cbrew..."
sudo mv $SCRIPT_DIR/cbrew /usr/local/bin/

# Removing old cbrew headers
if [ -d "$INCLUDE_DIR" ]; then
    echo "Removing old cbrew headers..."
    sudo rm -rf "$INCLUDE_DIR"
fi

# Installing cbrew headers
echo "Installing cbrew headers..."
sudo mkdir -p "$INCLUDE_DIR"
sudo cp "$SCRIPT_DIR/include/"* "$INCLUDE_DIR"

echo "Successfully installed cbrew"
