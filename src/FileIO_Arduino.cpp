// 
// 
// 

#include "FileIO_Arduino.h"

#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
#define SD_CS_PIN 8
SdFat SD;
char* FlexFileIOArduino::ReadFileBytes(const char * name, int * sz)
{

	if (!SD.begin(SD_CS_PIN, SD_SCK_MHZ(8))) {
		Serial.println("initialization failed!");
	}
	Serial.println("initialization done.");

	File file = SD.open(name);

	if (file) {
		*sz = file.size();
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
