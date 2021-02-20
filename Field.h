#pragma once
#include "Tetromino.h"
class Field
{
public:
	const int width, height;
	Field();
	char Get(int x, int y) const;
	void Set(int x, int y, char c);
	void Place(Tetromino piece);
	bool DoesPieceFit(Tetromino piece);
private:
	unsigned char* field = nullptr;
};

