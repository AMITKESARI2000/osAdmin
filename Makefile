CPPFLAGS = -std=c++17

all: main.cc
	g++ ${CPPFLAGS} main.cc -o main
	./main
