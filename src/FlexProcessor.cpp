// 
// 
// 

#include "FlexProcessor.h"
#include "FileIO_Arduino.h"
#include "SCGraphics_Arduino.h"

FlexProcessor::FlexProcessor()
{
	FStartTime = millis();
	FProgramCounter = 0;
	FProgramSize = 0;
	FMemory = new FlexMemory(8096);

	FSystemCalls[0].Code = 420;
	FSystemCalls[0].Call = &FPSCGraphics_Call;
	/*
	FSystemCalls[1].Code = 421;
	FSystemCalls[1].Call = &FPSCKeyboardInput_Call;

	FSystemCalls[2].Code = 422;
	FSystemCalls[2].Call = &FPSCSysUtils_Call;*/
}

FlexProcessor::~FlexProcessor()
{
	delete FMemory;
}

void FlexProcessor::LoadBinary(char * APath)
{
	if (FDIFileIO_IsInitialized())
	{
		// needs to free last program from ram
		int size = 0;
		FProgramCode = FDIFileIO_ReadFileBytes(APath, &size);
		FProgramSize = size;

		if (FProgramSize > 0)
		{

			for (int c = 0; c < FProgramSize; c++)
			{
				Serial.print(FProgramCode[c], HEX);
				Serial.print(" ");
			}
			Serial.println("");
			Serial.print(FProgramSize);
			Serial.println("");
		}
	}
}

void FlexProcessor::ReadBinaryFromSerial()
{
	pinMode(A9, OUTPUT);
	analogWrite(A9, 1);
	while (!Serial);
	analogWrite(A9, 0);

	// serial binary transmission protocol
	Serial.write("[sbt]ready[/sbt]");
}

/**
* FlexProcessor::ReadNextByte
*
* Reads the next byte from the binary executable code and increments AStepsTaken.
*/
unsigned char FlexProcessor::ReadNextByte(unsigned int * AStepsTaken)
{
	// reads one byte and increases AStepsTaken
	*AStepsTaken += 1;
	return this->FProgramCode[this->FProgramCounter + *AStepsTaken];
}

/**
* FlexProcessor::ReadNextWord
*
* Reads two bytes from the executable code.
*/
unsigned short FlexProcessor::ReadNextWord(unsigned int * AStepsTaken)
{
	return (ReadNextByte(AStepsTaken) << 8) + ReadNextByte(AStepsTaken);
}

/**
* FlexProcessor::ReadNextDWord
*
* Reads four bytes from the executable code.
*/
unsigned int FlexProcessor::ReadNextDWord(unsigned int * AStepsTaken)
{
	return (ReadNextWord(AStepsTaken) << 16) + ReadNextWord(AStepsTaken);
}

/**
* FlexProcessor::ReadNextBinaryRegister
*
* Reads the next register from the executable code.
*/
enum FPRegister FlexProcessor::ReadNextBinaryRegister(unsigned int * AStepsTaken)
{
	return (enum FPRegister)ReadNextByte(AStepsTaken);
}

/**
* FlexProcessor::ReadNextBinaryConstant
*
* Reads the next constant from the executable code.
*/
struct FPBinaryConstant FlexProcessor::ReadNextBinaryConstant(unsigned int * AStepsTaken)
{
	struct FPBinaryConstant result;

	// read size from first byte of binary constant notation
	result.Size = FP_ByteToSize(ReadNextByte(AStepsTaken));

	// Read "n" bytes depending on the constant size
	switch (result.Size)
	{
	case (msByte): result.Value = ReadNextByte(AStepsTaken); break;
	case (msWord): result.Value = ReadNextWord(AStepsTaken); break;
	case (msDWord): result.Value = ReadNextDWord(AStepsTaken); break;
	}

	return result;
}

/**
* FlexProcessor::ReadNextBinaryAddress
*
* Reads the next address from the executable code.
*/
struct FPBinaryAddress FlexProcessor::ReadNextBinaryAddress(unsigned int * AStepsTaken)
{
	struct FPBinaryAddress result;

	unsigned char pointerSize = ReadNextByte(AStepsTaken);
	unsigned char registerByte = ReadNextByte(AStepsTaken);

	result.Size = FP_ByteToSize(pointerSize);

	if ((registerByte >= 0xA0) && (registerByte <= 0xA5)) // no add
	{
		result.Register = FPRegister(registerByte - 0xA0 + 0x0C);
		result.Operation = aonUndefined;
	}
	else if ((registerByte >= 0xB0) && (registerByte <= 0xB5)) // add const
	{
		result.Register = FPRegister(registerByte - 0xB0 + 0x0C);
		result.Operation = aonAddition;
		result.Operand = aodConstant;
	}
	else if ((registerByte >= 0xC0) && (registerByte <= 0xC5)) // sub const
	{
		result.Register = FPRegister(registerByte - 0xC0 + 0x0C);
		result.Operation = aonSubtraction;
		result.Operand = aodConstant;
	}
	else if ((registerByte >= 0xD0) && (registerByte <= 0xD5)) // add reg
	{
		result.Register = FPRegister(registerByte - 0xD0 + 0x0C);
		result.Operation = aonAddition;
		result.Operand = aodRegister;
	}
	else if ((registerByte >= 0xE0) && (registerByte <= 0xE5)) // sub reg
	{
		result.Register = FPRegister(registerByte - 0xE0 + 0x0C);
		result.Operation = aonSubtraction;
		result.Operand = aodRegister;
	}

