#pragma once
#include "unicode.h"
#include <string>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
using namespace std;

class Input
{
public:
	const unsigned char Rotate = 'Z';
	const unsigned char Hold = 'C';
	const unsigned char Drop = ' ';
	const unsigned char Left = '\x25';
	const unsigned char Right = '\x27';
	const unsigned char Down = '\x28';
	bool OnKeyHeld(unsigned char key);
	bool OnKeyPressed(unsigned char key);
private:
};


/*
	wstring Left;
	wstring Right;
	wstring Down;
	wstring Rotate;
	wstring Hold;
*/
