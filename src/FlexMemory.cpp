// 
// 
// 

#include "FlexTypes.h"
#include "FlexMemory.h"


FlexMemory::FlexMemory(unsigned int ASize)
{
	BytesLength = ASize;
	Bytes = new unsigned char[ASize];
	for (int c = 0; c < ASize; c++)
		Bytes[c] = 0;
}

/**
* FlexMemory::GetMemorySize
*
* Returns the size of the memory.
*/
unsigned int FlexMemory::GetMemorySize()
{
	return BytesLength;
}


/**
* FlexMemory::Read
*
* Returns data from the memory.
*/
unsigned int FlexMemory::Read(const unsigned int APosition, enum FPMemorySize ASize)
{
	// call specific function depending on size
	switch (ASize)
	{
	case msByte: return this->Read8(APosition); break;
	case msWord: return this->Read16(APosition); break;
	case msDWord: return this->Read32(APosition); break;
	}
}

/**
* FlexMemory::Read8
*
* Returns a Byte from the memory.
*/
unsigned char FlexMemory::Read8(const unsigned int APosition)
{
	return this->Bytes[APosition];
}

/**
* FlexMemory::Read16
*
* Returns a Word (2 bytes) from the memory.
*/
unsigned short FlexMemory::Read16(const unsigned int APosition)
{
	return ((this->Bytes[APosition] << 8) + (this->Bytes[APosition + 1]));
}

/**
* FlexMemory::Read32
*
* Returns a DWord (4 bytes) from the memory.
*/
unsigned int FlexMemory::Read32(const unsigned int APosition)
{
	return ((((((this->Bytes[APosition] << 8) + this->Bytes[APosition + 1]) << 8) + this->Bytes[APosition + 2]) << 8) + this->Bytes[APosition + 3]);
}

/**
* FlexMemory::Write
*
* Writes data into memory.
*/
void FlexMemory::Write(const unsigned int APosition, const unsigned int AValue, enum FPMemorySize ASize)
{
	// call specific function depending on size
	switch (ASize)
	{
	case msByte: FlexMemory::Write8(APosition, AValue); break;
	case msWord: FlexMemory::Write16(APosition, AValue); break;
	case msDWord: FlexMemory::Write32(APosition, AValue); break;
	}
}

/**
* FlexMemory::Write8
*
* Writes a Byte into memory.
*/
void FlexMemory::Write8(const unsigned int APosition, const unsigned char AValue)
{
	this->Bytes[APosition] = AValue;
}

/**
* FlexMemory::Write16
*
* Writes a Word (2 bytes) into memory.
*/
void FlexMemory::Write16(const unsigned int APosition, const unsigned short AValue)
{
	this->Bytes[APosition] = AValue >> 8;
	this->Bytes[APosition + 1] = AValue;
}

/**
* FlexMemory::Write32
*
* Writes a DWord (4 bytes) into memory.
*/
void FlexMemory::Write32(const unsigned int APosition, const unsigned int AValue)
{
	this->Bytes[APosition] = AValue >> 24;
	this->Bytes[APosition + 1] = AValue >> 16;
	this->Bytes[APosition + 2] = AValue >> 8;
	this->Bytes[APosition + 3] = AValue;
}

/**
* FlexMemory::ReadFlags
*
* Returns the flags.
*/
unsigned char FlexMemory::ReadFlags()
{
	return this->Read8(FP_RegisterPosition(fprEBP) + 4);
}

/**
* FlexMemory::WriteRegisterValue
*
* Writes a value to a register.
*/
void FlexMemory::WriteRegisterValue(enum FPRegister ARegister, const unsigned int AValue)
{
	this->Write(FP_RegisterPosition(ARegister), AValue, FP_RegisterSize(ARegister));
}

/**
* FlexMemory::ReadRegisterValue
*
* Returns the value of a register.
*/
unsigned int FlexMemory::ReadRegisterValue(enum FPRegister ARegister)
{
	return this->Read(FP_RegisterPosition(ARegister), FP_RegisterSize(ARegister));
}

/**
* FlexMemory::GetEffectiveAddress
*
* Returns the effective location in memory of an address parameter.
*/
unsigned int FlexMemory::GetEffectiveAddress(struct FPBinaryAddress * AAddress)
{
	unsigned int effectiveAddress = this->ReadRegisterValue(AAddress->Register);

	// do operation if necessary
	if (AAddress->Operation == aonAddition)
	{
		if (AAddress->Operand == aodConstant)
			effectiveAddress += AAddress->OperandValue;
		else
			effectiveAddress += this->ReadRegisterValue(AAddress->OperandRegister);
	}
	else if (AAddress->Operation == aonSubtraction)
	{
		if (AAddress->Operand == aodConstant)
			effectiveAddress -= AAddress->OperandValue;
		else
			effectiveAddress -= this->ReadRegisterValue(AAddress->OperandRegister);
	}
	return effectiveAddress;
}

void FlexMemory::Push(const unsigned int AValue, FPMemorySize ASize)
{
	unsigned int currentStackPosition = ReadRegisterValue(fprESP) - ASize;
	Write(currentStackPosition, AValue, ASize);
	WriteRegisterValue(fprESP, currentStackPosition);
}

unsigned int FlexMemory::Pop(FPMemorySize ASize)
{
	unsigned int currentStackPosition = ReadRegisterValue(fprESP);
	WriteRegisterValue(fprESP, currentStackPosition + ASize);
	return Read(currentStackPosition, ASize);
}

/**
* FlexMemory::WriteAddressValue
*
* Writes data to an address parameter.
*/
void FlexMemory::WriteAddressValue(struct FPBinaryAddress * AAddress, const unsigned int AValue)
{
	// the bug is here
	FlexMemory::Write(this->GetEffectiveAddress(AAddress), AValue, AAddress->Size);
}

/**
* FlexMemory::ReadAddressValue
*
* Returns the value of the effective position from an address parameter.
*/
unsigned int FlexMemory::ReadAddressValue(struct FPBinaryAddress * AAddress)
{
	return FlexMemory::Read(this->GetEffectiveAddress(AAddress), AAddress->Size);
}