	if (result.Operation != aonUndefined)
	{
		if ((registerByte >= 0xB0) && (registerByte <= 0xC5))
			result.OperandValue = ReadNextDWord(AStepsTaken);
		else if ((registerByte >= 0xD0) && (registerByte <= 0xE5))
			result.OperandRegister = FPRegister(ReadNextByte(AStepsTaken));
	}


	return result;
};

/**
* FlexProcessor::ExecuteStep
*
* Executes one step in the binary code.
*/
void FlexProcessor::ExecuteStep()
{
	// reads current instruction opcode
	unsigned char instructionCode = this->FProgramCode[this->FProgramCounter];

	// needed for next bytes / arguments in code
	unsigned int stepsTaken = 0;

	unsigned char jumped = 0; // set jumped to false

	//FPDIGraphics_ProcessMessages();

	// switch case for every instruction
	switch (instructionCode)
	{
	case fpiMOV_REG_CONST:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, arg2.Value);
	}
	break;

	case fpiMOV_REG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg2));
	}
	break;

	case fpiMOV_REG_ADDR:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryAddress arg2 = ReadNextBinaryAddress(&stepsTaken);

		FMemory->WriteRegisterValue(arg1, FMemory->ReadAddressValue(&arg2));
	}
	break;

	case fpiMOV_ADDR_CONST:
	{
		// broken
		struct FPBinaryAddress arg1 = ReadNextBinaryAddress(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->WriteAddressValue(&arg1, arg2.Value);

	}
	break;

	case fpiMOV_ADDR_REG:
	{
		struct FPBinaryAddress arg1 = ReadNextBinaryAddress(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);
		FMemory->WriteAddressValue(&arg1, FMemory->ReadRegisterValue(arg2));
	}
	break;

	case fpiPUSH_CONST:
	{
		struct FPBinaryConstant arg1 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->Push(arg1.Value, arg1.Size);
	}
	break;

	case fpiPUSH_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		FMemory->Push(FMemory->ReadRegisterValue(arg1), FP_RegisterSize(arg1));
	}
	break;

	case fpiPUSH_ADDR:
	{
		struct FPBinaryAddress arg1 = ReadNextBinaryAddress(&stepsTaken);
		FMemory->Push(FMemory->ReadAddressValue(&arg1), arg1.Size);
	}
	break;

	case fpiPOP_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, FMemory->Pop(FP_RegisterSize(arg1)));
	}
	break;

	case fpiADD_REG_CONST:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) + arg2.Value);
	}
	break;

	case fpiADD_REG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);

		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) + FMemory->ReadRegisterValue(arg2));
	}
	break;

	case fpiSUB_REG_CONST:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) - arg2.Value);
	}
	break;

	case fpiSUB_REG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);

		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) - FMemory->ReadRegisterValue(arg2));
	}
	break;

	case fpiMUL_REG_CONST:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);
		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) * arg2.Value);
	}
	break;

	case fpiMUL_REG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);

		FMemory->WriteRegisterValue(arg1, FMemory->ReadRegisterValue(arg1) * FMemory->ReadRegisterValue(arg2));
	}
	break;

	case fpiINT_CONST:
	{
		struct FPBinaryConstant arg1 = ReadNextBinaryConstant(&stepsTaken);

		for (int c = 0; c < 16; c++)
		{
			if (this->FSystemCalls[c].Code == arg1.Value)
			{
				unsigned int output = FMemory->ReadRegisterValue(fprEBX);
				unsigned int code = FMemory->ReadRegisterValue(fprEAX);
				this->FSystemCalls[c].Call(this, code, (unsigned int)FMemory->ReadRegisterValue(fprECX), (unsigned int)FMemory->ReadRegisterValue(fprEDX), &output);
				FMemory->WriteRegisterValue(fprEBX, output);
				break;
			}
		}
	}
	break;

	case fpiCALL_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		FMemory->Push(this->FProgramCounter + stepsTaken + 1, msDWord);
		this->FProgramCounter = arg1;
		jumped = 1;
	}
	break;

	case fpiRET:
	{
		this->FProgramCounter = FMemory->Pop(msDWord);
		jumped = 1;
	}
	break;

	case fpiJMP_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		this->FProgramCounter = arg1;
		jumped = 1;
	}
	break;

	case fpiJE_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		if (FMemory->ReadFlags() & 0x1)
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiJNE_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		if (!(FMemory->ReadFlags() & 0x1))
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiJG_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		if (FMemory->ReadFlags() & 0x2)
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiJGE_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		if (FMemory->ReadFlags() & 0x3)
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiJL_LABEL:
	{
		unsigned int arg1 = ReadNextDWord(&stepsTaken);
		if (!(FMemory->ReadFlags() & 0x3))
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiJLE_LABEL:
	{
		unsigned int arg1 = this->ReadNextDWord(&stepsTaken);
		if (!(FMemory->ReadFlags() & 0x2))
		{
			this->FProgramCounter = arg1;
			jumped = 1;
		}
	}
	break;

	case fpiCMP_REG_CONST:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		struct FPBinaryConstant arg2 = ReadNextBinaryConstant(&stepsTaken);

		unsigned int val0 = FMemory->ReadRegisterValue(arg1);
		unsigned int val1 = arg2.Value;

		FMemory->Write8(FP_RegisterPosition(fprEBP) + 4, ((val0 == val1) | (val0 > val1) << 1));
	}
	break;

	case fpiCMP_REG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		enum FPRegister arg2 = ReadNextBinaryRegister(&stepsTaken);

		unsigned int val0 = FMemory->ReadRegisterValue(arg1);
		unsigned int val1 = FMemory->ReadRegisterValue(arg2);

		FMemory->Write8(FP_RegisterPosition(fprEBP) + 4, ((val0 == val1) | (val0 > val1) << 1));
	}
	break;

	case fpiDBG_REG:
	{
		enum FPRegister arg1 = ReadNextBinaryRegister(&stepsTaken);
		unsigned int val0 = FMemory->ReadRegisterValue(arg1);
		Serial.printf("Debug Register '%u': %u [0x%08x]\n", arg1, val0, val0);

	}
	break;

	case fpiDBG_ADDR:
	{
		struct FPBinaryAddress arg1 = ReadNextBinaryAddress(&stepsTaken);
		unsigned int val0 = FMemory->ReadAddressValue(&arg1);
		Serial.printf("Debug Address '%u': %u [0x%08x]\n", FMemory->GetEffectiveAddress(&arg1), val0, val0);
	}
	break;

	default:
		Serial.printf("unknown instruction '%u' at %u\n", instructionCode, this->FProgramCounter);
		break;
	}

	// if not jumped then go to next instruction
	if (jumped == 0)
		this->FProgramCounter += stepsTaken + 1;
	else
		jumped = 1;
}

