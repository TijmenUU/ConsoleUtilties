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
}
