#ifndef BREAKOUT_H
#define BREAKOUT_H
#pragma once 
#include "ConsoleFunc.h"
#include <ctime>
#include <iostream>
#include <string>
#include <Windows.h>

namespace BreakOutGame
{
	// Cleans up assets and closes the handles
	bool Exit();

	// Gameloop, called after SetupGame();
	// Call Exit after this!
	// Blocking
	void Loop();

	// Called once to init some vars
	bool Setup();
}

#endif 
