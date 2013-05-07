LIB    = libglfw.a
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm -lXrandr

all: Midterm

Midterm: main.o AI.o Winner.o HTML.o
	g++ main.o AI.o Winner.o HTML.o -o Midterm $(LFLAGS)

main.o: main.cpp
	g++ -c main.cpp

AI.o: AI.cpp
	g++ -c AI.h AI.cpp

Winner.o: Winner.cpp
	g++ -c Winner.h Winner.cpp

HTML.o: HTML.cpp
	g++ -c HTML.h HTML.cpp

clean:
	rm -f *.o *.gch Midterm
	
	
