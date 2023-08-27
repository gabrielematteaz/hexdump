#ifndef MTT_STR_H
#define MTT_STR_H

#include <stddef.h>
#include <limits.h>

#define IS_VAL_NEG(val) (val & (size_t)1 << (sizeof(val) * CHAR_BIT - 1))

enum mtt_str_fmt_ltrcase_t
{
	MIXED,
	UPPER,
	LOWER
};

enum mtt_str_fmt_fillmode_t
{
	LEFT,
	INTERNAL,
	RIGHT
};

struct mtt_str_fmt_t
{
	char plus, minus, fill;
	int base;
	enum mtt_str_fmt_ltrcase_t ltrcase;
	enum mtt_str_fmt_fillmode_t fillmode;
	int nonullterm;
	size_t width;
};

void *mtt_str_mem_rev(void *mem, size_t n);

size_t mtt_str_ival_to_fstr(char *fstr, size_t ival, struct mtt_str_fmt_t fmt);

size_t mtt_str_fstr_to_ival(const char *fstr, const char **last, struct mtt_str_fmt_t fmt);

#endif