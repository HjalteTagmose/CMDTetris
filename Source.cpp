#include "unicode.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include "Renderer.h"
#include "Field.h"
#include "Tetromino.h"
#include "Input.h"

using namespace std;

Renderer renderer;
Input input;
Field field;
Tetromino piece(L'█');
Tetromino ghost(L'░');
Tetromino nextp(L'█');
Tetromino* holdp;

bool usingHeld = false;
bool gameOver = false;
int piecesPlaced = 0;
int score = 0;
int speed = 20;
int speedCounter = 0;

bool TryMovePiece(int dx, int dy, int dr)
{
	// Move ghost to check
	ghost.Copy(piece);
	ghost.Move(dx, dy);
	ghost.Rotate(dr);

	// Move piece if it fits
	if (field.DoesPieceFit(ghost))
	{
		piece.Move(dx, dy);
		piece.Rotate(dr);
		return true;
	}
	
	ghost.Copy(piece);
	return false;
}

void NextPiece()
{
	piece.Copy(nextp);
	nextp.Reset();
}

void PlacePiece()
{
	if (!field.DoesPieceFit(piece))
	{
		gameOver = true;
		return;
	}

	piecesPlaced++;
	if (piecesPlaced % 10 == 0)
		speed -= speed < 10? 0 : 1;

	usingHeld = false;
	vector<int> lines = field.Place(piece);
	NextPiece();

	if (!lines.empty())
	{
		score += 100 * (lines.size() * lines.size());
		renderer.RenderLines(lines, field);
		field.RemoveLines(lines);
	}
	else score += 25;
}

void HoldPiece()
{
	if (usingHeld) return;
	usingHeld = true;
	piece.x = 0;
	piece.y = 0;
	piece.rotation = 0;

	if (!holdp)
	{
		holdp = new Tetromino(L'█');
		holdp->Copy(piece);
		NextPiece();
	}
	else
	{
		Tetromino temp(L'█');
		temp.Copy(piece);
		piece.Copy(*holdp);
		holdp->Copy(temp);
	}
}

int main()
{
	while (!gameOver)
	{
		this_thread::sleep_for(50ms);
		speedCounter++;

		// INPUT
		int dx = input.OnKeyHeld(input.Right)
			   - input.OnKeyHeld(input.Left), 
			dy = input.OnKeyHeld(input.Down), 
			dr = input.OnKeyPressed(input.Rotate);

		TryMovePiece(dx, dy, dr);

		if (input.OnKeyPressed(input.Hold))
			HoldPiece();

		// GHOST PIECE
		while (field.DoesPieceFit(ghost))
			ghost.Move(0, 1);
		ghost.Move(0, -1);

		// DROP
		if (input.OnKeyPressed(input.Drop))
		{
			piece.Copy(ghost);
			PlacePiece();
		}

		// FORCE MOVE
		if (speedCounter >= speed)
		{
			speedCounter = 0;
 			if (!TryMovePiece(0,1,0))
				PlacePiece();
		}

		// RENDER
		renderer.Render(field);
		renderer.Render(ghost);
		renderer.Render(piece);

		// Next
		renderer.RenderText(L"Next:", 20, 0);
		renderer.RenderBlock(L"╔════╗║    ║║    ║║    ║║    ║╚════╝", 6, 6, 20, 1);
		renderer.Render(nextp, 21, 2);

		// Hold
		renderer.RenderText(L"Hold:", 20, 10);
		renderer.RenderBlock(L"╔════╗║    ║║    ║║    ║║    ║╚════╝", 6, 6, 20, 11);
		if (holdp) renderer.Render(*holdp, 21, 12);

		// Score
		renderer.RenderText(L"Score: " + to_wstring(score), 0, 20);

		renderer.Render();
	}
	renderer.Close();

	cout << "GAME OVER!" << endl;
	cout << "Final score: " << score << endl;

	system("pause");
	return 0;
}