#!/bin/bash
# HilMa		Hilbert Mapper
mkdir tmp
cd tmp
cmake ..
make
mv HilMa ../
cd ../
rm -rf ./tmp
