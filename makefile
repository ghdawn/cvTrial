
CC = g++
Output = bin/

CFLAGS = -g

objs =Util.o GrayBMP.o EasyBMP.o  Filter.o ImageProcess.o main.o


main: $(objs)
	$(CC) -o $(Output)main $(CFLAGS) $(objs) 
	./$(Output)main

Filter.o: inc/Filter.h src/Filter.cpp inc/Vector.h
	cp inc/Filter.h $(Output)
	cp src/Filter.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)Filter.cpp

Util.o: src/Util.cpp
	cp src/Util.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)Util.cpp
	
GrayBMP.o: inc/GrayBMP.h src/GrayBMP.cpp inc/Vector.h
	cp inc/GrayBMP.h $(Output)
	cp inc/Vector.h $(Output)
	cp src/GrayBMP.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)GrayBMP.cpp

EasyBMP.o: lib/bmp/EasyBMP.cpp lib/bmp/EasyBMP*.h
	cp lib/bmp/EasyBMP*.h $(Output)
	cp lib/bmp/EasyBMP.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)EasyBMP.cpp

ImageProcess.o: inc/ImageProcess.h src/ImageProcess.cpp
	cp inc/ImageProcess.h $(Output)
	cp src/ImageProcess.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)ImageProcess.cpp

main.o: main.cpp inc/Vector.h inc/Matrix.h
	cp inc/*.h $(Output)
	cp main.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)main.cpp

clean: 
	rm -f *.o
	rm -f *.bmp
	rm $(Output)*.h
	rm $(Output)*.cpp
	
