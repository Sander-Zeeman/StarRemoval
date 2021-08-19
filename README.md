# StarRemoval

This is a Star Removal tool, based on MTObjects (https://github.com/CarolineHaigh/mtobjects).
Note extensive parameter tuning is required for this tool to be as effective as it can be.

## Prerequisites
- CFITSIO installed. (https://heasarc.gsfc.nasa.gov/fitsio/)
- Boost installed. (https://www.boost.org/)
- OpenCV installed. (https://opencv.org/)

## Usage
Modify the Makefile to specify the install directories of the above-mentioned libraries.
Set any options in ./include/Debug.h
Afterwards, call make, then call ./star_removal.out <(Image file (FITS format))>
