#include "stdafx.h"

MainGUI::MainGUI() : GUI(110, 30)
{
	DrawHeader();
	DrawConsole();

	SetStatus(STATUS_BIN_NOT_CHECKED);
	SetStatus(STATUS_GAME_NOTFOUND);

	consoleMaxLines = lines - 7;
	guiLines.reserve(consoleMaxLines);
}

void MainGUI::WriteLine(char* text, GUILineStatus status)
{
	auto pos = GetCursorPosition();
	auto line = GUILine(cols - 4);
	line.Set(text, status);
	guiLines.insert(guiLines.begin(), line);
	if (guiLines.size() > consoleMaxLines)
		guiLines.erase(guiLines.end() - 1);
	auto guiLinesCount = static_cast<unsigned short>(guiLines.size());
	for (unsigned short i = 0; i < guiLinesCount; i++)
	{
		SetCursorPosition({ 2, 4 + guiLinesCount - i });
		SetColor(guiLines[i].GetStatus());
		printf(guiLines[i].Get());
		SetColor(7);
	}
	SetCursorPosition(pos);
}

void MainGUI::SetStatus(MainGUIStatus status)
{
	auto line = GUILine(cols - cols / 2 - 4);
	auto initialPos = GetCursorPosition();
	COORD destinationPos = { 0, 0 };
	switch (status)
	{
	case STATUS_BIN_NOT_CHECKED:
		destinationPos = { cols / 2 + 2, 1 };
		line.Set("Binaries are not checked", STATUS_NORMAL);
		break;
	case STATUS_BIN_NOTFOUND:
		destinationPos = { cols / 2 + 2, 1 };
		line.Set("Binaries not found", STATUS_FAIL);
		break;
	case STATUS_BIN_OUTDATED:
		destinationPos = { cols / 2 + 2, 1 };
		line.Set("Binaries are outdated", STATUS_FAIL);
		break;
	case STATUS_BIN_CHECKING:
		destinationPos = { cols / 2 + 2, 1 };
		line.Set("Binaries check in progress", STATUS_WARNING);
		break;
	case STATUS_BIN_UPDATED:
		destinationPos = { cols / 2 + 2, 1 };
		line.Set("Binaries are up to date", STATUS_SUCCESS);
		break;
	case STATUS_GAME_NOTFOUND:
		destinationPos = { cols / 2 + 2, 2 };
		line.Set("Game window is not found", STATUS_FAIL);
		this->gameStatus = status;
		break;
	case STATUS_GAME_INJECTFAILED:
		destinationPos = { cols / 2 + 2, 2 };
		line.Set("Injection failed", STATUS_FAIL);
		this->gameStatus = status;
		break;
	case STATUS_GAME_INJECTED:
		destinationPos = { cols / 2 + 2, 2 };
		line.Set("Game in running and injected", STATUS_SUCCESS);
		this->gameStatus = status;
		break;
	}
	SetCursorPosition(destinationPos);
	SetColor(line.GetStatus());
	printf(line.Get());
	SetColor(7);
	SetCursorPosition(initialPos);
}

MainGUIStatus MainGUI::GetGameStatus() const
{
	return this->gameStatus;
}

void MainGUI::DrawHeader() const
{
	SetColor(11);
	printf("\xDA\xC4\xC4(");
	SetColor(13);
	printf("Information");
	SetColor(11);
	printf(")");
	for (auto i = 16; i < cols / 2; i++)
		printf("\xC4");
	printf("\xC2\xC4\xC4(");
	SetColor(13);
	printf("Status");
	SetColor(11);
	printf(")");
	for (auto i = cols / 2 + 11; i < cols - 1; i++)
		printf("\xC4");
	printf("\xBF\xB3 ");
	SetColor(10);
	printf("Cerberus");
	SetColor(7);
	printf(".");
	SetColor(14);
	printf("Launcher");
	SetColor(7);
	printf(" (%s)", __DATE__);
	SetColor(11);
	SetCursorPosition({ cols / 2, 1 });
	printf("\xB3");
	SetCursorPosition({ cols - 1, 1 });
	printf("\xB3\xB3 ");
	SetColor(7);
	printf("Coded by ");
	setlocale(LC_ALL, "Russian");
	printf("Барс");
	setlocale(LC_ALL, "C");
	printf(" (Skype baarsik2)");
	SetColor(11);
	SetCursorPosition({ cols / 2, 2 });
	printf("\xB3");
	SetCursorPosition({ cols - 1, 2 });
	printf("\xB3\xC0");
	for (auto i = 1; i < cols / 2; i++)
		printf("\xC4");
	printf("\xC1");
	for (auto i = cols / 2 + 1; i < cols - 1; i++)
		printf("\xC4");
	printf("\xD9");
	SetColor(7);
}

void MainGUI::DrawConsole() const
{
	SetCursorPosition({ 0, 4 });
	SetColor(11);
	printf("\xDA\xC4\xC4(");
	SetColor(13);
	printf("Console");
	SetColor(11);
	printf(")");
	for (auto i = 12; i < cols - 1; i++)
		printf("\xC4");
	printf("\xBF");
	for (short i = 5; i < lines - 2; i++)
	{
		SetCursorPosition({ 0, i });
		printf("\xB3");
		SetCursorPosition({ cols - 1, i });
		printf("\xB3");
	}
	printf("\xC0");
	for (auto i = 1; i < cols - 1; i++)
		printf("\xC4");
	printf("\xD9");
	SetColor(10);
	printf("> ");
	SetColor(7);
}