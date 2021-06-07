# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

FLAGS = \
  -O2 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion

OBJS = \
  BGFunctions.o \
  Clusterer.o \
  Detector.o \
  Heap.o \
  Image.o \
  main.o \
  MaxTree.o \
  PostProcessing.o \
  Timer.o

CFITSIO = -I /usr/local/include/cfitsio -lcfitsio
BOOST = -I /usr/local/include/boost

LIBS = -L /usr/local/lib

REBUILDABLES = $(addprefix out/, $(OBJS)) $(LINK_TARGET)

all : $(LINK_TARGET)

$(LINK_TARGET) : $(addprefix out/, $(OBJS))
	g++ -o $@ $^ $(LIBS) $(CFITSIO) $(BOOST)

out/%.o : src/%.cpp
	g++ $(FLAGS) -o $@ -c $< $(LIBS) $(CFITSIO) $(BOOST)

clean :
	rm -f $(REBUILDABLES)
