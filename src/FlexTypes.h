// FlexTypes.h

#ifndef _FLEXTYPES_h
#define _FLEXTYPES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum FPMemorySize {
	msUndefined = 0,
	msByte = 1,
	msWord = 2,
	msDWord = 4
};

enum FPFlags {
	fpfCarry
	// To be done.
};

enum FPRegister {
	fprUnknown = -1,
	// General purpose registers

	// Least significant bytes 000000[00]
	// floor(n / 4) = 0
	fprAL = 0, // mod 4 = 0
	fprBL = 1, // mod 4 = 1
	fprCL = 2, // mod 4 = 2
	fprDL = 3, // mod 4 = 3

			   // Significant byte of lower byte-pair 0000[00]00
			   // floor(n / 4) = 1
	fprAH = 4, // mod 4 = 0
	fprBH = 5, // mod 4 = 1
	fprCH = 6, // mod 4 = 2
	fprDH = 7, // mod 4 = 3

						  // Two most significant bytes [0000]0000
						  // floor(n / 4) = 2
	fprAX = 8,  // mod 4 = 0
	fprBX = 9,  // mod 4 = 1
	fprCX = 10, // mod 4 = 2
	fprDX = 11, // mod 4 = 3

			  // All four bytes [00000000]
			  // floor(n / 4) = 3
	fprEAX = 12, // mod 4 = 0
	fprEBX = 13, // mod 4 = 1
	fprECX = 14, // mod 4 = 2
	fprEDX = 15, // mod 4 = 3

						   // Stack registers
						   // floor(n / 4) = 4
	fprESP = 16, // mod 4 = 0
	fprEBP = 17  // mod 4 = 1
};

enum FPInstruction {
	fpiUnknown = 0,

	// Move instructions (0xA0 - 0xA5)
	fpiMOV_REG_CONST = 0xa0,
	fpiMOV_REG_REG = 0xa1,
	fpiMOV_REG_ADDR = 0xa2,
	fpiMOV_ADDR_CONST = 0xa3,
	fpiMOV_ADDR_REG = 0xa4,
	fpiMOV_ADDR_ADDR = 0xa5,

	// Stack instructions (0xAC - 0xAF)
	fpiPUSH_CONST = 0xac,
	fpiPUSH_REG = 0xad,
	fpiPUSH_ADDR = 0xae,
	fpiPOP_REG = 0xaf,

	// Flowcontrol instructions (0x70 - 0x94)
	fpiJMP_LABEL = 0x70,
	fpiJMP_ADDR = 0x71,
	fpiJO_LABEL = 0x72,
	fpiJO_ADDR = 0x73,
	fpiJNO_LABEL = 0x74,
	fpiJNO_ADDR = 0x75,
	fpiJS_LABEL = 0x76,
	fpiJS_ADDR = 0x77,
	fpiJNS_LABEL = 0x78,
	fpiJNS_ADDR = 0x79,
	fpiJE_LABEL = 0x7a,
	fpiJE_ADDR = 0x7b,
	fpiJNE_LABEL = 0x7c,
	fpiJNE_ADDR = 0x7d,
	fpiJB_LABEL = 0x7e,
	fpiJB_ADDR = 0x7f,
	fpiJBE_LABEL = 0x80,
	fpiJBE_ADDR = 0x81,
	fpiJA_LABEL = 0x82,
	fpiJA_ADDR = 0x83,
	fpiJAE_LABEL = 0x84,
	fpiJAE_ADDR = 0x85,
	fpiJL_LABEL = 0x86,
	fpiJL_ADDR = 0x87,
	fpiJLE_LABEL = 0x88,
	fpiJLE_ADDR = 0x89,
	fpiJG_LABEL = 0x8a,
	fpiJG_ADDR = 0x8b,
	fpiJGE_LABEL = 0x8c,
	fpiJGE_ADDR = 0x8d,
	fpiJP_LABEL = 0x8e,
	fpiJP_ADDR = 0x8f,
	fpiJNP_LABEL = 0x90,
	fpiJNP_ADDR = 0x91,

	fpiCALL_LABEL = 0x92,
	fpiCALL_ADDR = 0x93,
	fpiRET = 0x94,

	// System Call instruction (0x98)
	fpiINT_CONST = 0x98,

	// Math instructions (0xE0 - 0xEF)
	// Unsigned
	fpiADD_REG_CONST = 0xe0,
	fpiADD_REG_REG = 0xe1,
	fpiSUB_REG_CONST = 0xe2,
	fpiSUB_REG_REG = 0xe3,
	fpiMUL_REG_CONST = 0xe4,
	fpiMUL_REG_REG = 0xe5,
	fpiDIV_REG_CONST = 0xe6,
	fpiDIV_REG_REG = 0xe7,

	// signed
	fpiIADD_REG_CONST = 0xe8,
	fpiIADD_REG_REG = 0xe9,
	fpiISUB_REG_CONST = 0xea,
	fpiISUB_REG_REG = 0xeb,
	fpiIMUL_REG_CONST = 0xec,
	fpiIMUL_REG_REG = 0xed,
	fpiIDIV_REG_CONST = 0xee,
	fpiIDIV_REG_REG = 0xef,

	fpiCMP_REG_CONST = 0xf0,
	fpiCMP_REG_REG = 0xf1,

	fpiDBG_REG = 0xfe,
	fpiDBG_ADDR = 0xff

};

typedef struct FPBinaryConstant {
	unsigned int Value;
	enum FPMemorySize Size;
};

enum FPAddressOperation { aonUndefined, aonAddition, aonSubtraction };
enum FPAddressOperand { aodConstant, aodRegister };

typedef struct FPBinaryAddress {
	enum FPMemorySize Size;
	enum FPRegister Register;
	enum FPAddressOperation Operation;
	enum FPAddressOperand Operand;
	enum FPRegister OperandRegister;
	unsigned int OperandValue;
};

unsigned int FP_RegisterPosition(enum FPRegister ARegister);
enum FPMemorySize FP_ByteToSize(const unsigned char AByte);
enum FPMemorySize FP_RegisterSize(enum FPRegister ARegister);


#endif

