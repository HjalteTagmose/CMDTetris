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

void Field::Place(Tetromino piece)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if (piece.IsSolid(px, py))
				Set(piece.x + px, piece.y + py, 2); //0=emptyspace,1=wall,2=piece
		}
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