/**
* FlexProcessor::ExecuteStep
*
* Executes all instructions.
*/
void FlexProcessor::Execute()
{
	FStartTime = millis();
	FProgramCounter = 0;
	unsigned char finished = 0;
	FProgramCounter = 0;
	if (this->FProgramSize <= this->FProgramCounter) return;

	do
	{
		ExecuteStep();
		// if program counter is out of program then finish
		if (this->FProgramSize <= this->FProgramCounter)
			finished = 1;
	} while (finished == 0);
}

/**
* FPProcessor_ReportProgramEnd
*
* Prints some interesting informations in to the console.
*/
void FlexProcessor::ReportProgramEnd()
{
	Serial.printf("------------------------------\n");
	//float diff = ((float)(clock() - AProcessor->StartTime) / 1000000.0F) * 1000;
	//Serial.printf("Program finished...\nExecution time: %.3f s\n------------------------------\n", millis());


	unsigned long diff = millis() - FStartTime;
	Serial.printf("Program finished...\nExecution time: %u ms\n------------------------------\n", millis());

	Serial.printf("Memory Size: %u bytes\n\n", FMemory->GetMemorySize());

	Serial.printf("Registers Data: \n");

	Serial.printf("EAX Register: 0x%08x\n", FMemory->ReadRegisterValue(fprEAX));
	Serial.printf("EBX Register: 0x%08x\n", FMemory->ReadRegisterValue(fprEBX));
	Serial.printf("ECX Register: 0x%08x\n", FMemory->ReadRegisterValue(fprECX));
	Serial.printf("EDX Register: 0x%08x\n", FMemory->ReadRegisterValue(fprEDX));
	Serial.printf("ESP Register: 0x%08x\n", FMemory->ReadRegisterValue(fprESP));
	Serial.printf("EBP Register: 0x%08x\n", FMemory->ReadRegisterValue(fprEBP));

	Serial.printf("\n");

	Serial.printf("Program Counter: %u\n", FProgramCounter);

	Serial.printf("------------------------------\n");
}

char * FlexProcessor::GetProgramCode()
{
	return FProgramCode;
}

unsigned int FlexProcessor::GetProgramSize()
{
	return FProgramSize;
}

unsigned int FlexProcessor::GetProgramCounter()
{
	return FProgramCounter;
}

bool FlexProcessor::IsProgramLoaded()
{
	return FProgramSize > 0;
}
