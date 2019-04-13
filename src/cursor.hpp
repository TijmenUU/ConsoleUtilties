#pragma once
#include <iostream>
#include <Windows.h>

namespace Cursor
{
	void Set(const COORD& position);

	COORD Get();
}

std::ostream& operator <<(std::ostream& outputStream, const COORD& cursor);

std::ostream& operator >>(std::ostream& outputStream, COORD& cursor);