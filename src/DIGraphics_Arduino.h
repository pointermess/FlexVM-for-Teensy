// DIGraphics_Arduino.h

#ifndef _DIGRAPHICS_ARDUINO_h
#define _DIGRAPHICS_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <ILI9341_t3.h>
#include <font_ArialBold.h>
#include <font_Arial.h>


void FDIGraphics_Initialize();
void FDIGraphics_FillScreen();
void FDIGraphics_DrawEllipse(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color);
void FDIGraphics_FillEllipse(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color);
void FDPIGraphics_DrawRectangle(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color);
void FDPIGraphics_FillRectangle(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color);
void FDPIGraphics_Invalidate();
void FDIGraphics_Show();
unsigned char FPDIGraphics_ProcessMessages();
#endif

