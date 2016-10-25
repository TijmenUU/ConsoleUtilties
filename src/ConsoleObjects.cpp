#ifndef CONSOLEOBJECTS_CPP
#define CONSOLEOBJECTS_CPP
#pragma once

/*
ALPHA V0.3

Copyright (c) 2016  Tijmen van Nesselrooij

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ConsoleObjects.h"

namespace ConsoleObject
{
#pragma region Cin Cout
	Cout::Cout()
	{
		{
			previousOutputHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);

			if (!ConsoleBasic::InitConsole(screenBuffer_))
			{
				throw std::exception("Cout: Failed to create a console window.");
			}
			if (!ConsoleBasic::SetScreenBuffer(screenBuffer_))
			{
				CloseHandle(screenBuffer_);
				throw std::exception("Cout: Failed to set screenbuffer to active.");
			}
			if (!ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_))
			{
				CloseHandle(screenBuffer_);
				throw std::exception("Cout: Failed to get screenbuffer info.");
			}

			SetFontColor(ConsoleBasic::FONTCOLOR::WHITE);
			SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR::BLACK);
		}
	}

	Cout::~Cout()
	{
		if (!CloseHandle(screenBuffer_))
		{
			std::cerr << "Cout: Failed to clean up! Possible open handles left!\n";
		}
		else
		{
			SetStdHandle(STD_OUTPUT_HANDLE, previousOutputHandle_);
		}
	}

	void Cout::SetFontColor(ConsoleBasic::FONTCOLOR color)
	{
		currentFontColor_ = static_cast<int>(color);
	}

	void Cout::SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR color)
	{
		currentBGColor_ = static_cast<int>(color);
	}

	void Cout::Print(const char * message, const int messageSize)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const wchar_t * message, const int messageSize)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::string message)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::wstring message)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const char * message, const int messageSize, DWORD premixedColors)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, premixedColors);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const wchar_t * message, const int messageSize, DWORD premixedColors)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, premixedColors);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::string message, DWORD premixedColors)
	{
		SetConsoleTextAttribute(screenBuffer_, premixedColors);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::wstring message, DWORD premixedColors)
	{
		SetConsoleTextAttribute(screenBuffer_, premixedColors);
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const char * message, const int messageSize, const ConsoleBasic::FONTCOLOR fontColor, const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
	{
		SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const wchar_t * message, const int messageSize, const ConsoleBasic::FONTCOLOR fontColor, const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
	{
		SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
		ConsoleBasic::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::string message, const ConsoleBasic::FONTCOLOR fontColor, const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
	{
		SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::wstring message, const ConsoleBasic::FONTCOLOR fontColor, const ConsoleBasic::BACKGROUNDCOLOR backgroundColor)
	{
		SetConsoleTextAttribute(screenBuffer_, (static_cast<int>(fontColor) | static_cast<int>(backgroundColor)));
		ConsoleBasic::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleBasic::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	Cout & Cout::operator<<(const char s)
	{
		Print(&s, 1);
		return *this;
	}

	Cout & Cout::operator<<(const wchar_t s)
	{
		Print(&s, 1);
		return *this;
	}

	Cout & Cout::operator<<(const char * s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator<<(const wchar_t * s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator<<(const std::string & s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator<<(const std::wstring & s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator<<(std::istream & in)
	{
		Print(std::string(std::istreambuf_iterator<char>(in), {}));
		return *this;
	}

	void Cout::SetCursorPosition(const int x, const int y)
	{
		assert(x > 0 && y > 0 && x <= screenBufferInfo_.dwSize.X && y <= screenBufferInfo_.dwSize.Y);

		screenBufferInfo_.dwCursorPosition.X = x;
		screenBufferInfo_.dwCursorPosition.Y = y;
	}

	void Cout::SetCursorPosition(const COORD position)
	{
		assert(position.X > 0 && position.Y > 0 && position.X <= screenBufferInfo_.dwSize.X && position.Y <= screenBufferInfo_.dwSize.Y);

		screenBufferInfo_.dwCursorPosition = position;
	}

	COORD Cout::GetCursorPosition() const
	{
		return screenBufferInfo_.dwCursorPosition;
	}

	void Cout::GetCursorPosition(int & x, int & y)
	{
		x = screenBufferInfo_.dwCursorPosition.X;
		y = screenBufferInfo_.dwCursorPosition.Y;
	}

	HANDLE Cout::_GetOutputHandle() const
	{
		return screenBuffer_;
	}

	CONSOLE_SCREEN_BUFFER_INFO Cout::GetConsoleScreenBufferInfo() const
	{
		return screenBufferInfo_;
	}
#pragma endregion
#pragma region Menu Objects
	Menu::~Menu()
	{
	}

	void Menu::NextItem(const HANDLE outputHandle)
	{
		menuItems_[currentItem_].Deselect(outputHandle);
		++currentItem_;
		currentItem_ = (currentItem_ >= menuItems_.size()) ? 0 : currentItem_;
		menuItems_[currentItem_].Select(outputHandle);
	}

	void Menu::PreviousItem(const HANDLE outputHandle)
	{
		menuItems_[currentItem_].Deselect(outputHandle);
		--currentItem_;
		currentItem_ = (currentItem_ < 0) ? 0 : currentItem_;
		menuItems_[currentItem_].Select(outputHandle);
	}

	MenuItem::MenuItem(std::string description)
	{
		description_ = description;
	}

	MenuItem::~MenuItem()
	{

	}

	std::string MenuItem::GetDescription() const
	{
		return description_;
	}

	void MenuItem::SetPositionX(const int positionX)
	{
		positionX_ = positionX;
	}

	void MenuItem::SetPositionY(const int positionY)
	{
		positionY_ = positionY;
	}

	int MenuItem::GetPositionX() const
	{
		return positionX_;
	}

	int MenuItem::GetPositionY() const
	{
		return positionY_;
	}
#pragma endregion
}

#endif // !CONSOLEOBJECTS_CPP
