#include <fitsio.h>
#include "include/Image.h"

int main(int argc, char *argv[])
{
	fitsfile *fptr;
	SR::Image *img = new SR::Image(argv[1]);
	delete img;
	return 0;
}
