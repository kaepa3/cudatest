COMPILER  = g++
INCLUDES = -I/usr/local/include/opencv4/
LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
main: main.cpp
	$(COMPILER) -std=c++17 main.cpp $(INCLUDES) $(LIBS) -o main 

clean:
	rm -f *.o main
