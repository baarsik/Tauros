#include "stdafx.h"
#include "GUILine.h"

GUILine::GUILine(int width)
{
	this->bInitialized = false;
	this->width = width;
	this->text = "";
	FitToWidth();
}

GUILine::~GUILine()
{

}

void GUILine::Set(char* text, GUILineStatus status)
{
	if (text == nullptr)
		return;

	this->bInitialized = true;
	this->text = text;
	this->status = status;
	FitToWidth();
}

char* GUILine::Get() const
{
	return !bInitialized ? "" : this->text;
}

GUILineStatus GUILine::GetStatus() const
{
	return this->status;
}

void GUILine::FitToWidth()
{
	std::string str;
	str.append(this->text);

	auto len = str.length();
	if (len < this->width)
		str.append(this->width - len, ' ');
	else if (len > this->width)
		str.erase(this->width, len - this->width);

	this->text = _strdup(str.c_str());
}