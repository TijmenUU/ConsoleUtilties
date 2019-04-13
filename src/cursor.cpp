#include "cursor.hpp"

namespace Cursor
{
	void Set(const COORD& position)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	}

	COORD Get()
	{
		COORD position{ 0, 0 };

		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
		{
			position.X = conInfo.dwCursorPosition.X;
			position.Y = conInfo.dwCursorPosition.Y;
		}

		return position;
	}
}

std::ostream& operator <<(std::ostream& outputStream, const COORD& cursor)
{
	Cursor::Set(cursor);

	return outputStream;
}

std::ostream& operator >>(std::ostream& outputStream, COORD& cursor)
{
	cursor = Cursor::Get();

	return outputStream;
}