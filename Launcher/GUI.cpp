#include "stdafx.h"

GUI::GUI(int columns, int lines)
{
	this->lines = lines;
	this->cols = columns;

	char buffer[32];
	sprintf(buffer, "mode con cols=%d lines=%d", columns, lines);
	system(buffer);

	SetConsoleTitle("Launcher");
	hWnd = GetConsoleWindow();
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX));
}

void GUI::SetColor(int color) const
{
	SetConsoleTextAttribute(handle, color);
}

void GUI::SetCursorPosition(COORD position) const
{
	SetConsoleCursorPosition(handle, position);
}

COORD GUI::GetCursorPosition() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(handle, &csbi))
		return { 0, 0 };

	return csbi.dwCursorPosition;
}