course: course.cpp course.o
	g++ -std=c++11 course.cpp course.o -o course
course.o: course.hpp polynom.o multi_affine.o espp.o vector_operations.o
	g++ -std=c++11 course.hpp polynom.o multi_affine.o espp.o vector_operations.o -o course.o
polynom.o: polynom.h polynom.hpp
	g++ -std=c++11 polynom.h polynom.hpp -o polynom.o
multi_affine.o: multi_affine.h multi_affine.hpp
	g++ -std=c++11 multi_affine.h multi_affine.hpp -o multi_affine.o
espp.o: espp.h espp.hpp
	g++ -std=c++11 espp.h espp.hpp -o espp.o
vector_operations.o: vector_operations.hpp
	g++ -std=c++11 vector_operations.hpp -o vector_operations.o
