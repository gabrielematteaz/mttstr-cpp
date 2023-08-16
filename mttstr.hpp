#ifndef MTTSTR_HPP
#define MTTSTR_HPP

#include <string>

#define FMT_FS_UCASE 0x00
#define FMT_FS_LCASE 0x01
#define FMT_FS_MCASE 0x02
#define FMT_FS_LEFT_FILL 0x00
#define FMT_FS_INT_FILL 0x04
#define FMT_FS_RIGHT_FILL 0x08
#define FMT_FS_DO_NOT_NULL_TERM 0x00
#define FMT_FS_NULL_TERM 0x10

#define FMT_DEF_HEX { 0, 0, ' ', 16, FMT_FS_MCASE | FMT_FS_LEFT_FILL | FMT_FS_NULL_TERM, 0 }

#define WFMT_DEF_HEX { 0, 0, L' ', 16, FMT_FS_MCASE | FMT_FS_LEFT_FILL | FMT_FS_NULL_TERM, 0 }

#define IS_VAL_NEG(val) (val & (std::size_t)1 << (sizeof(val) * CHAR_BIT - 1))

void *mttstr_mem_rev(void *mem, std::size_t n) noexcept;
void *mttstr_wmem_rev(void *wmem, std::size_t n) noexcept;

class mttstr_fmt_t
{
public:
	char plus, minus, fill;

private:
	int base;

public:
	int fs;
	std::size_t width;

	inline mttstr_fmt_t() noexcept
		: plus('+'), minus('-'), fill(' '), base(10), fs(FMT_FS_LEFT_FILL | FMT_FS_NULL_TERM), width(0)
	{

	}

	inline mttstr_fmt_t(char plus, char minus, char fill, int base, int fs, std::size_t width) noexcept
		: plus(plus), minus(minus), fill(fill), base(base < 2 || base > 36 ? 10 : base), fs(fs), width(width)
	{

	}

	inline int get_base() const noexcept
	{
		return base;
	}

	inline void set_base(int base) noexcept
	{
		this->base = base < 2 || base > 36 ? 10 : base;
	}

	std::size_t ival_to_fstr(char *fstr, std::size_t ival) const noexcept;
	std::string ival_to_fstr(std::size_t ival) const;

	std::size_t fstr_to_ival(const char *fstr, const char **last) const noexcept;
};

class mttstr_wfmt_t
{
public:
	wchar_t plus, minus, fill;

private:
	int base;

public:
	int fs;
	std::size_t width;

	inline mttstr_wfmt_t() noexcept
		: plus(L'+'), minus(L'-'), fill(L' '), base(10), fs(FMT_FS_LEFT_FILL | FMT_FS_NULL_TERM), width(0)
	{

	}

	inline mttstr_wfmt_t(wchar_t plus, wchar_t minus, wchar_t fill, int base, int fs, std::size_t width) noexcept
		: plus(plus), minus(minus), fill(fill), base(base < 2 || base > 36 ? 10 : base), fs(fs), width(width)
	{

	}

	inline int get_base() const noexcept
	{
		return base;
	}

	inline void set_base(int base) noexcept
	{
		this->base = base < 2 || base > 36 ? 10 : base;
	}

	std::size_t ival_to_wfstr(wchar_t *wfstr, std::size_t ival) const noexcept;
	std::wstring ival_to_wfstr(std::size_t ival) const;

	std::size_t wfstr_to_ival(const wchar_t *wfstr, const wchar_t **wlast) const noexcept;
};

#endif