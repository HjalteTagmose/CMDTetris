#pragma once
class Tetromino
{
public:
	int type;
	int rotation;
	int x;
	int y;
	wchar_t graphic = L'█';
	Tetromino();
	Tetromino(wchar_t c);
	void Reset();
	void Move(int adjX, int adjY);
	void Rotate(int adj);
	void Copy(Tetromino other);
	bool IsSolid(int x, int y) const;
	int Rotate(int px, int py, int r) const;
private:
};

