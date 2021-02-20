#include "Tetromino.h"
#include <string>
using namespace std;

wstring tetromino[7];

Tetromino::Tetromino()
{
	#pragma region Create Tetriminos
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");
	#pragma endregion
	Reset();
}

void Tetromino::Reset()
{
	x = 0;
	y = 0;
	rotation = 0;
	type = rand() % 7;
}

void Tetromino::Move(int adjX, int adjY)
{
	x += adjX;
	y += adjY;
}

void Tetromino::Rotate(int adj)
{
	rotation += adj;
}

void Tetromino::Copy(Tetromino other)
{
	x = other.x;
	y = other.y;
	type = other.type;
	rotation = other.rotation;
}

bool Tetromino::IsSolid(int x, int y) const
{
	return tetromino[type][Rotate(x, y, rotation)] == L'X';
}

int Tetromino::Rotate(int px, int py, int r) const
{
	// Gets rotated index
	switch (r % 4)
	{
	case 0: return py * 4 + px;
	case 1: return 12 + py - (px * 4);
	case 2: return 15 - (py * 4) - px;
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}