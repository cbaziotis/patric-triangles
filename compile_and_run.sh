#!/bin/bash

echo "compiling..."
mpicxx main.cpp auxiliary.cpp -o triangles -std=c++11
echo "done!"
echo 
echo "executing..."
mpirun -np 4 ./triangles 
