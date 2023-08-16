#include "mttstr.hpp"

void *mttstr_mem_rev(void *mem, std::size_t n) noexcept
{
	if (mem == nullptr) return nullptr;

	char *m = (char *)mem, *mn = m + n;

	while (m < mn)
	{
		mn--;

		char mc = *m;

		*m = *mn;
		m++;
		*mn = mc;
	}

	return mem;
}

void *mttstr_wmem_rev(void *wmem, std::size_t n) noexcept
{
	if (wmem == nullptr) return nullptr;

	wchar_t *wm = (wchar_t *)wmem, *wmn = wm + n;

	while (wm < wmn)
	{
		wmn--;

		wchar_t wmc = *wm;

		*wm = *wmn;
		wm++;
		*wmn = wmc;
	}

	return wmem;
}

std::size_t mttstr_fmt_t::ival_to_fstr(char *fstr, std::size_t ival) const noexcept
{
	if (base < 2 || base > 36) return 0;

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
			char a = fs & FMT_FS_LCASE ? 87 : 55;

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

		if (fs & FMT_FS_INT_FILL)
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
		else if (fs & FMT_FS_RIGHT_FILL)
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

		if (fs & FMT_FS_NULL_TERM) *f = 0;
	}

	return len;
}

std::string mttstr_fmt_t::ival_to_fstr(std::size_t ival) const
{
	std::size_t fstrsize = ival_to_fstr(nullptr, ival);

	if (fs & FMT_FS_NULL_TERM) fstrsize++;

	std::string fstr(fstrsize * sizeof(char), 0);

	ival_to_fstr(fstr.data(), ival);

	return fstr;
}

std::size_t mttstr_fmt_t::fstr_to_ival(const char *fstr, const char **last) const noexcept
{
	if (fstr == nullptr || base < 2 || base > 36)
	{
		if (last != nullptr) *last = nullptr;

		return 0;
	}

	char fc = *fstr;
	std::size_t s;

	if (fs & FMT_FS_INT_FILL)
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
		if ((fs & FMT_FS_RIGHT_FILL) == 0x00)
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
		if (fs & FMT_FS_MCASE)
		{
			char umax = 'A' + base - 10, lmax = umax + 32;

			while (1)
			{
				if (fc < '0' || fc > '9')
				{
					if (fc < 'A' || umax <= fc)
					{
						if (fc < 'a' || lmax <= fc) break;

						ival = ival * base + fc - 87;
					}
					else ival = ival * base + fc - 55;
				}
				else ival = ival * base + fc - '0';

				fstr++;
				fc = *fstr;
			}
		}
		else
		{
			char min = fs & FMT_FS_LCASE ? 'a' : 'A', max = min + base - 10;

			while (1)
			{
				if (fc < '0' || fc > '9')
				{
					if (fc < min || max <= fc) break;

					ival = ival * base + fc - min + 10;
				}
				else ival = ival * base + fc - '0';

				fstr++;
				fc = *fstr;
			}
		}
	}
	else
	{
		char max = '0' + base;

		while (1)
		{
			if (fc < '0' || max <= fc) break;

			fstr++;
			ival = ival * base + fc - '0';
			fc = *fstr;
		}
	}

	if (last != nullptr) *last = fstr;

	return s * ival;
}

