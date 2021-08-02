#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

// Enable if cfitsio is installed.
// Make sure another method of reading images exists if this is disabled!
#define CFITSIO

// Enable if you wish to display all debug messages.
#define DEBUG

// Enable if you wish to generate noiseless images.
#define NOISELESS

// Enable if you wish to generate noisy images.
#define NOISY

// Enable if you wish to time the application.
#define TIMED


// Define filtering constants
#define MAX_AREA 2000
#define MIN_BRIGHTNESS 0.8
#define MIN_RATIO (0.3 / 10000)


#ifdef TIMED
    #include "Timer.h"
#endif

#endif
