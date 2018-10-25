CFLAGS=-g3 -ggdb

#Location of the libraries, specifically libGL and libglut.
LIBS=

#Location of the include files, specifically the GL/ folder and the GL/ folder that contains glut, if they are different.
INCLUDES=

all:
	$(CXX) $(CFLAGS) -c -o mandelbrot_work.o mandelbrot.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) -c -o color.o color.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) -o mandelbrot main.cpp mandelbrot_work.o color.o  $(INCLUDES) $(LIBS) -lGL -lglut -lm -lpthread
	$(CXX) $(CFLAGS) -o julia julia_sets.cpp mandelbrot_work.o color.o $(INCLUDES) $(LIBS) -lGL -lglut -lm -lpthread
	
clean:
	rm -rf *.o
	rm -rf *.obj