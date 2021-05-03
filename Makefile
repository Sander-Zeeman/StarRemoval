# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

OBJS = \
  Connectivity.o \
  Heap.o \
  Image.o \
  main.o \
  MaxTree.o \
  Node.o \
  Pixel.o

INCLUDE_PATH = /usr/local/include/cfitsio
LIB_PATH = /usr/local/lib

LIBS = -lcfitsio

REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	echo All done

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^ -I $(INCLUDE_PATH) -L $(LIB_PATH) $(LIBS)

%.o : %.cpp
	g++ -g -o $@ -c $< -I $(INCLUDE_PATH) -L $(LIB_PATH) $(LIBS)

clean :
	rm -f $(REBUILDABLES)
	echo Clean done
