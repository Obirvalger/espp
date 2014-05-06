#! /bin/bash
g++ -std=c++11 -x c++ -c vector_operations.hpp
g++ -std=c++11 -x c++ -c polynom.hpp
g++ -std=c++11 -x c++ -c multi_affine.hpp
g++ -std=c++11 -x c++ -c espp.hpp
g++ -std=c++11 -x c++ -c main.cpp
g++ -std=c++11 vector_operations.o polynom.o multi_affine.o espp.o main.o -o main 
./main
