#ifndef MTTSTR_HPP
#define MTTSTR_HPP

#include <cstddef>
#include <climits>

enum mttstr_fillmode
{
	LEFT,
	INTERNAL,
	RIGHT
};

enum mttstr_flags
{
	UCASE = 0,
	LCASE = 1,
	MCASE = 2,
	DO_NOT_NULL_TERM = 0,
	NULL_TERM = 4
};

class mttstr
{
	int base = 10;

public:
	char plus = 0, minus = '-', fill = ' ';
	std::size_t width = 0;
	int fillmode = LEFT, flags = NULL_TERM;

	static void *mem_rev(void *mem, std::size_t n);

	static constexpr bool IS_VAL_NEG(std::size_t val)
	{
		return val & (size_t)1 << (sizeof(val) * CHAR_BIT - 1);
	}

	mttstr();
	mttstr(int base, char plus, char minus, char fill, std::size_t width, int fillmode, int flags);

	void set_base(int base);
	int get_base();

	std::size_t ival_to_fstr(char *fstr, std::size_t ival);
	std::size_t fstr_to_ival(char *fstr, char **last);
};

#endif