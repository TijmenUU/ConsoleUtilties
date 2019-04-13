#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <Windows.h>

namespace Event
{
	enum class Type : std::uint16_t
	{
		UNKNOWN = 0x0000,
		FOCUS = 0x0010,
		KEY = 0x0001,
		MENU = 0x0008,
		MOUSE = 0x0002,
		RESIZE = 0x0004
	};

	/*
		InputEvent.Event can contain either of the following (a union):
		- KEY_EVENT_RECORD KeyEvent;
		- MOUSE_EVENT_RECORD MouseEvent;
		- WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
		- MENU_EVENT_RECORD MenuEvent;
		- FOCUS_EVENT_RECORD FocusEvent;

		Be sure to evaluate the event Type before you try accessing the
		Event member variable.
	*/
	struct InputEvent : public INPUT_RECORD
	{
		Type GetEventType() const;

		InputEvent();
	};

	// Returns the number of events actually read
	unsigned long GetAll(InputEvent* events, const std::size_t size);

	// Resizes the vector to the number of read events
	void GetAll(std::vector<InputEvent>& events);

	// Resizes the vector with a maximum of count events
	std::vector<InputEvent> GetAll(const std::size_t count = 64);

	InputEvent Get();
}

std::istream& operator >>(std::istream& inputStream, Event::InputEvent& ev);

// Expects events to have a size >= 0
std::istream& operator >>(std::istream& inputStream, std::vector<Event::InputEvent>& events);