all: ford_falk

ford_falk: main.cpp fulkerson.hpp fulkerson.cpp
	clang++ -g -Wall -Wextra -std=c++17 -o fulkerson  main.cpp fulkerson.cpp		
