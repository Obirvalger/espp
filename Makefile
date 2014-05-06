CF = -std=c++11 -x c++ -c

main: main.o polynom.o multi_affine.o espp.o vector_operations.o
	g++ -std=c++11 main.o polynom.o multi_affine.o espp.o vector_operations.o -o main
main.o: main.cpp
	g++ $(CF) main.cpp
polynom.o: polynom.h polynom.hpp
	g++ $(CF) polynom.hpp
multi_affine.o: multi_affine.h multi_affine.hpp
	g++ $(CF) multi_affine.hpp
espp.o: espp.h espp.hpp
	g++ $(CF) espp.hpp
vector_operations.o: vector_operations.hpp vector_operations.h
	g++ $(CF) vector_operations.hpp
