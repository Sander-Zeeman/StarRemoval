# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

FLAGS = \
  -O2 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion

OBJS = \
  Clusterer.o \
  Detector.o \
  Heap.o \
  Image.o \
  main.o \
  MaxTree.o \
  Timer.o

CFITSIO = -I /usr/local/include/cfitsio -lcfitsio
LIBS = -L /usr/local/lib

REBUILDABLES = $(addprefix out/, $(OBJS)) $(LINK_TARGET)

all : $(MKDIR_OUT) $(LINK_TARGET)

$(MKDIR_OUT) : mkdir out

$(LINK_TARGET) : $(addprefix out/, $(OBJS))
	g++ -o $@ $^ $(LIBS) $(CFITSIO)

out/%.o : src/%.cpp
	g++ $(FLAGS) -o $@ -c $< $(LIBS) $(CFITSIO)

clean :
	rm -f $(REBUILDABLES)
