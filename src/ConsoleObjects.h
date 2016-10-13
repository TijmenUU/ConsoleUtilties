#ifndef CONSOLEOBJECTS_H
#define CONSOLEOBJECTS_H

/*
ALPHA V0.2

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

#pragma once
#include "..\ConsoleUtilities.h"

namespace ConsoleObject
{
	class MenuItem
	{
	protected:
		std::string description_;
	public:
		MenuItem(std::string description);
		~MenuItem();
		virtual void Click(const HANDLE outputHandle); // Mouse specific event handler
		virtual void Select(const HANDLE outputHandle); // Hovering over the item
		virtual void Deselect(const HANDLE outputHandle); // Stop hovering over the item
		virtual void Activate(const HANDLE outputHandle); // Keyboard or mouse activation on item
		std::string GetDescription() const;
	};

	enum MENUITEM_ORDER { HORIZONTAL, VERTICAL, DIAGONAL_TOPLEFT, DIAGONAL_TOPRIGHT };

	class Menu
	{
	protected:
		std::vector<MenuItem> menuItems_;
		MENUITEM_ORDER displayOrder_;
		int currentItem_;
		int positionX_;
		int positionY_;
	public:
		Menu(const MENUITEM_ORDER displayOrder, const int positionX = 0, const int positionY = 0);
		~Menu();
		void NextItem(const HANDLE outputHandle);
		void PreviousItem(const HANDLE outputHandle);
		void InsertItem(const MenuItem item, const int location = -1); // Insert a new MenuItem, -1 means insert as last
		bool DeleteItem(const int location); // Delete the menu item from given position
		void HandleKeyInput(const HANDLE outputHandle, const HANDLE inputHandle);
		void HandleMouseInput(const HANDLE outputHandle, const HANDLE inputHandle);
		void Draw(const HANDLE outputHandle);
	};

	class DisplayItem : MenuItem
	{
	public:
		DisplayItem();
		~DisplayItem();
	};

	class RadioButtonItem : MenuItem
	{
	public:
		RadioButtonItem();
		~RadioButtonItem();
	};
}
#endif // !CONSOLEOBJECTS_H
