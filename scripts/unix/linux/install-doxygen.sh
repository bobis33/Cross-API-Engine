#!/bin/bash
set -e

version=1.11.0
platform=linux

wget https://www.doxygen.nl/files/doxygen-$version.$platform.bin.tar.gz && tar xf doxygen-$version.$platform.bin.tar.gz
tar xf doxygen-$version.$platform.bin.tar.gz
sudo cp doxygen-$version/bin/doxygen /usr/local/bin/
