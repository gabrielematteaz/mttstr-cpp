#include "mttstr.hpp"

void *mttstr::mem_rev(void *mem, std::size_t n)
{
	char *m, *mn, mc;

	if (mem != nullptr)
	{
		m = (char *)mem, mn = m + n;

		while (m < mn)
		{
			mn--;
			mc = *m;
			*m = *mn;
			m++;
			*mn = mc;
		}
	}

	return mem;
}

mttstr::mttstr()
{

}

void mttstr::set_base(int base)
{
	this->base = 2 <= base && base <= 36 ? base : 10;
}

int mttstr::get_base()
{
	return base;
}

mttstr::mttstr(int base, char plus, char minus, char fill, std::size_t width, int fillmode, int flags)
	: base(2 <= base && base <= 36 ? base : 10), plus(plus), minus(minus), fill(fill), width(width), fillmode(fillmode), flags(flags)
{

}

std::size_t mttstr::ival_to_fstr(char *fstr, std::size_t ival)
{
	std::size_t i, len;
	char *f, a, rem, *fw;

	if (fstr != nullptr)
	{
		if (minus && IS_VAL_NEG(ival))
		{
			i = ival;
			ival = -ival;
		}
		else i = 0;

		f = fstr;

		if (base > 10)
		{
			a = flags & LCASE ? 87 : 55;

			do
			{
				rem = ival % base;
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

		switch (fillmode)
		{
		case INTERNAL:
			fw = fstr + width - 1;

			while (f < fw)
			{
				*f = fill;
				f++;
			}

			if (IS_VAL_NEG(i))
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
			mem_rev(fstr, len);

			break;
		case RIGHT:
			if (IS_VAL_NEG(i))
			{
				*f = minus;
				f++;
			}
			else if (plus)
			{
				*f = plus;
				f++;
			}

			mem_rev(fstr, f - fstr);

			fw = fstr + width;

			while (f < fw)
			{
				*f = fill;
				f++;
			}

			len = f - fstr;
			
			break;
		default:
			if (IS_VAL_NEG(i))
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
			mem_rev(fstr, len);

			break;
		}

		if (flags & NULL_TERM) *f = 0;
	}
	else
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

	return len;
}

std::size_t mttstr::fstr_to_ival(char *fstr, char **last)
{
	char fc, umax, lmax, min, max;
	size_t s, ival;

	if (fstr != nullptr)
	{
		fc = *fstr;

		switch (fillmode)
		{
		case INTERNAL:
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

			break;
		default:
			while (fc == fill)
			{
				fstr++;
				fc = *fstr;
			}

		case RIGHT:
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

			break;
		}

		ival = 0;

		if (base > 10)
		{
			if (flags & MCASE)
			{
				umax = 'A' + base - 10;
				lmax = umax + 32;

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
				min = flags & LCASE ? 'a' : 'A';
				max = min + base - 10;

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
			max = '0' + base;

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