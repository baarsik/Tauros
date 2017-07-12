#pragma once

enum GUILineStatus : int
{
	STATUS_NORMAL = 7,
	STATUS_SUCCESS = 10,
	STATUS_FAIL = 12,
	STATUS_WARNING = 14
};

class GUILine
{
public:
	explicit GUILine(int width);
	~GUILine();

	void            Set(char* text, GUILineStatus status);
	char*           Get() const;
	GUILineStatus   GetStatus() const;
private:
	bool            bInitialized;
	char*           text;
	size_t          width;
	GUILineStatus   status;

	void            FitToWidth();
};