std::size_t mttstr_wfmt_t::ival_to_wfstr(wchar_t *wfstr, std::size_t ival) const noexcept
{
if (base < 2 || base > 36) return 0;

	std::size_t len;

	if (wfstr == nullptr)
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

		wchar_t *wf = wfstr;

		if (base > 10)
		{
			wchar_t wa = fs & FMT_FS_LCASE ? 87 : 55;

			do
			{
				wchar_t wrem = ival % base;

				ival /= base;
				*wf = (wrem < 10 ? '0' : wa) + wrem;
				wf++;
			} while (ival);
		}
		else
		{
			do
			{
				*wf = '0' + ival % base;
				ival /= base;
				wf++;
			} while (ival);
		}

		wchar_t *wfw;

		if (fs & FMT_FS_INT_FILL)
		{
			wfw = wfstr + width - 1;

			while (wf < wfw)
			{
				*wf = fill;
				wf++;
			}

			if IS_VAL_NEG(i)
			{
				*wf = minus;
				wf++;
			}
			else if (plus)
			{
				*wf = plus;
				wf++;
			}
			else if (wf == wfw)
			{
				*wf = fill;
				wf++;
			}

			len = wf - wfstr;
			mttstr_wmem_rev(wfstr, len);
		}
		else if (fs & FMT_FS_RIGHT_FILL)
		{
			if IS_VAL_NEG(i)
			{
				*wf = minus;
				wf++;
			}
			else if (plus)
			{
				*wf = plus;
				wf++;
			}

			mttstr_wmem_rev(wfstr, wf - wfstr);
			wfw = wfstr + width;

			while (wf < wfw)
			{
				*wf = fill;
				wf++;
			}

			len = wf - wfstr;
		}
		else
		{
			if IS_VAL_NEG(i)
			{
				*wf = minus;
				wf++;
			}
			else if (plus)
			{
				*wf = plus;
				wf++;
			}

			wfw = wfstr + width;

			while (wf < wfw)
			{
				*wf = fill;
				wf++;
			}

			len = wf - wfstr;
			mttstr_wmem_rev(wfstr, len);
		}

		if (fs & FMT_FS_NULL_TERM) *wf = 0;
	}

	return len;
}

std::wstring mttstr_wfmt_t::ival_to_wfstr(std::size_t ival) const
{
	std::size_t wfstrsize = ival_to_wfstr(nullptr, ival);

	if (fs & FMT_FS_NULL_TERM) wfstrsize++;

	std::wstring wfstr(wfstrsize * sizeof(wchar_t), 0);

	ival_to_wfstr(wfstr.data(), ival);

	return wfstr;
}

std::size_t mttstr_wfmt_t::wfstr_to_ival(const wchar_t *wfstr, const wchar_t **wlast) const noexcept
{
	if (wfstr == nullptr || base < 2 || base > 36)
	{
		if (wlast != nullptr) *wlast = nullptr;

		return 0;
	}

	wchar_t wfc = *wfstr;
	std::size_t s;

	if (fs & FMT_FS_INT_FILL)
	{
		if (wfc == minus)
		{
			wfstr++;
			wfc = *wfstr;
			s = -1;
		}
		else
		{
			if (wfc == plus)
			{
				wfstr++;
				wfc = *wfstr;
			}

			s = 1;
		}

		while (wfc == fill)
		{
			wfstr++;
			wfc = *wfstr;
		}
	}
	else
	{
		if ((fs & FMT_FS_RIGHT_FILL) == 0x00)
		{
			while (wfc == fill)
			{
				wfstr++;
				wfc = *wfstr;
			}
		}

		if (wfc == minus)
		{
			wfstr++;
			wfc = *wfstr;
			s = -1;
		}
		else
		{
			if (wfc == plus)
			{
				wfstr++;
				wfc = *wfstr;
			}

			s = 1;
		}
	}

	std::size_t ival = 0;

	if (base > 10)
	{
		if (fs & FMT_FS_MCASE)
		{
			wchar_t wumax = L'A' + base - 10, wlmax = wumax + 32;

			while (1)
			{
				if (wfc < L'0' || wfc > L'9')
				{
					if (wfc < L'A' || wumax <= wfc)
					{
						if (wfc < L'a' || wlmax <= wfc) break;

						ival = ival * base + wfc - 87;
					}
					else ival = ival * base + wfc - 55;
				}
				else ival = ival * base + wfc - L'0';

				wfstr++;
				wfc = *wfstr;
			}
		}
		else
		{
			wchar_t wmin = fs & FMT_FS_LCASE ? L'a' : L'A', wmax = wmin + base - 10;

			while (1)
			{
				if (wfc < L'0' || wfc > L'9')
				{
					if (wfc < wmin || wmax <= wfc) break;

					ival = ival * base + wfc - wmin + 10;
				}
				else ival = ival * base + wfc - L'0';

				wfstr++;
				wfc = *wfstr;
			}
		}
	}
	else
	{
		wchar_t wmax = L'0' + base;

		while (1)
		{
			if (wfc < L'0' || wmax <= wfc) break;

			wfstr++;
			ival = ival * base + wfc - L'0';
			wfc = *wfstr;
		}
	}

	if (wlast != nullptr) *wlast = wfstr;

	return s * ival;
}