#pragma once

std::ostream& operator <<(std::ostream& outputStream, const COORD& cursor)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

	return outputStream;
}

std::ostream& operator >>(std::ostream& outputStream, COORD& cursor)
{
	CONSOLE_SCREEN_BUFFER_INFO conInfo;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo))
	{
		cursor.X = conInfo.dwCursorPosition.X;
		cursor.Y = conInfo.dwCursorPosition.Y;
	}

	return outputStream;
}