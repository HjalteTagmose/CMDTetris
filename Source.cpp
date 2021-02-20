#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>

using namespace std;

int screenWidth = 80;
int screenHeight = 30;

wstring tetromino[7];
int fieldWidth = 12;
int fieldHeight = 18;
unsigned char* field = nullptr;

// gets rotated index
int Rotate(int px, int py, int r)
{
	switch (r%4)
	{
	case 0: return py * 4 + px;
	case 1: return 12 + py - (px * 4);
	case 2: return 15 - (py * 4) - px;
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}

bool DoesPieceFit(int pieceType, int rotation, int posX, int posY)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Index of the position (to check if this bit is part of the piece or not)
			int pi = Rotate(px, py, rotation);

			// Get field index (y*w+x) (posX/Y is top left corner)
			int fi = (posY + py) * fieldWidth + (posX + px);

			// Check that we're within bounds
			if (posX + px >= 0 && posX + px < fieldWidth)
			{
				if (posY + py >= 0 && posY + py < fieldHeight)
				{
					// Get piece and check if colliding
					if (tetromino[pieceType][pi] == L'X' && field[fi] != 0)
						return false;
				}
			}
		}
}

int main()
{
	#pragma region Tetrimino
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

	// Screen buffer
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	
	// Clear screen
	for (int i = 0; i < screenWidth * screenHeight; i++)
		screen[i] = L' ';

	// Get handle to console screen buffer
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Field buffer
	field = new unsigned char[fieldWidth * fieldHeight];

	// Create border
	for (int x = 0; x < fieldWidth; x++)
		for (int y = 0; y < fieldHeight; y++)
			field[y * fieldWidth + x] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0; // sides/bottom

	// Game
	bool key[4];
	bool rotateHeld = false;
	bool gameOver = false;

	int curPiece = 1;
	int curRot = 0;
	int curX = fieldWidth / 2;
	int curY = 0;

	int speed = 20;
	int speedCounter = 0;
	bool forceMove = false;
	int piecesPlaced = 0;
	int score = 0;

	vector<int> lines;

	while (!gameOver)
	{
		// Timing
		this_thread::sleep_for(50ms);
		speedCounter++;
		forceMove = (speed == speedCounter);

		// Input
		for (int k = 0; k < 4; k++)								// R   L   D Z
			key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// Move
		if (key[1] && DoesPieceFit(curPiece, curRot, curX - 1, curY)) curX--; // L
		if (key[0] && DoesPieceFit(curPiece, curRot, curX + 1, curY)) curX++; // R
		if (key[2] && DoesPieceFit(curPiece, curRot, curX, curY + 1)) curY++; // D

		// Rotate
		if (key[3])
		{
			if (!rotateHeld && DoesPieceFit(curPiece, curRot + 1, curX, curY)) 
				curRot++;
			rotateHeld = true;
		}
		else rotateHeld = false;

		// Force move
		if (forceMove)
		{
			if (DoesPieceFit(curPiece, curRot, curX, curY + 1)) curY++;
			else
			{
				// Place piece
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						// if solid part of piece, update field
						if (tetromino[curPiece][Rotate(px, py, curRot)] == L'X')
							field[(curY + py) * fieldWidth + (curX + px)] = curPiece + 1; //0=emptyspace
				
				piecesPlaced++;
				if (piecesPlaced % 10 == 0)
					speed -= speed < 10? 0 : 1;

				// Check for lines
				for (int py = 0; py < 4; py++)
					if (curY + py < fieldHeight - 1)
					{
						// Check if every position in line is filled
						bool line = true;
						for (int px = 1; px < fieldWidth - 1; px++)
							line &= (field[(curY + py) * fieldWidth + px]) != 0;

						// If there's a line
						if (line)
						{
							for (int px = 1; px < fieldWidth - 1; px++)
								field[(curY + py) * fieldWidth + px] = 8; //=
							
							lines.push_back(curY+py); // lets us delay the game when lines are clearing (to show player)
						}
					}

				// Scoring
				score += 25;
				if (!lines.empty()) score += (1 << lines.size()) * 100;

				// Choose next piece
				curPiece = rand() % 7;
				curX = fieldWidth / 2;
				curY = 0;
				curRot = 0;
				
				// Check if game over
				gameOver = !DoesPieceFit(curPiece, curRot, curX, curY + 1);
			}

			// Reset speedcounter
			speedCounter = 0;
		}

		// Draw Field
		for (int x = 0; x < fieldWidth; x++)
			for (int y = 0; y < fieldHeight; y++)
				screen[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[field[y * fieldWidth + x]]; //number determines character 

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				// if solid part of piece, draw to screen as letter A+pieceIndex (A=65,B=66,etc)
				if (tetromino[curPiece][Rotate(px, py, curRot)] == L'X')
					screen[(curY + py + 2) * screenWidth + (curX + px + 2)] = curPiece + 'A';

		// Draw Score
		swprintf_s(&screen[2 * screenWidth + fieldWidth + 6], 16, L"SCORE: %8d", score);

		// Draw Line-removals
		if (!lines.empty())
		{
			// Draw Line Score
			swprintf_s(&screen[6 * screenWidth + fieldWidth + 6], 16, L"%8d lines!", lines.size());
			
			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
			// Wait a bit
			this_thread::sleep_for(400ms);

			// Remove line score
			swprintf_s(&screen[6 * screenWidth + fieldWidth + 6], 16, L"               ");

			// Remove lines
			for (auto &v : lines)
				for (int px = 1; px < fieldWidth-1; px++)
				{
					// set row to row above
					for (int py = v; py > 0; py--)
						field[py * fieldWidth + px] = field[(py - 1) * fieldWidth + px];
					
					// set top row to empty 
					field[px] = 0;
				}

			lines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);
	cout << "GAME OVER!" << endl;
	cout << "Final score: " << score << endl;
	system("pause");
	
	return 0;
}