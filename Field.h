#pragma once
#include "Tetromino.h"
#include <vector>
#include <algorithm>
using namespace std;

class Field
{
public:
	const int width, height;
	Field();
	char Get(int x, int y) const;
	void Set(int x, int y, char c);
	vector<int> Place(Tetromino piece);
	void RemoveLines(vector<int> lines);
	bool IsLine(int y);
	bool DoesPieceFit(Tetromino piece);
private:
	unsigned char* field = nullptr;
};

