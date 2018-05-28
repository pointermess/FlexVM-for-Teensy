// 
// 
// 

#include "DIGraphics_Arduino.h"

#define TFT_DC  9
#define TFT_CS 10
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

void FDIGraphics_Initialize()
{
	
	tft.begin();
}

void FDIGraphics_FillScreen()
{
	tft.fillScreen(ILI9341_BLACK);
}

void FDIGraphics_DrawEllipse(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color)
{
	FDPIGraphics_DrawRectangle(x, y, x2, y2, color);
}

void FDIGraphics_FillEllipse(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color)
{
	FDPIGraphics_FillRectangle(x, y, x2, y2, color);
}

void FDPIGraphics_DrawRectangle(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color)
{
	tft.drawRect(x, y, x2 - x, y2 - y, color);
}

void FDPIGraphics_FillRectangle(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned short color)
{
	tft.fillRect(x, y, x2 - x, y2 - y, color);
}
