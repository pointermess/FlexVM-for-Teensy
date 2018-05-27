// 
// 
// 

#include "FlexTypes.h"



/**
* FP_RegisterPosition
*
* Returns the effective memory location of a register.
*/
unsigned int FP_RegisterPosition(enum FPRegister ARegister)
{
	// direct port of FPRegisterPosition from delphi project
	unsigned char LRegisterBegin;
	unsigned char LRegisterType;
	unsigned char LEnumValue = ARegister;
	if (LEnumValue >= 0 && LEnumValue <= 15)
	{
		LRegisterBegin = (LEnumValue % 4) * 4;
		LRegisterType = floor(LEnumValue / 4);


		if (LRegisterType == 0)
			return LRegisterBegin + 3;
		else if (LRegisterType == 1)
			return LRegisterBegin + 2;
		else if (LRegisterType == 2)
			return LRegisterBegin;
		else if (LRegisterType == 3)
			return LRegisterBegin;
	}
	else if (LEnumValue == 16 || LEnumValue == 17)
	{
		return 16 + ((LEnumValue - 16) * 4);
	}
}

/**
* FP_RegisterSize
*
* Returns the size of a register.
*/
enum FPMemorySize FP_RegisterSize(enum FPRegister ARegister)
{
	// direct port of FPRegisterPosition from delphi project
	unsigned char LEnumValue = ARegister;

	if (LEnumValue >= 0 && LEnumValue <= 7)
		return msByte;
	if (LEnumValue >= 8 && LEnumValue <= 11)
		return msWord;
	if (LEnumValue >= 12)
		return msDWord;
};

/**
* FP_ByteToSize
*
* Returns the binary size notation as FPMemorySize enum.
*/
enum FPMemorySize FP_ByteToSize(const unsigned char AByte)
{
	if (AByte == 0) return msByte;
	if (AByte == 1) return msWord;
	if (AByte == 2) return msDWord;
}