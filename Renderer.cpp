#include "Renderer.h"

const int width = 80;
const int height = 30;
const int offset = 2;

Renderer::Renderer()
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
	Close();
}

void Renderer::Close()
{
	CloseHandle(hConsole);
}

void Renderer::Render(Field field)
{
	for (int x = 0; x < field.width; x++)
		for (int y = 0; y < field.height; y++)
			screen[GetIndex(x, y)] = L" ▓█"[field.Get(x, y)];
}

void Renderer::Render(Tetromino piece, int offsetX, int offsetY)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (piece.IsSolid(px, py))
			{
				int x = piece.x + px + offsetX;
				int y = piece.y + py + offsetY;
				screen[GetIndex(x, y)] = piece.graphic;
			}
}

void Renderer::RenderText(wstring text, int x, int y)
{
	RenderBlock(text, text.length(), 1, x, y);
}

void Renderer::RenderBlock(wstring graphic, int w, int h, int x, int y)
{
	int i = 0;
	for (int dy = y; dy < h+y; dy++)
		for (int dx = x; dx < w+x; dx++)
			screen[GetIndex(dx, dy)] = graphic[i++];
}

void Renderer::RenderLines(vector<int> lines, Field field)
{
	Render(field);

	if (!lines.empty())
	{
		for (auto& y : lines)
			RenderLine(y, field.width);
		Render();

		this_thread::sleep_for(400ms);
	}
	lines.clear();
}

void Renderer::RenderLine(int y, int width)
{
	for (int x = 0; x < width; x++)
	{
		screen[GetIndex(x, y)] = L'░';
	}
}

void Renderer::Render()
{
	WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0,0 }, &dwBytesWritten);
}

int Renderer::GetIndex(int x, int y) const
{
	return (y + offset) * width + (x + offset);
}