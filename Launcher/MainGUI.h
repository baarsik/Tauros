#pragma once

enum MainGUIStatus
{
	STATUS_BIN_NOT_CHECKED,
	STATUS_BIN_NOTFOUND,
	STATUS_BIN_OUTDATED,
	STATUS_BIN_CHECKING,
	STATUS_BIN_UPDATED,
	STATUS_GAME_NOTFOUND,
	STATUS_GAME_INJECTFAILED,
	STATUS_GAME_INJECTED
};

class MainGUI : GUI
{
public:
	MainGUI();
	void            WriteLine(char* text, GUILineStatus status = STATUS_NORMAL);
	void            SetStatus(MainGUIStatus status);
	MainGUIStatus   GetGameStatus() const;
private:
	size_t          consoleMaxLines;
	std::vector<GUILine> guiLines;
	MainGUIStatus   gameStatus;

	void            DrawHeader() const;
	void            DrawConsole() const;
};
