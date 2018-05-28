#include "FlexProcessor.h"
#include "DIGraphics_Arduino.h"
#include <SysCall.h>
#include "FileIO_Arduino.h"
#include "FileIO_Interface.h"
#include "FlexTypes.h"
#include "FlexMemory.h"

FlexMemory * Memory;
FlexProcessor * Processor;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	Serial.begin(9600);
	FDIGraphics_Initialize();
	FDIGraphics_FillScreen();

	FDIFileIO_Initialize();
	while (!Serial);
	Processor = new FlexProcessor();
	Processor->LoadBinary("test.asm");
	Processor->Execute();
	Processor->ReportProgramEnd();
}

void loop()
{

}
