#include "Renderer.h"

const int width = 80;
const int height = 30;
const int offset = 2;

Renderer::Renderer() : theme(L" ABCDEFG=#")
{
	// Screen buffer
	screen = new wchar_t[width * height];

	// Clear screen
	for (int i = 0; i < width * height; i++)
		screen[i] = L' ';

	// Get handle to console screen buffer
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
}

Renderer::~Renderer()
{
	CloseHandle(hConsole);
}

void Renderer::Render(Field field)
{
	for (int x = 0; x < field.width; x++)
		for (int y = 0; y < field.height; y++)
			screen[GetIndex(x, y)] = theme[field.Get(x, y)];
}

void Renderer::Render(Tetromino piece)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (piece.IsSolid(px, py))
				screen[ GetIndex(piece.x + px, piece.y + py) ] = theme[piece.type+1];
}

void Renderer::Render()
{
	WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0,0 }, &dwBytesWritten);
}

int Renderer::GetIndex(int x, int y) const
{
	return (y + offset) * width + (x + offset);
}