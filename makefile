
CC = g++
Output = bin/

CFLAGS = -g -Iinc/

Obj = Obj/
objs = $(Obj)GrayBMP.o $(Obj)Draw.o  $(Obj)EasyBMP.o  $(Obj)Filter.o $(Obj)ImageProcess.o $(Obj)main.o

all: $(Obj)main

$(Obj)main: $(objs)
	$(CC) -o $(Output)main $(CFLAGS) $(objs) 

$(Obj)Filter.o: inc/Filter.h src/Filter.cpp inc/Vector.h
	$(CC) $(CFLAGS) -c src/Filter.cpp -o $(Obj)Filter.o

	
$(Obj)Draw.o: src/Draw.cpp inc/Draw.h
	$(CC) $(CFLAGS) -c src/Draw.cpp -o $(Obj)Draw.o
	
$(Obj)GrayBMP.o: inc/GrayBMP.h src/GrayBMP.cpp inc/Vector.h
	$(CC) $(CFLAGS) -c src/GrayBMP.cpp -o $(Obj)GrayBMP.o

$(Obj)EasyBMP.o: src/EasyBMP.cpp inc/EasyBMP*.h
	$(CC) $(CFLAGS) -c src/EasyBMP.cpp -o $(Obj)EasyBMP.o
	
$(Obj)ImageProcess.o: inc/ImageProcess.h src/ImageProcess.cpp
	$(CC) $(CFLAGS) -c src/ImageProcess.cpp -o $(Obj)ImageProcess.o

$(Obj)main.o: main.cpp inc/Vector.h inc/Matrix.h
	$(CC) $(CFLAGS) -c main.cpp -o $(Obj)main.o

clean: 
	rm -f *.o
	rm -f *.bmp
	rm $(Output)*.h
	rm $(Output)*.cpp
	
