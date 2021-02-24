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
	usingHeld = false;
	field.Place(piece);
	NextPiece();
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
	bool gameOver = false;
	int speed = 20;
	int speedCounter = 0;

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
		if (speedCounter == speed)
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
		renderer.RenderBlock(L"Next:", 5, 1, 20, 0);
		renderer.RenderBlock(L"╔════╗║    ║║    ║║    ║║    ║╚════╝", 6, 6, 20, 1);
		renderer.Render(nextp, 21, 2);

		// Hold
		renderer.RenderBlock(L"Hold:", 5, 1, 20, 10);
		renderer.RenderBlock(L"╔════╗║    ║║    ║║    ║║    ║╚════╝", 6, 6, 20, 11);
		if (holdp) renderer.Render(*holdp, 21, 12);

		renderer.Render();
	}

	#pragma region game stuff
	// Game

	//bool forceMove = false;
	//int piecesPlaced = 0;
	//int score = 0;

	//vector<int> lines;

	//while (!gameOver)
	//{
	//	// Timing
	//	this_thread::sleep_for(50ms);
	//	speedCounter++;
	//	forceMove = (speed == speedCounter);

	//	// Input
	//	for (int k = 0; k < 4; k++)								// R   L   D Z
	//		key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

	//	// Move
	//	if (key[1] && DoesPieceFit(curPiece, curRot, curX - 1, curY)) curX--; // L
	//	if (key[0] && DoesPieceFit(curPiece, curRot, curX + 1, curY)) curX++; // R
	//	if (key[2] && DoesPieceFit(curPiece, curRot, curX, curY + 1)) curY++; // D

	//	// Rotate
	//	if (key[3])
	//	{
	//		if (!rotateHeld && DoesPieceFit(curPiece, curRot + 1, curX, curY)) 
	//			curRot++;
	//		rotateHeld = true;
	//	}
	//	else rotateHeld = false;

	//	// Force move
	//	if (forceMove)
	//	{
	//		if (DoesPieceFit(curPiece, curRot, curX, curY + 1)) curY++;
	//		else
	//		{
	//			// Place piece
	//			for (int px = 0; px < 4; px++)
	//				for (int py = 0; py < 4; py++)
	//					// if solid part of piece, update field
	//					if (tetromino[curPiece][Rotate(px, py, curRot)] == L'X')
	//						field[(curY + py) * fieldWidth + (curX + px)] = curPiece + 1; //0=emptyspace
	//			
	//			piecesPlaced++;
	//			if (piecesPlaced % 10 == 0)
	//				speed -= speed < 10? 0 : 1;

	//			// Check for lines
	//			for (int py = 0; py < 4; py++)
	//				if (curY + py < fieldHeight - 1)
	//				{
	//					// Check if every position in line is filled
	//					bool line = true;
	//					for (int px = 1; px < fieldWidth - 1; px++)
	//						line &= (field[(curY + py) * fieldWidth + px]) != 0;

	//					// If there's a line
	//					if (line)
	//					{
	//						for (int px = 1; px < fieldWidth - 1; px++)
	//							field[(curY + py) * fieldWidth + px] = 8; //=
	//						
	//						lines.push_back(curY+py); // lets us delay the game when lines are clearing (to show player)
	//					}
	//				}

	//			// Scoring
	//			score += 25;
	//			if (!lines.empty()) score += (1 << lines.size()) * 100;

	//			// Choose next piece
	//			curPiece = rand() % 7;
	//			curX = fieldWidth / 2;
	//			curY = 0;
	//			curRot = 0;
	//			
	//			// Check if game over
	//			gameOver = !DoesPieceFit(curPiece, curRot, curX, curY + 1);
	//		}

	//		// Reset speedcounter
	//		speedCounter = 0;
	//	}

		//// Draw Score
		//swprintf_s(&screen[2 * screenWidth + fieldWidth + 6], 16, L"SCORE: %8d", score);

		//// Draw Line-removals
		//if (!lines.empty())
		//{
		//	// Draw Line Score
		//	swprintf_s(&screen[6 * screenWidth + fieldWidth + 6], 16, L"%8d lines!", lines.size());
		//	
		//	// Display Frame
		//	WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
		//	// Wait a bit
		//	this_thread::sleep_for(400ms);

		//	// Remove line score
		//	swprintf_s(&screen[6 * screenWidth + fieldWidth + 6], 16, L"               ");

		//	// Remove lines
		//	for (auto &v : lines)
		//		for (int px = 1; px < fieldWidth-1; px++)
		//		{
		//			// set row to row above
		//			for (int py = v; py > 0; py--)
		//				field[py * fieldWidth + px] = field[(py - 1) * fieldWidth + px];
		//			
		//			// set top row to empty 
		//			field[px] = 0;
		//		}

		//	lines.clear();
		//}

	//}

	//CloseHandle(hConsole);
	//cout << "GAME OVER!" << endl;
	//cout << "Final score: " << score << endl;
	//system("pause");
	#pragma endregion
	
	return 0;
}