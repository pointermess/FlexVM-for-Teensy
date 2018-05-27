#include <SysCall.h>
#include <ILI9341_t3.h>
#include <font_ArialBold.h>
#include <font_Arial.h>
#include "FileIO_Arduino.h"
#include "FileIO_Interface.h"
#include "FlexTypes.h"
#include "FlexMemory.h"

FlexMemory * Memory;

#define TFT_DC  9
#define TFT_CS 10
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
void setup()
{

	Memory = new FlexMemory(64);
	pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	Serial.begin(9600);
	Memory->Write(0, 0xABCDEF12, msDWord);
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);
}

void loop()
{

	FlexFileIO* fio = new FlexFileIOArduino();
	int ProgramSize;
	char * ProgramCode;
	ProgramCode = fio->ReadFileBytes("test.asm", &ProgramSize);
	delete fio;
	Serial.println("");
	Serial.print("size: ");
	Serial.println(ProgramSize);
		Serial.println("");

	Serial.write("test");
	unsigned int test = Memory->Read32(2);
	Serial.print(test, HEX);
	digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
									  // but actually the LED is on; this is because 

	tft.fillScreen(ILI9341_RED);						  // it is active low on the ESP-01)
	delay(100);
	tft.fillScreen(ILI9341_GREEN);                // Wait for a second
	digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
	delay(100);
	tft.fillScreen(ILI9341_YELLOW);
}
