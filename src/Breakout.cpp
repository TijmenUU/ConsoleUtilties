#include "Breakout.h"

namespace BreakOutGame
{
#pragma region Constants
	const unsigned int frameTime_desired = 64;
	const int inputBuffer_length = 32;
	const std::string sprite_block = "#", sprite_player = "=", sprite_ball = "o";
	const int  blocks_yStart = 2, blocks_yStop = 7, blocks_width = 31;
	const std::string score_msg = "Score: ", lives_msg = "Lives: ", gameover_msg = "Game Over! Press any key to exit...";
#pragma endregion
#pragma region Variables
	HANDLE consoleScreen, consoleInput, oldScreenHandle;
	DWORD previousInputConfig;
	const DWORD color_text = ConsoleFunc::CreateColorAttributes(ConsoleFunc::FONTCOLOR::WHITE, ConsoleFunc::BACKGROUNDCOLOR::BLACK),
		color_player = ConsoleFunc::CreateColorAttributes(ConsoleFunc::FONTCOLOR::YELLOW, ConsoleFunc::BACKGROUNDCOLOR::BLACK),
		color_ball = ConsoleFunc::CreateColorAttributes(ConsoleFunc::FONTCOLOR::PURPLE, ConsoleFunc::BACKGROUNDCOLOR::BLACK),
		color_blocks = ConsoleFunc::CreateColorAttributes(ConsoleFunc::FONTCOLOR::RED, ConsoleFunc::BACKGROUNDCOLOR::DARKGREEN),
		color_bg = ConsoleFunc::CreateColorAttributes(ConsoleFunc::FONTCOLOR::BLACK, ConsoleFunc::BACKGROUNDCOLOR::BLACK);
	COORD tempCoord, scorePos, livesPos, playerPos, ballPos, ballVel;
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo;
	int playerLength = 7, playerLives = 0,
		scoreVal = 0, scoreNew = 0;
#pragma endregion

	void DrawScore()
	{
		SetConsoleTextAttribute(consoleScreen, color_text);
		std::string scoreString = score_msg;
		scoreString += std::to_string(scoreVal);
		ConsoleFunc::WriteToConsole(consoleScreen, scoreString, scorePos);
	}

	void DrawLives()
	{
		SetConsoleTextAttribute(consoleScreen, color_text);
		std::string livesString = lives_msg;
		livesString += std::to_string(playerLives);
		ConsoleFunc::WriteToConsole(consoleScreen, livesString, livesPos);
	}

	void DrawPlayer()
	{
		std::string playerStr = "";
		for (int i = 0; i < playerLength; ++i)
		{
			playerStr += sprite_player;
		}
		SetConsoleTextAttribute(consoleScreen, color_player);
		ConsoleFunc::WriteToConsole(consoleScreen, playerStr, playerPos);
	}

	void DrawBall()
	{
		SetConsoleTextAttribute(consoleScreen, color_ball);
		ConsoleFunc::WriteToConsole(consoleScreen, sprite_ball, ballPos);
	}

	void ResetPlayerAndBall()
	{
		SetConsoleTextAttribute(consoleScreen, color_bg);
		ConsoleFunc::WriteToConsole(consoleScreen, sprite_ball, ballPos);
		std::string playerStr = "";
		for (int i = 0; i < playerLength; ++i)
		{
			playerStr += sprite_player;
		}
		ConsoleFunc::WriteToConsole(consoleScreen, playerStr, playerPos);

		playerPos.Y = consoleScreenInfo.dwMaximumWindowSize.Y - 3;
		playerPos.X = (consoleScreenInfo.dwMaximumWindowSize.X / 2) - playerLength;
		ballPos.X = (consoleScreenInfo.dwMaximumWindowSize.X / 2) - (playerLength / 2);
		ballPos.Y = blocks_yStop + 1;
		ballVel.Y = 1;
		ballVel.X = 0;

		DrawBall();
		DrawPlayer();
		DrawLives();
	}

	void ResetGame()
	{
		std::string blockLine = "";
		for (int x = 0; x < consoleScreenInfo.dwMaximumWindowSize.X; ++x)
		{
			blockLine += sprite_block;
		}
		tempCoord.X = 0;
		SetConsoleTextAttribute(consoleScreen, color_blocks);
		for (int y = blocks_yStart; y < blocks_yStop; ++y)
		{
			tempCoord.Y = y;
			ConsoleFunc::WriteToConsole(consoleScreen, blockLine, tempCoord);
		}

		playerLives = 3;
		scoreVal = 0;
		scoreNew = 0;

		DrawScore();
		ResetPlayerAndBall();
	}

	// Returns whether the player has been moved
	bool MovePlayer(const int units_horizontal)
	{
		playerPos.X += units_horizontal;
		if (playerPos.X < 0)
		{
			playerPos.X = 0;
			return false;
		}
		else if (playerPos.X + playerLength > consoleScreenInfo.dwMaximumWindowSize.X)
		{
			playerPos.X = consoleScreenInfo.dwMaximumWindowSize.X - playerLength;
			return false;
		}
		return true;
	}

	// Checks if the coord is a block and if so removes block and adds score
	bool IsBallOnBlock(COORD coord)
	{
		CHAR_INFO sample;
		if (ConsoleFunc::GetCharFromConsole(consoleScreen, coord, sample))
		{
			if (sample.Char.AsciiChar == sprite_block[0])
			{
				ConsoleFunc::WriteToConsole(consoleScreen, sprite_ball, coord); // clear hit block
				++scoreNew;
				return true;
			}
		}
		return false;
	}

