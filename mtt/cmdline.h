#ifndef MTT_CMDLINE_H
#define MTT_CMDLINE_H

#include <stddef.h>

enum mtt_cmdline_opt_fs_t
{
	NO_ARG,
	OPTIONAL_ARG,
	REQUIRED_ARG
};

struct mtt_cmdline_opt_t
{
	int alias;
	enum mtt_cmdline_opt_fs_t fs;
	const char *arg;
};

extern char *cur_opt, **cur_arg, **not_opt;

struct mtt_cmdline_opt_t *mtt_cmdline_extr_opt(int argc, char *argv[], int optc, struct mtt_cmdline_opt_t *optv);

#endif