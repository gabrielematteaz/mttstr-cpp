#ifndef MTTSTR_HPP
#define MTTSTR_HPP

#include <cstddef>
#include <cstdint>
#include <climits>

void *mttstr_mem_rev(void *mem, std::size_t n);

#define FMT_FLAGS_UCASE 0
#define FMT_FLAGS_LCASE 1
#define FMT_FLAGS_MCASE 2
#define FMT_FLAGS_LEFT_FILL 0
#define FMT_FLAGS_INT_FILL 4
#define FMT_FLAGS_RIGHT_FILL 8
#define FMT_FLAGS_DO_NOT_NULL_TERM 0
#define FMT_FLAGS_NULL_TERM 16
#define FMT_DEF_DEC { 10, '+', '-', ' ', FMT_FLAGS_LEFT_FILL | FMT_FLAGS_NULL_TERM, 0 }
#define FMT_DEF_HEX { 16, 0, 0, ' ', FMT_FLAGS_MCASE | FMT_FLAGS_LEFT_FILL | FMT_FLAGS_NULL_TERM, 0 }

#define IS_VAL_NEG(val) (val & (std::size_t)1 << (sizeof(val) * CHAR_BIT - 1))

class mttstr_fmt_t
{
	uint8_t base;

public:
	uint8_t plus, minus, fill;
	uint8_t flags;
	uint8_t width;

	mttstr_fmt_t();
	mttstr_fmt_t(uint8_t base, uint8_t plus, uint8_t minus, uint8_t fill, uint8_t flags, uint8_t width);

	uint8_t get_base() const;
	void set_base(uint8_t base);

	std::size_t ival_to_fstr(char *fstr, std::size_t ival) const;

	std::size_t fstr_to_ival(const char *fstr, const char **last) const;
};

#endif