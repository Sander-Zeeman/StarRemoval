# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

OBJS = \
  out/Connectivity.o \
  out/Detector.o \
  out/Heap.o \
  out/Image.o \
  out/main.o \
  out/MaxTree.o \
  out/Node.o \
  out/Pixel.o \
  out/Timer.o

CFITSIO = -I /usr/local/include/cfitsio -lcfitsio
LIBS = -L /usr/local/lib


REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^ $(LIBS) $(CFITSIO)

out/%.o : src/%.cpp
	g++ -g -O2 -Wall -o $@ -c $< $(LIBS) $(CFITSIO)

clean :
	rm -f $(REBUILDABLES)