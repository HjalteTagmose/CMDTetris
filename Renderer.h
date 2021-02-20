#pragma once
#include "Field.h"
#include "Tetromino.h"
#include <string>
#include <Windows.h>
using namespace std;

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Render(Field field);
	void Render(Tetromino piece);
	void Render();



private:
	wstring theme;
	wchar_t* screen;
	HANDLE hConsole;
	DWORD dwBytesWritten = 0;
	int GetIndex(int x, int y) const;
};

