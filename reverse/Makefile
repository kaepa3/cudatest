COMPILER  = g++
main: main.cpp
	$(COMPILER) -std=c++17 main.cpp -o main 

dbg: main.cpp
	$(COMPILER) -g -O0 main.cpp -o main_debug 

clean:
	rm -f *.o main
