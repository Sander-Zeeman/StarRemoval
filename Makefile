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
  out/Pixel.o

INCLUDE_PATH = /usr/local/include/cfitsio
LIB_PATH = /usr/local/lib

LIBS = -lcfitsio

REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^ #-I $(INCLUDE_PATH) -L $(LIB_PATH) $(LIBS)

out/%.o : src/%.cpp
	g++ -g -O2 -Wall -o $@ -c $< #-I $(INCLUDE_PATH) -L $(LIB_PATH) $(LIBS)

clean :
	rm -f $(REBUILDABLES)