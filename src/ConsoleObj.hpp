#ifndef CONSOLEOBJ_H
#define CONSOLEOBJ_H
#pragma once 

/*
	ALPHA V0.4

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

#include "ConsoleFunc.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <Windows.h>

namespace ConsoleObj
{
#pragma region Cin Cout
	// Similar to std::cout. It handles all the HANDLE creation
	// and necessary initializations. After the constructor has
	// been called it can be used.
	class Cout
	{
	protected:
		int currentFontColor_;
		int currentBGColor_;
		HANDLE screenBuffer_, previousOutputHandle_;
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo_;
	public:
		// Throws an std::exception if something went wrong
		Cout();
		// Restores the original i/o handles and closes the used ones
		// Throws and std::exception if something went wrong
		~Cout();

		void SetFontColor(ConsoleFunc::FONTCOLOR color);
		void SetBackgroundColor(ConsoleFunc::BACKGROUNDCOLOR color);

		// Classical Print behaviour, prints using stored color info
		void Print(const char * message, const int messageSize);
		// Classical Print behaviour, prints using stored color info
		void Print(const wchar_t * message, const int messageSize);
		// Classical Print behaviour, prints using stored color info
		void Print(const std::string message);
		// Classical Print behaviour, prints using stored color info
		void Print(const std::wstring message);
		// Classical Print behaviour at a custom location. Does not affect the cursor position!
		void Print(const char * message, const int messageSize, const COORD printingLocation);
		// Classical Print behaviour at a custom location. Does not affect the cursor position!
		void Print(const wchar_t * message, const int messageSize, const COORD printingLocation);
		// Classical Print behaviour at a custom location. Does not affect the cursor position!
		void Print(const std::string message, const COORD printingLocation);
		// Classical Print behaviour at a custom location. Does not affect the cursor position!
		void Print(const std::wstring message, const COORD printingLocation);
		
		// Use this to update the cursor of COUT after you have done printing
		// without using this Object (instance)
		void UpdateCursor();

		// Handle input streams <<

		Cout& operator << (const char s);
		Cout& operator << (const wchar_t s);
		Cout& operator << (const char * s);
		Cout& operator << (const wchar_t* s);
		Cout& operator << (const std::string &s);
		Cout& operator << (const std::wstring &s);
		Cout& operator << (std::istream &in);
		// TODO differentiate between ascii and unicode strings

		// Getters and setters

		void SetCursorPosition(const int x, const int y);
		void SetCursorPosition(const COORD position);

		COORD GetCursorPosition() const;
		void GetCursorPosition(int &x, int &y);

		// Do not use this if you don't know what you're doing
		HANDLE _GetOutputHandle() const;

		CONSOLE_SCREEN_BUFFER_INFO GetConsoleScreenBufferInfo() const;
	};
#pragma endregion
#pragma region Menu Objects
	/*
		These objects are meant to be overridden. Hence it provides no callback construct.
		If you want your radio button to do something specific, create a new class, inherit
		from the RadioButtonItem and override to your hearts desire! Or create complete new
		wacky things by inheriting from MenuItem.
	*/
	
	class MenuItem
	{
	protected:
		int positionX_, positionY_;
		std::string description_;
		ConsoleFunc::FONTCOLOR fontColor_;
		ConsoleFunc::BACKGROUNDCOLOR backgroundColor_;
	public:
		MenuItem(std::string description);
		~MenuItem();
		virtual void Click(const HANDLE outputHandle); // Mouse specific event handler
		virtual void Select(const HANDLE outputHandle); // Hovering over the item
		virtual void Deselect(const HANDLE outputHandle); // Stop hovering over the item
		virtual void Activate(const HANDLE outputHandle); // Keyboard or mouse activation on item
		virtual void Draw(const HANDLE outputHandle);
		std::string GetDescription() const;
		void SetPositionX(const int positionX);
		void SetPositionY(const int positionY);
		int GetPositionX() const;
		int GetPositionY() const;
	};

	enum class MENUITEM_ORDER { HORIZONTAL, VERTICAL};

	class Menu
	{
	protected:
		std::vector<MenuItem> menuItems_;
		MENUITEM_ORDER displayOrder_;
		int currentItem_;
		int positionX_;
		int positionY_;
	public:
		Menu(const MENUITEM_ORDER displayOrder = MENUITEM_ORDER::VERTICAL, const int positionX = 0, const int positionY = 0);
		~Menu();
		// Highlight the next item (wraps around)
		void NextItem(const HANDLE outputHandle);
		// Highlight the previous item (wraps around)
		void PreviousItem(const HANDLE outputHandle);
		// Insert a new MenuItem, -1 means insert as last, 0 is first
		void InsertItem(const MenuItem item, const int location = -1); 
		// Delete the menu item from given position
		bool DeleteItem(const int location); 
		void HandleKeybInput(const INPUT_RECORD key, const HANDLE outputHandle);
		void HandleMouseInput(const INPUT_RECORD mouseAction, const HANDLE outputHandle);
		void Draw(const HANDLE outputHandle);
	};

	class RadioButtonItem : MenuItem
	{
	public:
		RadioButtonItem(const std::string description);
		~RadioButtonItem();
	};
#pragma endregion
}
#endif // !CONSOLEOBJ_H
