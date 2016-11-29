/*
ALPHA V0.5

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

#include "Tetris.h"

namespace TetrisGame
{
	HANDLE consoleScreen, consoleInput, oldConsoleScreen;
	DWORD previousConsoleInputConfiguration;
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;

	const unsigned int cFrameTimeBase = 64;
	unsigned int frameTimeTarget = 0, frameStart = 0, frameEnd = 0, frameTime = 0;
	
	void Draw()
	{

	}

	void Update()
	{

	}

	void TurnBlock(int direction)
	{

	}

	void HandleInput()
	{
		if (GetAsyncKeyState(VK_LEFT))
		{
			TurnBlock(-1); // counter clockwise
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			TurnBlock(1); // clock wise
		}
		else if (GetAsyncKeyState(VK_UP))
		{
			frameTimeTarget = cFrameTimeBase * 2;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			frameTimeTarget = cFrameTimeBase / 2;
		}
	}

	void Loop()
	{
		frameStart = clock();
		
		HandleInput();
		Update();
		Draw();
		
		frameEnd = clock();
		frameTime = frameEnd - frameStart;
		if (frameTime < frameTimeTarget)
		{
			Sleep(frameTimeTarget - frameTime);
		}
	}

	bool Setup()
	{
		if (!ConsoleFunc::SetupConsole(consoleScreen, consoleScreenBufferInfo, oldConsoleScreen))
		{
			std::cerr << "Something went wrong setting up the console screen.\n";
			return false;
		}
		if (!ConsoleFunc::EnableConsoleInput(consoleInput, previousConsoleInputConfiguration))
		{
			std::cerr << "Could not enable console input, exiting.\n";
			ConsoleFunc::DestroyInputHandle(consoleInput, previousConsoleInputConfiguration);
			ConsoleFunc::DestroyConsole(consoleScreen, oldConsoleScreen);
			return false;
		}
		return true;
	}

	void Exit()
	{
		if (ConsoleFunc::DestroyInputHandle(consoleInput, previousConsoleInputConfiguration) 
			&& ConsoleFunc::DestroyConsole(consoleScreen, oldConsoleScreen))
		{
			return;
		}
		std::cerr << "Something went wrong exiting, possible handles left open!\n";
	}
	
	void Play()
	{
		Setup();
		Loop();
		Exit();
	}
}