	// Ball movement & collisions 
	void UpdateBall()
	{
		SetConsoleTextAttribute(consoleScreen, color_bg);
		ConsoleFunc::WriteToConsole(consoleScreen, sprite_ball, ballPos); // clear old ball spot
		ballPos.X += ballVel.X;
		ballPos.Y += ballVel.Y;

		// Bounds check
		if (ballPos.X <= 0 && ballVel.X < 0) // left bound
		{
			ballPos.X = 0;
			ballVel.X = -ballVel.X;
		}
		else if (ballPos.X >= consoleScreenInfo.dwMaximumWindowSize.X && ballVel.X > 0) // right bound
		{
			ballPos.X = consoleScreenInfo.dwMaximumWindowSize.X;
			ballVel.X = -ballVel.X;
		}
		if (ballPos.Y <= 0 && ballVel.Y < 0) // top bound
		{
			ballPos.Y = 0;
			ballVel.Y = -ballVel.Y;
		}
		else if (ballPos.Y > playerPos.Y && ballVel.Y > 0) // out of bounds
		{
			--playerLives;
			ResetPlayerAndBall();
			return;
		}

		// Player & level collision check
		if (ballPos.Y == playerPos.Y && ballVel.Y > 0) // player collision
		{
			int playerHalfLength = playerLength / 2;
			int difference = (playerPos.X + playerHalfLength) - ballPos.X; // center = -1, 0, 1 left = + 2, + 3 right = -2, -3
			switch (difference)
			{
			case -1: // center
			case 0:
			case 1:
			{
				ballVel.Y = -1;
				ballVel.X = 0;
				ballPos.Y -= 1;
				break;
			}
			case -2: // right
			case -3:
			{
				ballVel.Y = -1;
				ballVel.X = 1;
				ballPos.Y -= 1;
				break;
			}
			case 2: // left
			case 3:
			{
				ballVel.Y = -1;
				ballVel.X = -1;
				ballPos.Y -= 1;
				break;
			}
			default:
			{
				break;
			}
			}
		}
		else
		{
			COORD above = ballPos;
			above.Y = (ballPos.Y == 0) ? 0 : ballPos.Y - 1;
			COORD below = ballPos;
			below.Y = ballPos.Y + 1; // this should never go outside of bounds due to bounds check

			if (IsBallOnBlock(above) || IsBallOnBlock(below))
				ballVel.Y = -ballVel.Y;
			if (ballVel.X != 0)
			{
				COORD left = ballPos;
				left.X = (ballPos.X == 0) ? 0 : ballPos.X - 1;
				COORD right = ballPos;
				right.X = (ballPos.X == consoleScreenInfo.dwMaximumWindowSize.X) ? consoleScreenInfo.dwMaximumWindowSize.X : ballPos.X + 1;

				if (IsBallOnBlock(left) || IsBallOnBlock(right))
					ballVel.X = -ballVel.X;
			}
		}

		SetConsoleTextAttribute(consoleScreen, color_ball);
		ConsoleFunc::WriteToConsole(consoleScreen, sprite_ball, ballPos);
	}

	void UpdateScore()
	{
		if (scoreNew != 0)
		{
			scoreVal += scoreNew;
			scoreNew = 0;
			DrawScore();
		}
	}

	void UpdatePlayer()
	{
		int moveDirection = 0;
		if (GetAsyncKeyState(VK_LEFT))
		{
			moveDirection = -1;
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			moveDirection = 1;
		}

		if (moveDirection)
		{
			if (MovePlayer(moveDirection))
			{
				COORD cleanUpPixel;
				cleanUpPixel.Y = playerPos.Y;
				cleanUpPixel.X = (moveDirection > 0) ? playerPos.X - 1 : playerPos.X + playerLength;
				SetConsoleTextAttribute(consoleScreen, color_bg);
				ConsoleFunc::WriteToConsole(consoleScreen, sprite_player, cleanUpPixel);
			}
		}
		DrawPlayer();
	}

	bool Exit()
	{
		ConsoleFunc::ShowConsoleCursor(consoleScreen);
		bool result = ConsoleFunc::DeleteInputHandle(consoleInput, previousInputConfig);
		result = result && SetStdHandle(STD_OUTPUT_HANDLE, oldScreenHandle);

		return result && CloseHandle(consoleScreen);
	}

	void Loop()
	{
		while (!GetAsyncKeyState(VK_ESCAPE))
		{
			unsigned int frameTime_start = clock();

			if (playerLives <= 0)
			{
				SetConsoleTextAttribute(consoleScreen, color_text);
				tempCoord.X = 0;
				tempCoord.Y = blocks_yStop - 2;
				ConsoleFunc::WriteToConsole(consoleScreen, gameover_msg, tempCoord);
				ConsoleFunc::WaitOnKey(consoleInput);
				break;
			}

			UpdatePlayer(); // handles input + player movement + drawing
			UpdateBall(); // handles ball movement + collisions + drawing
			UpdateScore(); // handles score
			// FPS lock
			unsigned int frameTime_stop = clock();
			unsigned int frameTime_time = frameTime_stop - frameTime_start;
			if (frameTime_time < frameTime_desired) // 32ms per frame = ~30fps
			{
				Sleep(frameTime_desired - frameTime_time);
			}
		}
	}

	// Called once to init some vars
	bool Setup()
	{
		oldScreenHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (ConsoleFunc::SetupConsole(consoleScreen, consoleScreenInfo)
			&& ConsoleFunc::EnableConsoleInput(consoleInput, previousInputConfig))
		{
			ConsoleFunc::HideConsoleCursor(consoleScreen);
			consoleScreenInfo.dwMaximumWindowSize.X = blocks_width;

			scorePos.Y = consoleScreenInfo.dwMaximumWindowSize.Y - 1;
			scorePos.X = 0;
			livesPos.X = 0;
			livesPos.Y = scorePos.Y - 1;

			ResetGame();
			return true;
		}

		return false;
	}
}
