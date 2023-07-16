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

size_t mttstr_ival_to_fstr(char *fstr, size_t ival, char base, char plus, char minus, char fill, enum mttstr_fillmode_t fillmode, size_t width, enum mttstr_flags_t flags)
{
	size_t i, len;
	char *f, a, rem, *fw;

	if (2 <= base && base <= 36)
	{
		if (fstr != NULL)
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

				break;
			case RIGHT:
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

				break;
			default:
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

			if (flags & NULL_TERM) len++;
		}

		return len;
	}

	return 0;
}

size_t mttstr_fstr_to_ival(char *fstr, char **last, char base, char plus, char minus, char fill, enum mttstr_fillmode_t fillmode, enum mttstr_flags_t flags)
{
	char fc, maxucase, maxlcase, min, max;
	size_t ival, sign;

	if (fstr != NULL && 2 <= base && base <= 36)
	{
		fc = *fstr;
		ival = 0;

		switch (fillmode)
		{
		case UNKNOWN:
			if (fc == fill)
			{
				fstr++;
				fc = *fstr;

				goto left;
			}

		case INTERNAL:
			if (fc == minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}

			while (fc == fill)
			{
				fstr++;
				fc = *fstr;
			}

			break;
		default:
		left:
			while (fc == fill)
			{
				fstr++;
				fc = *fstr;
			}

		case NONE: case RIGHT:
			if (fc == minus)
			{
				fstr++;
				fc = *fstr;
				sign = -1;
			}
			else
			{
				if (fc == plus)
				{
					fstr++;
					fc = *fstr;
				}

				sign = 1;
			}

			break;
		}

		if (base > 10)
		{
			if (flags & MCASE)
			{
				maxucase = 'A' + base - 10;
				maxlcase = maxucase + 32;

				while (1)
				{
					if ('0' <= fc && fc <= '9') ival = ival * base + fc - '0';
					else if ('A' <= fc && fc < maxucase) ival = ival * base + fc - 55;
					else if ('a' <= fc && fc < maxlcase) ival = ival * base + fc - 87;
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

		if (last != NULL) *last = fstr;

		return sign * ival;
	}

	if (last != NULL) *last = NULL;

	return 0;
}