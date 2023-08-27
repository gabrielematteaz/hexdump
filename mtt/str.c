#include "str.h"

void *mtt_str_mem_rev(void *mem, size_t n)
{
	if (mem)
	{
		char *m = mem, *mn = m + n;

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

size_t mtt_str_ival_to_fstr(char *fstr, size_t ival, struct mtt_str_fmt_t fmt)
{
	if (fmt.base < 2 || fmt.base > 36) return 0;

	size_t len;

	if (fstr)
	{
		size_t i;

		if (fmt.minus && IS_VAL_NEG(ival))
		{
			i = ival;
			ival = -ival;
		}
		else i = 0;

		char *f = fstr;

		if (fmt.base > 10)
		{
			char a = fmt.ltrcase & LOWER ? 87 : 55;

			do
			{
				char rem = ival % fmt.base;

				ival /= fmt.base;
				*f = (rem < 10 ? '0' : a) + rem;
				f++;
			} while (ival);
		}
		else
		{
			do
			{
				*f = '0' + ival % fmt.base;
				ival /= fmt.base;
				f++;
			} while (ival);
		}

		char *fw;

		if (fmt.fillmode == INTERNAL)
		{
			fw = fstr + fmt.width - 1;

			while (f < fw)
			{
				*f = fmt.fill;
				f++;
			}

			if IS_VAL_NEG(i)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}
			else if (f == fw)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
			mtt_str_mem_rev(fstr, len);
		}
		else if (fmt.fillmode == RIGHT)
		{
			if IS_VAL_NEG(i)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}

			mtt_str_mem_rev(fstr, f - fstr);
			fw = fstr + fmt.width;

			while (f < fw)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
		}
		else
		{
			if IS_VAL_NEG(i)
			{
				*f = fmt.minus;
				f++;
			}
			else if (fmt.plus)
			{
				*f = fmt.plus;
				f++;
			}

			fw = fstr + fmt.width;

			while (f < fw)
			{
				*f = fmt.fill;
				f++;
			}

			len = f - fstr;
			mtt_str_mem_rev(fstr, len);
		}

		if (fmt.nonullterm == 0) *f = 0;
	}
	else
	{
		if (fmt.minus && IS_VAL_NEG(ival))
		{
			ival = -ival;
			len = 1;
		}
		else len = fmt.plus ? 1 : 0;

		do
		{
			ival /= fmt.base;
			len++;
		} while (ival);

		if (len < fmt.width) len = fmt.width;
	}

	return len;
}

size_t mtt_str_fstr_to_ival(const char *fstr, const char **last, struct mtt_str_fmt_t fmt)
{
	if (fstr == NULL || fmt.base < 2 || fmt.base > 36)
	{
		if (last) *last = NULL;

		return 0;
	}

	char fc = *fstr;
	size_t s;

	if (fmt.fillmode == INTERNAL)
	{
		if (fc == fmt.minus)
		{
			fstr++;
			fc = *fstr;
			s = -1;
		}
		else
		{
			if (fc == fmt.plus)
			{
				fstr++;
				fc = *fstr;
			}

			s = 1;
		}

		while (fc == fmt.fill)
		{
			fstr++;
			fc = *fstr;
		}
	}
	else
	{
		if (fmt.fillmode == LEFT)
		{
			while (fc == fmt.fill)
			{
				fstr++;
				fc = *fstr;
			}
		}

		if (fc == fmt.minus)
		{
			fstr++;
			fc = *fstr;
			s = -1;
		}
		else
		{
			if (fc == fmt.plus)
			{
				fstr++;
				fc = *fstr;
			}

			s = 1;
		}
	}

	size_t ival = 0;

	if (fmt.base > 10)
	{
		char diff;

		if (fmt.ltrcase)
		{
			char min = fmt.ltrcase & LOWER ? 'a' : 'A', max = min + fmt.base - 10, diffmin = min + 10;

			while (1)
			{
				if (fc < '0' || fc > '9')
				{
					if (fc < min || max <= fc) break;

					diff = diffmin;
				}
				else diff = '0';

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
		else
		{
			char umax = 55 + fmt.base, lmax = umax + 32;

			while (1)
			{
				if (fc < '0' || fc > '9')
				{
					if (fc < 'A' || umax <= fc)
					{
						if (fc < 'a' || lmax <= fc) break;

						diff = 87;
					}
					else diff = 55;
				}
				else diff = '0';

				fstr++;
				ival = ival * fmt.base + fc - diff;
				fc = *fstr;
			}
		}
	}
	else
	{
		char max = '0' + fmt.base;

		while (1)
		{
			if (fc < '0' || max <= fc) break;

			fstr++;
			ival = ival * fmt.base + fc - '0';
			fc = *fstr;
		}
	}

	if (last) *last = fstr;

	return s * ival;
}