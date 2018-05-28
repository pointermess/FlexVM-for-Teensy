// FlexProcessor.h

#ifndef _FLEXPROCESSOR_h
#define _FLEXPROCESSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "FlexMemory.h"

class FlexProcessor;

typedef struct FPSystemCall {
	unsigned int Code;
	void(*Call)(FlexProcessor * AProcessor, unsigned int ACode, unsigned int AParam1, unsigned int AParam2, unsigned int * AOutput);
};

class FlexProcessor
{
private:
	unsigned long FStartTime;
	unsigned int FProgramCounter;
	unsigned int FProgramSize;
	char * FProgramCode;
	FlexMemory* FMemory;
	struct FPSystemCall FSystemCalls[16];
public:
	FlexProcessor();
	~FlexProcessor();

	// Methods
	void LoadBinary(char * APath);
	unsigned char ReadNextByte(unsigned int * AStepsTaken);
	unsigned short ReadNextWord(unsigned int * AStepsTaken);
	unsigned int ReadNextDWord(unsigned int * AStepsTaken);
	FPRegister ReadNextBinaryRegister(unsigned int * AStepsTaken);
	FPBinaryConstant ReadNextBinaryConstant(unsigned int * AStepsTaken);
	FPBinaryAddress ReadNextBinaryAddress(unsigned int * AStepsTaken);
	void ExecuteStep();
	void Execute();
	void ReportProgramEnd();


	// Properties
	char * GetProgramCode();
	unsigned int GetProgramSize();
	unsigned int GetProgramCounter();
	bool IsProgramLoaded();
};

#endif

