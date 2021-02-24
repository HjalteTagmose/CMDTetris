#pragma once
#include "Field.h"
#include "Tetromino.h"
#include <thread>
#include <string>
#include <Windows.h>
using namespace std;

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Close();
	void Render(Field field);
	void Render(Tetromino piece, int offsetX = 0, int offsetY = 0);
	void RenderText(wstring text, int x, int y);
	void RenderBlock(wstring graphic, int w, int h, int x, int y);
	void RenderLines(vector<int> lines, Field field);
	void Render();
private:
	wstring theme;
	wchar_t* screen;
	HANDLE hConsole;
	DWORD dwBytesWritten = 0;
	int GetIndex(int x, int y) const;
	void RenderLine(int y, int width);
};

