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
#include "ConsoleObjects.h"

namespace ConsoleObject
{
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
}
