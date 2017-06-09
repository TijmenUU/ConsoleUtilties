/*
ALPHA V0.5

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

#include "ConsoleObj.h"

namespace ConsoleObj
{
#pragma region Cin Cout
	Cout::Cout()
	{
		if (!ConsoleFunc::InitConsole(screenBuffer_, previousOutputHandle_))
		{
			throw std::runtime_error("Cout: Failed to create a console window.");
		}
		if (!ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_))
		{
			CloseHandle(screenBuffer_);
			throw std::runtime_error("Cout: Failed to get screenbuffer info.");
		}

		SetFontColor(ConsoleFunc::FONTCOLOR::WHITE);
		SetBackgroundColor(ConsoleFunc::BACKGROUNDCOLOR::BLACK);
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

	void Cout::SetFontColor(ConsoleFunc::FONTCOLOR color)
	{
		currentFontColor_ = static_cast<int>(color);
	}

	void Cout::SetBackgroundColor(ConsoleFunc::BACKGROUNDCOLOR color)
	{
		currentBGColor_ = static_cast<int>(color);
	}

#pragma region Print
	void Cout::Print(const char * message, const int messageSize)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const wchar_t * message, const int messageSize)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, messageSize, screenBufferInfo_.dwCursorPosition);
		ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::string message)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const std::wstring message)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, screenBufferInfo_.dwCursorPosition);
		ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

	void Cout::Print(const char * message, const int messageSize, const COORD printingLocation)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, messageSize, printingLocation);
	}

	void Cout::Print(const wchar_t * message, const int messageSize, const COORD printingLocation)
	{
		assert(messageSize > 0);

		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, messageSize, printingLocation);
	}

	void Cout::Print(const std::string message, const COORD printingLocation)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, printingLocation);
	}

	void Cout::Print(const std::wstring message, const COORD printingLocation)
	{
		SetConsoleTextAttribute(screenBuffer_, currentFontColor_ | currentBGColor_);
		ConsoleFunc::WriteToConsole(screenBuffer_, message, printingLocation);
	}
#pragma endregion

	void Cout::UpdateCursor()
	{
		ConsoleFunc::GetConsoleInfo(screenBuffer_, screenBufferInfo_);
	}

#pragma region <<
	Cout & Cout::operator << (const char s)
	{
		Print(&s, 1);
		return *this;
	}

	Cout & Cout::operator << (const wchar_t s)
	{
		Print(&s, 1);
		return *this;
	}

	Cout & Cout::operator << (const char * s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator << (const wchar_t * s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator << (const std::string & s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator << (const std::wstring & s)
	{
		Print(s);
		return *this;
	}

	Cout & Cout::operator << (std::istream & in)
	{
		Print(std::string(std::istreambuf_iterator<char>(in), {}));
		return *this;
	}

	Cout & Cout::operator << (const COORD printLocation)
	{
		SetCursorPosition(printLocation);
		return *this;
	}

	Cout & Cout::operator << (const ConsoleFunc::FONTCOLOR color)
	{
		SetFontColor(color);
		return *this;
	}

	Cout & Cout::operator << (const ConsoleFunc::BACKGROUNDCOLOR color)
	{
		SetBackgroundColor(color);
		return *this;
	}

#pragma endregion
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
	Menu::Menu(const MENUITEM_ORDER displayOrder, const int positionX, const int positionY)
	{
		displayOrder_ = displayOrder;
		positionX_ = positionX;
		positionY_ = positionY;
	}

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

	void Menu::InsertItem(const MenuItem item, const int location)
	{
	}

	bool Menu::DeleteItem(const int location)
	{
		return false;
	}

	void Menu::HandleKeybInput(const INPUT_RECORD key, const HANDLE outputHandle)
	{
	}

	void Menu::HandleMouseInput(const INPUT_RECORD mouseAction, const HANDLE outputHandle)
	{
	}

	void Menu::Draw(const HANDLE outputHandle)
	{
	}

	MenuItem::MenuItem(std::string description)
	{
		description_ = description;
	}

	MenuItem::~MenuItem()
	{

	}

	void MenuItem::Click(const HANDLE outputHandle)
	{
	}

	void MenuItem::Select(const HANDLE outputHandle)
	{
	}

	void MenuItem::Deselect(const HANDLE outputHandle)
	{
	}

	void MenuItem::Activate(const HANDLE outputHandle)
	{
	}

	void MenuItem::Draw(const HANDLE outputHandle)
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
	RadioButtonItem::RadioButtonItem(const std::string description)
		: MenuItem(description)
	{
	}
	RadioButtonItem::~RadioButtonItem()
	{
	}
}
