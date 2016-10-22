#ifndef CONSOLEOBJECTS_H
#define CONSOLEOBJECTS_H

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

#pragma once
#include "ConsoleUtilities.h"

namespace ConsoleObject
{
	class MenuItem
	{
	protected:
		int positionX_, positionY_;
		std::string description_;
		ConsoleBasic::FONTCOLOR fontColor_;
		ConsoleBasic::BACKGROUNDCOLOR backgroundColor_;
	public:
		MenuItem(std::string description);
		~MenuItem();
		virtual void Click(const HANDLE outputHandle); // Mouse specific event handler
		virtual void Select(const HANDLE outputHandle); // Hovering over the item
		virtual void Deselect(const HANDLE outputHandle); // Stop hovering over the item
		virtual void Activate(const HANDLE outputHandle); // Keyboard or mouse activation on item
		std::string GetDescription() const
		{
			return description_;
		}
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
		RadioButtonItem();
		~RadioButtonItem();
	};
}
#endif // !CONSOLEOBJECTS_H
