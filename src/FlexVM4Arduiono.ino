#include "SCGraphics_Arduino.h"
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
	Serial.begin(9600); 




	FDIFileIO_Initialize();
	Processor = new FlexProcessor();
	Processor->LoadBinary("fill_1x1.asm");
	Processor->Execute();
	Processor->ReportProgramEnd();
}

void loop()
{

}
