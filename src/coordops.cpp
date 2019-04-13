#include "coordops.hpp"

COORD operator -(const COORD a, const COORD b)
{
	COORD result;
	result.X = a.X - b.X;
	result.Y = a.Y - b.Y;

	return result;
}

COORD operator +(const COORD a, const COORD b)
{
	COORD result;
	result.X = a.X + b.X;
	result.Y = a.Y + b.Y;

	return result;
}

COORD operator *(const COORD a, const COORD b)
{
	COORD result;
	result.X = a.X * b.X;
	result.Y = a.Y * b.Y;

	return result;
}

COORD operator /(const COORD a, const COORD b)
{
	COORD result;
	result.X = a.X / b.X;
	result.Y = a.Y / b.Y;

	return result;
}