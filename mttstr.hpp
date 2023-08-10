#ifndef MTTSTR_HPP
#define MTTSTR_HPP

#include <cstddef>
#include <climits>

enum mttstr_LetterCase
{
	UPPERCASE,
	LOWERCASE,
	MIXEDCASE
};

enum mttstr_FillMode
{
	LEFT,
	INTERNAL,
	RIGHT
};

class mttstr_Converter
{
	int base = 10;

public:
	char plus = '+', minus = '-', fill = ' ';
	mttstr_LetterCase letterCase = MIXEDCASE;
	mttstr_FillMode fillMode = LEFT;
	std::size_t width = 0;
	bool nullTerminate = true;

	mttstr_Converter();
	mttstr_Converter(int base, char plus, char minus, char fill, mttstr_LetterCase letterCase, mttstr_FillMode fillMode, bool nullTerminate, std::size_t width);

	int GetBase();
	void SetBase(int base);

	std::size_t ValueToString(char *string, std::size_t value);
	std::size_t StringToValue(char *string, char **lastProcessedChar);

	static void *ReverseMemory(void *memory, std::size_t size);

	static constexpr bool IsNegativeValue(std::size_t value)
	{
		return value & (std::size_t)1 << (sizeof(value) * CHAR_BIT - 1);
	}
};

#endif