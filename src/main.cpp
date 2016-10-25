#ifndef MAIN_CPP
#define MAIN_CPP
#pragma once

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

#include "ConsoleObjects.h"

int main(int argc, char** argv)
{
	ConsoleObject::Cout cout;
	HANDLE inputHandle;
	DWORD inputConfigBackup;

	ConsoleBasic::EnableConsoleInput(inputHandle, inputConfigBackup);

	cout.SetFontColor(ConsoleBasic::FONTCOLOR::GREEN);
	cout.SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR::DARKBLUE);

	std::string input = ConsoleBasic::GetLineVisualized(cout._GetOutputHandle(),
	inputHandle,
	cout.GetConsoleScreenBufferInfo());

	cout.SetFontColor(ConsoleBasic::FONTCOLOR::RED);
	cout.SetBackgroundColor(ConsoleBasic::BACKGROUNDCOLOR::DARKYELLOW);

	cout << input << "\n\n" << "Press any key to exit...";

	ConsoleBasic::GetChar(inputHandle);

	return 0;
}

#endif // !MAIN_CPP
