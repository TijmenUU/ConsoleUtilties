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
		Type GetEventType() const
		{
			return static_cast<Type>(EventType);
		}

		InputEvent()
		{
			EventType = 0;
		}
	};

	// Returns the number of events actually read
	std::size_t GetAll(InputEvent* events, const std::size_t size)
	{
		DWORD availableEvents = 0;
		if (size == 0 ||
			!GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &availableEvents) ||
			availableEvents == 0)
		{
			return 0;
		}

		DWORD itemsRead = 0;
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE),
			&events[0],
			size,
			&itemsRead);

		return itemsRead;
	}

	void GetAll(std::vector<InputEvent> & events)
	{
		events.resize(GetAll(&events[0], events.size()));
	}

	std::vector<InputEvent> GetAll(const std::size_t count = 64)
	{
		std::vector<InputEvent> events;
		events.resize(count);
		GetAll(events);

		return events;
	}

	InputEvent Get()
	{
		InputEvent ev;
		
		GetAll(&ev, 1);
		
		return ev;
	}
}

std::istream& operator >>(std::istream& inputStream,
	Event::InputEvent& ev)
{
	ev = Event::Get();

	return inputStream;
}

// Expects events to have a size >= 0
std::istream& operator >>(std::istream& inputStream,
	std::vector<Event::InputEvent>& events)
{
	Event::GetAll(events);

	return inputStream;
}