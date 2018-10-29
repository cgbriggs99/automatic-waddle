CFLAGS=-g3 -ggdb -static-libstdc++ -static-libgcc -static

#Location of the libraries, specifically libGL and libglut.
LIBS=-L C:\\freeglut\\lib\\x64

#Location of the include files, specifically the GL/ folder and the GL/ folder that contains glut, if they are different.
INCLUDES=-I C:\\mingw64\\mingw64\\x86_64-w64-mingw32\\include -I C:\\freeglut\\include

all:
	$(CXX) $(CFLAGS) -c -o mandelbrot_work.o mandelbrot.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) -c -o color.o color.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) -o mandelbrot main.cpp mandelbrot_work.o color.o  $(INCLUDES) $(LIBS) -lopengl32 -lfreeglut -lm -lpthread
	$(CXX) $(CFLAGS) -o julia julia_sets.cpp mandelbrot_work.o color.o $(INCLUDES) $(LIBS) -lopengl32 -lfreeglut -lm -lpthread
	
clean:
	rm -rf *.o
	rm -rf *.obj