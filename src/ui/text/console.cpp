#include "console.h"
#include "print.h"

Console::Console()
{
	x = 0.5;
	y = 0.5;

	FlipColorAtCursor();
}

void Console::FlipColorAtCursor()
{
	int xCoord = (int)(x * width);
	int yCoord = (int)(y * height);

	uint16_t lower = *(screen + yCoord * width + xCoord) & 0x0F00;
	uint16_t upper = *(screen + yCoord * width + xCoord) & 0xF000;

	uint16_t newUpper = lower << 4;
	uint16_t newLower = upper >> 4;

	*(screen + yCoord * width + xCoord) &= 0x00FF;
	*(screen + yCoord * width + xCoord) |= (newUpper | newLower);
}

void Console::OnMouseUp(int button)
{
}

void Console::OnMouseMove(double dx, double dy)
{
	FlipColorAtCursor();

	x += dx / 1000;
	y += width * dy / height / 1000;

	if ( x < 0 )
		x = 0;
	if ( x >= 1 )
		x = 0.999;
	if ( y < 0 )
		y = 0;
	if ( y >= 1 )
		y = 0.999;

	FlipColorAtCursor();
}

void Console::OnMouseDown(int button)
{
	printf("%d", button);
}

void Console::OnKeyDown(char key)
{
	putchar(key);
}

void Console::OnKeyUp(char)
{
}