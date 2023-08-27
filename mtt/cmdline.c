#include "cmdline.h"

char *cur_opt = NULL, **cur_arg = NULL, **not_opt;

struct mtt_cmdline_opt_t *mtt_cmdline_extr_opt(int argc, char *argv[], int optc, struct mtt_cmdline_opt_t *optv)
{
	if (cur_opt)
	{
	parse_opt:
		struct mtt_cmdline_opt_t *ov = optv, *ovc = ov + optc;

		while (ov < ovc)
		{
			if (*cur_opt == ov->alias)
			{
				enum mtt_cmdline_opt_fs_t fs = ov->fs;

				cur_opt++;

				if (fs == OPTIONAL_ARG)
				{
					cur_arg++;
					ov->arg = *cur_opt ? cur_opt : NULL;
					cur_opt = NULL;
				}
				else if (fs == REQUIRED_ARG)
				{
					if (*cur_opt) ov->arg = cur_opt;
					else
					{
						cur_arg++;

						if (cur_arg == argv + argc)
						{
							cur_opt = NULL;
							not_opt = cur_arg;
							cur_arg = NULL;

							return NULL;
						}

						ov->arg = *cur_arg;
					}

					cur_opt = NULL;
					cur_arg++;
				}
				else if (*cur_opt == 0)
				{
					cur_opt = NULL;
					cur_arg++;
				}

				return ov;
			}

			ov++;
		}

		cur_opt++;

		if (*cur_opt == 0)
		{
			cur_arg++;

			goto parse_arg;
		}

		goto parse_opt;
	}
	else if (cur_arg)
	{
	parse_arg:
		char *arg = *cur_arg;

		if (cur_arg < argv + argc && arg && *arg == '-')
		{
			arg++;

			char ac = *arg;

			if (ac)
			{
				if (ac == '-') cur_arg++;
				else
				{
					cur_opt = arg;

					goto parse_opt;
				}
			}
		}

		cur_opt = NULL;
		not_opt = cur_arg;
		cur_arg = NULL;

		return NULL;	
	}
	else
	{
		not_opt = NULL;

		if (argv == NULL) return NULL;

		cur_arg = argv + 1;

		goto parse_arg;
	}
}