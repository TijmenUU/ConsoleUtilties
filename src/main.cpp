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

#include <conio.h>
#include "ConsoleObj.h"

void DemonstrateCout()
{
	ConsoleObj::Cout cout;
	HANDLE inputHandle;
	DWORD inputConfigBackup;

	ConsoleFunc::EnableConsoleInput(inputHandle, inputConfigBackup);

	cout.SetFontColor(ConsoleFunc::FONTCOLOR::GREEN);
	cout.SetBackgroundColor(ConsoleFunc::BACKGROUNDCOLOR::DARKBLUE);

	COORD oddCoordinate;
		oddCoordinate.X = 50;
		oddCoordinate.Y = 10;
	cout.Print("Odd printing location", oddCoordinate); // You can print things in odd places

	cout << "Give me something to echo: ";
	std::string input = ConsoleFunc::GetLineVisualized(cout._GetOutputHandle(),
		inputHandle,
		cout.GetConsoleScreenBufferInfo());
	cout.UpdateCursor(); // Since Get Line Visualized prints our keystrokes we need to do this
	cout << "\nYou wrote: "; // Goto next line

	cout.SetFontColor(ConsoleFunc::FONTCOLOR::YELLOW);
	cout.SetBackgroundColor(ConsoleFunc::BACKGROUNDCOLOR::DARKGREEN);

	cout << input << "\n\n" << "Press any key to continue...";

	ConsoleFunc::GetChar(inputHandle);
	ConsoleFunc::DeleteInputHandle(inputHandle, inputConfigBackup); // Restore input so that you can use std::cin again
}

int main(int argc, char ** argv)
{
	DemonstrateCout();

	std::cout << "And all was normal again. Press any key to exit...";
	_getch();

	return 0;
}
