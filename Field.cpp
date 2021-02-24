#include "Field.h"
Field::Field() : width(12), height(18)
{
	// Field buffer
	field = new unsigned char[width * height];

	// Create border
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			field[y * width + x] = (x == 0 || x == width - 1 || y == height - 1); 
}

vector<int> Field::Place(Tetromino piece)
{
	vector<int> lines; 
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if (piece.IsSolid(px, py))
			{
				int x = piece.x + px;
				int y = piece.y + py;
				Set(x, y, 2); //0=emptyspace,1=wall,2=piece
				
				if (IsLine(y))
					lines.push_back(y);
			}
		}

	return lines;
}

void Field::RemoveLines(vector<int> lines)
{
	if (lines.empty())
		return;

	int count = lines.size();
	auto max = max_element(lines.begin(), lines.end());

	for (int y = *max; y > count; y--)
	{
		for (int x = 0; x < width; x++)
			Set(x, y, Get(x, y - count));
	}
}

bool Field::IsLine(int y)
{
	for (int x = 0; x < width; x++)
		if (Get(x, y) == 0)
			return false;
	return true;
}

bool Field::DoesPieceFit(Tetromino piece)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if (piece.IsSolid(px, py))
			{
				int x = piece.x + px;
				int y = piece.y + py;

				if (x >= 0 && x < width &&
					y >= 0 && y < height)
				{
					int fi = y * width + x;

					if (field[fi] != 0)
						return false;
				}
				else return false;
			}
		}

	return true;
}

char Field::Get(int x, int y) const
{
	return field[y * width + x];
}

void Field::Set(int x, int y, char c)
{
	field[y * width + x] = c;
}