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
	tft.invertDisplay(false);
	tft.fillScreen(ILI9341_BLACK);
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
#define COUNT_ENTRIES(ARRAY)    (sizeof(ARRAY) / sizeof(ARRAY[0]))
void FDPIGraphics_FillText(char * text, unsigned int x, unsigned int y, unsigned short color, unsigned short bgcolor)
{
	// 1 = 6
	// 2 = 12
	unsigned int addX = 0;
	tft.fillScreen(ILI9341_BLUE);
	for (unsigned char c = 0; c < COUNT_ENTRIES(text); c++)
	{
		tft.drawChar(x + addX, y, text[c], ILI9341_RED, ILI9341_WHITE, 5);
		addX += 5 * 6;
	}
}
