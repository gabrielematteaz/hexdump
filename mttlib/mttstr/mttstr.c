#include "mttstr.h"

void *mttstr_mem_rev(void *mem, size_t n)
{
	char *m, *mn, mc;

	if (mem != NULL)
	{
		m = mem, mn = m + n;

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

size_t mttstr_ival_to_fstr(char *fstr, size_t ival, struct mttstr_fmt_t fmt)
{
	size_t i, len;
	char *f, a, rem, *fw;

	if (2 <= fmt.base && fmt.base <= 36)
	{
		if (fstr != NULL)
		{
			if (fmt.minus && IS_VAL_NEG(ival))
			{
				i = ival;
				ival = -ival;
			}
			else i = 0;

			f = fstr;

			if (fmt.base > 10)
			{
				a = fmt.flags & LCASE ? 87 : 55;

				do
				{
					rem = ival % fmt.base;
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

			switch (fmt.fillmode)
			{
			case INTERNAL:
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
				mttstr_mem_rev(fstr, len);

				break;
			case RIGHT:
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

				mttstr_mem_rev(fstr, f - fstr);

				fw = fstr + fmt.width;

				while (f < fw)
				{
					*f = fmt.fill;
					f++;
				}

				len = f - fstr;
				
				break;
			default:
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
				mttstr_mem_rev(fstr, len);

				break;
			}

			if (fmt.flags & NULL_TERM) *f = 0;
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

	return 0;
}

size_t mttstr_fstr_to_ival(char *fstr, char **last, struct mttstr_fmt_t fmt)
{
	char fc, umax, lmax, min, max;
	size_t s, ival;

	if (fstr != NULL && 2 <= fmt.base && fmt.base <= 36)
	{
		fc = *fstr;

		switch (fmt.fillmode)
		{
		case INTERNAL:
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

			break;
		default:
			while (fc == fmt.fill)
			{
				fstr++;
				fc = *fstr;
			}

		case RIGHT:
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

			break;
		}

		ival = 0;

		if (fmt.base > 10)
		{
			if (fmt.flags & MCASE)
			{
				umax = 'A' + fmt.base - 10;
				lmax = umax + 32;

				while (1)
				{
					if ('0' <= fc && fc <= '9') ival = ival * fmt.base + fc - '0';
					else if ('A' <= fc && fc < umax) ival = ival * fmt.base + fc - 55;
					else if ('a' <= fc && fc < lmax) ival = ival * fmt.base + fc - 87;
					else break;

					fstr++;
					fc = *fstr;
				}
			}
			else
			{
				min = fmt.flags & LCASE ? 'a' : 'A';
				max = min + fmt.base - 10;

				while (1)
				{
					if ('0' <= fc && fc <= '9') ival = ival * fmt.base + fc - '0';
					else if (min <= fc && fc < max) ival = ival * fmt.base + fc - min + 10;
					else break;

					fstr++;
					fc = *fstr;
				}
			}
		}
		else
		{
			max = '0' + fmt.base;

			while ('0' <= fc && fc < max)
			{
				fstr++;
				ival = ival * fmt.base + fc - '0';
				fc = *fstr;
			}
		}

		if (last != NULL) *last = fstr;

		return s * ival;
	}

	if (last != NULL) *last = NULL;

	return 0;
}