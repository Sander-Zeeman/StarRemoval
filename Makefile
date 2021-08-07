# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

FLAGS = \
  -O2 \
  -pedantic \
  -Wall \
  -Wextra

OBJS = \
  BGFunctions.o \
  Detector.o \
  Heap.o \
  Image.o \
  main.o \
  MaxTree.o \
  PostProcessing.o \
  Timer.o

CFITSIO = -I /usr/local/include/cfitsio -lcfitsio
BOOST = -I /usr/local/include/boost
OPENCV = -I /usr/local/include/opencv4 -lopencv_core -lopencv_photo -lopencv_imgproc

LIBS = -L /usr/local/lib

REBUILDABLES = $(addprefix out/, $(OBJS)) $(LINK_TARGET)

all : $(LINK_TARGET)

$(LINK_TARGET) : $(addprefix out/, $(OBJS))
	g++ -o $@ $^ $(LIBS) $(CFITSIO) $(BOOST) $(OPENCV)

out/%.o : src/%.cpp
	g++ $(FLAGS) -o $@ -c $< $(LIBS) $(CFITSIO) $(BOOST) $(OPENCV)

clean :
	rm -f $(REBUILDABLES)
