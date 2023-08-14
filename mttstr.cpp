#include "mttstr.hpp"

void *mttstr_mem_rev(void *mem, std::size_t n) noexcept
{
	if (mem != nullptr)
	{
		char *m = (char *)mem, *mn = m + n;

		while (m < mn)
		{
			mn--;

			char mc = *m;

			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}

mttstr_fmt_t::mttstr_fmt_t() noexcept
	: base(10), plus('+'), minus('-'), fill(' '), flags(FMT_FLAGS_LEFT_FILL | FMT_FLAGS_NULL_TERM), width(0)
{

}

mttstr_fmt_t::mttstr_fmt_t(uint8_t base, uint8_t plus, uint8_t minus, uint8_t fill, uint8_t flags, uint8_t width) noexcept
	: base(2 <= base && base <= 36 ? base : 10), plus(plus), minus(minus), fill(fill), flags(flags), width(width)
{

}

uint8_t mttstr_fmt_t::get_base() const noexcept
{
	return base;
}

void mttstr_fmt_t::set_base(uint8_t base) noexcept
{
	this->base = 2 <= base && base <= 36 ? base : 10;
}

std::size_t mttstr_fmt_t::ival_to_fstr(char *fstr, std::size_t ival) const noexcept
{
	std::size_t len;

	if (fstr == nullptr)
	{
		if (minus && IS_VAL_NEG(ival))
		{
			ival = -ival;
			len = 1;
		}
		else len = plus ? 1 : 0;

		do
		{
			ival /= base;
			len++;
		} while (ival);

		if (len < width) len = width;
	}
	else
	{
		std::size_t i;

		if (minus && IS_VAL_NEG(ival))
		{
			i = ival;
			ival = -ival;
		}
		else i = 0;

		char *f = fstr;

		if (base > 10)
		{
			char a = flags & FMT_FLAGS_LCASE ? 87 : 55;

			do
			{
				char rem = ival % base;

				ival /= base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % base;
				ival /= base;
				f++;
			} while (ival);
		}

		char *fw;

		if (flags & FMT_FLAGS_INT_FILL)
		{
			fw = fstr + width - 1;

			while (f < fw)
			{
				*f = fill;
				f++;
			}

			if IS_VAL_NEG(i)
			{
				*f = minus;
				f++;
			}
			else if (plus)
			{
				*f = plus;
				f++;
			}
			else if (f == fw)
			{
				*f = fill;
				f++;
			}

			len = f - fstr;
			mttstr_mem_rev(fstr, len);
		}
		else if (flags & FMT_FLAGS_RIGHT_FILL)
		{
			if IS_VAL_NEG(i)
			{
				*f = minus;
				f++;
			}
			else if (plus)
			{
				*f = plus;
				f++;
			}

			mttstr_mem_rev(fstr, f - fstr);
			fw = fstr + width;

			while (f < fw)
			{
				*f = fill;
				f++;
			}

			len = f - fstr;
		}
		else
		{
			if IS_VAL_NEG(i)
			{
				*f = minus;
				f++;
			}
			else if (plus)
			{
				*f = plus;
				f++;
			}

			fw = fstr + width;

			while (f < fw)
			{
				*f = fill;
				f++;
			}

			len = f - fstr;
			mttstr_mem_rev(fstr, len);
		}

		if (flags & FMT_FLAGS_NULL_TERM) *f = 0;
	}

	return len;
}

std::size_t mttstr_fmt_t::fstr_to_ival(const char *fstr, const char **last) const noexcept
{
	if (fstr != nullptr)
	{
		char fc = *fstr;
		std::size_t s;

		if (flags & FMT_FLAGS_INT_FILL)
		{
			if (fc == minus)
			{
				fstr++;
				fc = *fstr;
				s = -1;
			}
			else
			{
				if (fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				s = 1;
			}

			while (fc == fill)
			{
				fstr++;
				fc = *fstr;
			}
		}
		else
		{
			if ((flags & FMT_FLAGS_RIGHT_FILL) == 0)
			{
				while (fc == fill)
				{
					fstr++;
					fc = *fstr;
				}
			}

			if (fc == minus)
			{
				fstr++;
				fc = *fstr;
				s = -1;
			}
			else
			{
				if (fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				s = 1;
			}
		}

		std::size_t ival = 0;

		if (base > 10)
		{
			if (flags & FMT_FLAGS_MCASE)
			{
				char umax = 'A' + base - 10, lmax = umax + 32;

				while (1)
				{
					if ('0' <= fc && fc <= '9') ival = ival * base + fc - '0';
					else if ('A' <= fc && fc < umax) ival = ival * base + fc - 55;
					else if ('a' <= fc && fc < lmax) ival = ival * base + fc - 87;
					else break;

					fstr++;
					fc = *fstr;
				}
			}
			else
			{
				char min = flags & FMT_FLAGS_LCASE ? 'a' : 'A', max = min + base - 10;

				while (1)
				{
					if ('0' <= fc && fc <= '9') ival = ival * base + fc - '0';
					else if (min <= fc && fc < max) ival = ival * base + fc - min + 10;
					else break;

					fstr++;
					fc = *fstr;
				}
			}
		}
		else
		{
			char max = '0' + base;

			while ('0' <= fc && fc < max)
			{
				fstr++;
				ival = ival * base + fc - '0';
				fc = *fstr;
			}
		}

		if (last != nullptr) *last = fstr;

		return s * ival;
	}

	if (last != nullptr) *last = nullptr;

	return 0;
}