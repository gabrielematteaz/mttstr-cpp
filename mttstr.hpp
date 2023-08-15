#ifndef MTTSTR_HPP
#define MTTSTR_HPP

#include <string>

void *mttstr_mem_rev(void *mem, std::size_t n) noexcept;
void *mttstr_wmem_rev(void *wmem, std::size_t n) noexcept;

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
#define WFMT_DEF_DEC { 10, L'+', L'-', L' ', FMT_FLAGS_LEFT_FILL | FMT_FLAGS_NULL_TERM, 0 }
#define WFMT_DEF_HEX { 16, 0, 0, L' ', FMT_FLAGS_MCASE | FMT_FLAGS_LEFT_FILL | FMT_FLAGS_NULL_TERM, 0 }

#define IS_VAL_NEG(val) (val & (std::size_t)1 << (sizeof(val) * CHAR_BIT - 1))

class mttstr_fmt_t
{
	uint8_t base;

public:
	char plus, minus, fill;
	uint8_t flags;
	uint8_t width;

	mttstr_fmt_t() noexcept;
	mttstr_fmt_t(uint8_t base, char plus, char minus, char fill, uint8_t flags, uint8_t width) noexcept;

	uint8_t get_base() const noexcept;
	void set_base(uint8_t base) noexcept;

	std::size_t ival_to_fstr(char *fstr, std::size_t ival) const noexcept;
	std::string ival_to_fstr(std::size_t ival) const;

	std::size_t fstr_to_ival(const char *fstr, const char **last) const noexcept;
};

class mttstr_wfmt_t
{
	uint8_t base;

public:
	wchar_t wplus, wminus, wfill;
	uint8_t flags;
	uint8_t width;

	mttstr_wfmt_t() noexcept;
	mttstr_wfmt_t(uint8_t base, wchar_t wplus, wchar_t wminus, wchar_t wfill, uint8_t flags, uint8_t width) noexcept;

	uint8_t get_base() const noexcept;
	void set_base(uint8_t base) noexcept;

	std::size_t ival_to_wfstr(wchar_t *wfstr, std::size_t ival) const noexcept;
	std::wstring ival_to_wfstr(std::size_t ival) const;

	std::size_t wfstr_to_ival(const wchar_t *wfstr, const wchar_t **wlast) const noexcept;
};

#endif