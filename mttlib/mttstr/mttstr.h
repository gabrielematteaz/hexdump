#ifndef MTTSTR_H
#define MTTSTR_H

#include <stddef.h>
#include <limits.h>

void *mttstr_mem_rev(void *mem, size_t n);

enum mttstr_fmt_fillmode_t
{
	LEFT,
	INTERNAL,
	RIGHT
};

enum mttstr_fmt_flags_t
{
	UCASE = 0,
	LCASE = 1,
	MCASE = 2,
	DO_NOT_NULL_TERM = 0,
	NULL_TERM = 4
};

struct mttstr_fmt_t
{
	char plus, minus, fill;
	int base;
	enum mttstr_fmt_fillmode_t fillmode;
	enum mttstr_fmt_flags_t flags;
	size_t width;
};

#define IS_VAL_NEG(val) (val & (size_t)1 << (sizeof(val) * CHAR_BIT - 1))

size_t mttstr_ival_to_fstr(char *fstr, size_t ival, struct mttstr_fmt_t fmt);

size_t mttstr_fstr_to_ival(char *fstr, char **last, struct mttstr_fmt_t fmt);

#endif