# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

OBJS = \
  Detector.o \
  Heap.o \
  Image.o \
  main.o \
  MaxTree.o \
  Timer.o

CFITSIO = -I /usr/local/include/cfitsio -lcfitsio
LIBS = -L /usr/local/lib

REBUILDABLES = $(addprefix out/, $(OBJS)) $(LINK_TARGET)

all : $(LINK_TARGET)

$(LINK_TARGET) : $(addprefix out/, $(OBJS))
	g++ -g -o $@ $^ $(LIBS) $(CFITSIO)

out/%.o : src/%.cpp
	g++ -g -O2 -Wall -o $@ -c $< $(LIBS) $(CFITSIO)

clean :
	rm -f $(REBUILDABLES)
