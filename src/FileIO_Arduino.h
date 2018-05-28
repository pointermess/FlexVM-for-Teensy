// FileIO_Arduino.h


#ifndef _FILEIO_ARDUINO_h
#define _FILEIO_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "FileIO_Interface.h"

void FDIFileIO_Initialize();
bool FDIFileIO_IsInitialized();
char* FDIFileIO_ReadFileBytes(const char *name, int* size);

#endif

