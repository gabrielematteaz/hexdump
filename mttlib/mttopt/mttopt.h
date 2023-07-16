#ifndef MTTOPT_H
#define MTTOPT_H

#include <stddef.h>

enum mttopt_opt_copymode_t
{
	OVERWRITE_ON_COPY,
	IGNORE_COPIES,
	EXIT_ON_COPY,
};

enum mttopt_opt_argmode_t
{
	HAS_NO_ARG,
	CAN_HAVE_ARG,
	MUST_HAVE_ARG,
};

enum mttopt_opt_status_t
{
	NOT_FOUND,
	FOUND,
};

struct mttopt_opt_t
{
	int val;
	enum mttopt_opt_copymode_t copymode;
	enum mttopt_opt_argmode_t argmode;
	enum mttopt_opt_status_t status;
	char *arg;
};

int mttopt_extract_optv(int argc, char *argv[], int optc, struct mttopt_opt_t *optv);

#endif