#pragma once

class GUI
{
public:
	GUI(int columns, int lines);
	virtual ~GUI() = 0 {}
protected:
	short           lines;
	short           cols;
	HWND            hWnd;
	HANDLE          handle;

	void            SetColor(int color) const;
	void            SetCursorPosition(COORD position) const;
	COORD           GetCursorPosition() const;
};