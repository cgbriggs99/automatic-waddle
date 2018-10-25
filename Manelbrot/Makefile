CFLAGS=-g3 -ggdb

all:
	$(CXX) $(CFLAGS) -c -o mandelbrot_work.o mandelbrot.cpp
	$(CXX) $(CFLAGS) -c -o color.o color.cpp
	g++ $(CFLAGS) -o mandelbrot main.cpp mandelbrot_work.o color.o -lGL -lglut -lm -lpthread