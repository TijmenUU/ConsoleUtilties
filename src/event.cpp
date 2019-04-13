#include "event.hpp"

namespace Event
{
	Type InputEvent::GetEventType() const
	{
		return static_cast<Type>(EventType);
	}

	InputEvent::InputEvent()
	{
		EventType = 0;
	}

	unsigned long GetAll(InputEvent* events, const std::size_t size)
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
			static_cast<DWORD>(size),
			&itemsRead);

		return itemsRead;
	}

	void GetAll(std::vector<InputEvent>& events)
	{
		events.resize(GetAll(&events[0], events.size()));
	}

	std::vector<InputEvent> GetAll(const std::size_t count)
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