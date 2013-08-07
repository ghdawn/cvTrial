#
#  EasyBMP Cross-Platform Windows Bitmap Library  
#                                                
#  Author: Paul Macklin                          
#   email: macklin01@users.sourceforge.net       
# support: http://easybmp.sourceforge.net        
#          file: makefile                
#    date added: 04-22-2006                      
# date modified: 12-01-2006                      
#       version: 1.06                            
#                                                
#   License: BSD (revised/modified)              
# Copyright: 2005-6 by the EasyBMP Project        
#                                                
# description: Sample makefile for compiling with
#              the EasyBMP library. This compiles
#              the EasyBMPsample.cpp program.
#

CC = g++
Output = bin/
# this line gives compiler optimizations that are geared towards g++ and Pentium4 
# computers. Comment it out if you don't have a Pentium 4 (or Athlon XP) or up

# CFLAGS = -O3 -Wno-deprecated -mcpu=pentium4 -march=pentium4 \
# -mfpmath=sse -msse -mmmx -msse2 -pipe -fomit-frame-pointer -s 

# Uncomment these two lines to use with any Pentium with MMX or up.

# CFLAGS = -Wno-deprecated -mcpu=pentium -march=pentium -pipe \
# -fomit-frame-pointer -mmmx -funroll-all-loops -s

# Uncomment these lines for some "safe" optimization flags

CFLAGS = -g -O1 -pipe -fomit-frame-pointer -funroll-all-loops -s 

objs = EasyBMP.o  ImageProcess.o main.o
main: $(objs)
	g++ $(CFLAGS) $(objs) -o $(Output)main
	$(Output)main

EasyBMP.o: lib/bmp/EasyBMP.cpp lib/bmp/EasyBMP*.h
	cp lib/bmp/EasyBMP*.h $(Output)
	cp lib/bmp/EasyBMP.cpp $(Output)
	g++ $(CFLAGS) -c $(Output)EasyBMP.cpp

ImageProcess.o: inc/ImageProcess.h src/ImageProcess.cpp
	cp inc/ImageProcess.h $(Output)
	cp src/ImageProcess.cpp $(Output)
	g++ $(CFLAGS) -c $(Output)ImageProcess.cpp
main.o: main.cpp
	cp main.cpp $(Output)
	g++ $(CFLAGS) -c $(Output)main.cpp

clean: 
	rm -f *.o
	rm $(Output)*.h
	rm $(Output)*.cpp
	
