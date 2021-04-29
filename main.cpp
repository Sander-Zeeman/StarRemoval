#include "Image.h"

int main(int argc, char *argv[])
{
	SR::Image *img = new SR::Image(argv[1]);
	delete img;
	return 0;
}
