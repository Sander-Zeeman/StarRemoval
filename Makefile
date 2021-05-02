# Based on the information at: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

LINK_TARGET = star_removal.out

OBJS = \
  Image.o \
  main.o

CFITSIO_INCLUDE = /usr/include/cfitsio
CFITSIO_LIB = /usr/lib/cfitsio

REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	echo All done

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^

%.o : %.cpp
	g++ -g -o $@ -c $< -I $(CFITSIO_INCLUDE) -L $(CFITSIO_LIB)

clean :
	rm -f $(REBUILDABLES)
	echo Clean done
