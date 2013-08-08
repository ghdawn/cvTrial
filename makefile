
CC = g++
Output = bin/

CFLAGS = -g -O1

objs = Util.o GrayBMP.o EasyBMP.o  ImageProcess.o main.o


main: $(objs)
	$(CC) -o $(Output)main $(CFLAGS) $(objs) 
	./$(Output)main
Util.o: src/Util.cpp
	cp src/Util.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)Util.cpp
	
GrayBMP.o: inc/GrayBMP.h src/GrayBMP.cpp
	cp inc/GrayBMP.h $(Output)
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

main.o: main.cpp
	cp main.cpp $(Output)
	$(CC) $(CFLAGS) -c $(Output)main.cpp

clean: 
	rm -f *.o
	rm -f *.bmp
	rm $(Output)*.h
	rm $(Output)*.cpp
	
