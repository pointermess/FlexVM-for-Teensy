// FlexMemory.h

#ifndef _FLEXMEMORY_h
#define _FLEXMEMORY_h

#ifdef RELEASE_ARDUINO
	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif
#endif

class FlexMemory
{
private:
	unsigned char * Bytes;
public:
	FlexMemory(unsigned int ASize);

	unsigned int Read(const unsigned int APosition, enum FPMemorySize ASize);
	unsigned char Read8(const unsigned int APosition);
	unsigned short Read16(const unsigned int APosition);
	unsigned int Read32(const unsigned int APosition);

	void Write(const unsigned int APosition, const unsigned int AValue, enum FPMemorySize ASize);
	void Write8(const unsigned int APosition, const unsigned char AValue);
	void Write16(const unsigned int APosition, const unsigned short AValue);
	void Write32(const unsigned int APosition, const unsigned int AValue);

	unsigned char ReadFlags(struct FPMemory * AMemory);

	void WriteRegisterValue(enum FPRegister ARegister, const unsigned int AValue);
	unsigned int ReadRegisterValue(enum FPRegister ARegister);

	void WriteAddressValue(struct FPBinaryAddress * AAddress, const unsigned int AValue);
	unsigned int ReadAddressValue(struct FPBinaryAddress * AAddress);


	unsigned int GetEffectiveAddress(struct FPBinaryAddress * AAddress);
};


#endif

