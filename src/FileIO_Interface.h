// FileIO_Interface.h

#ifndef _FILEIO_INTERFACE_h
#define _FILEIO_INTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class FlexFileIO
{
public:
	virtual char* ReadFileBytes(const char *name, int* sz);
};

#endif

