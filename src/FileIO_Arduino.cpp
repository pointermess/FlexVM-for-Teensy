// 
// 
// 

#include "FileIO_Arduino.h"

#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
#define SD_CS_PIN 8
SdFat SD;

bool FDIFileIO_FIsInitialized = false;

void FDIFileIO_Initialize()
{
	FDIFileIO_FIsInitialized = SD.begin(SD_CS_PIN, SD_SCK_MHZ(16));
}

bool FDIFileIO_IsInitialized()
{
	return FDIFileIO_FIsInitialized;
}

char * FDIFileIO_ReadFileBytes(const char * name, int * size)
{
	File file = SD.open(name);

	if (file) {
		*size = file.size();
		char * ret = new char[file.size()];
		unsigned int c = 0;
		while (file.available()) {
			ret[c] = (unsigned char)file.read();
			c = c + 1;
		}
		file.close();
		return ret;
	}
	else {
		Serial.print("error opening ");
		Serial.println(name);
	}
}
