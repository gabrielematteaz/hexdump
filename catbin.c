#include "mtt/cmdline.h"
#include "mtt/file.h"
#include "mtt/str.h"

int main(int argc, char *argv[])
{
	struct mtt_cmdline_opt_t optv[] = {
		{ 'o', REQUIRED_ARG },
		{ 'c', REQUIRED_ARG },
		{ 's', REQUIRED_ARG }
	};

	long off = 0;
	size_t count = 256, sectcols = 16;
	struct mtt_str_fmt_t optfmt = { '+', 0, ' ', 10, .fillmode = LEFT };

	while (1)
	{
		struct mtt_cmdline_opt_t *opt = mtt_cmdline_extr_opt(argc, argv, 3, optv);

		if (opt == NULL) break;

		int alias = opt->alias;

		switch (alias)
		{
		case 'o':
			off = mtt_str_fstr_to_ival(opt->arg, NULL, optfmt);

			break;
		case 'c':
			count = mtt_str_fstr_to_ival(opt->arg, NULL, optfmt);

			break;
		case 's':
			sectcols = mtt_str_fstr_to_ival(opt->arg, NULL, optfmt);

			break;
		}
	}

	size_t contsize;
	char *cont = mtt_file_load(*not_opt, off, count, &contsize, 0);

	if (cont)
	{
		if (sectcols == 0) sectcols = contsize;

		size_t maxrowcount = contsize / sectcols;

		if (contsize % sectcols) maxrowcount++;

		struct mtt_str_fmt_t hexfmt = { 0, 0, '0', 16, UPPER, LEFT, 1, 0 };
		size_t maxrowcountlen = mtt_str_ival_to_fstr(NULL, maxrowcount - 1, hexfmt);
		char *line = malloc(maxrowcountlen + 3 + sectcols * 2 + sectcols - 1 + 3 + sectcols + 1);

		if (line)
		{
			char *l = line + maxrowcountlen;

			*l = ' ';
			l++;
			*l = ' ';
			l++;
			*l = ' ';
			l++;

			char *hexsect = l;

			size_t sectcol = 1;

			while (1)
			{
				l = l + 2;

				if (sectcol == sectcols) break;

				*l = ' ';
				l++;
				sectcol++;
			}

			*l = ' ';
			l++;
			*l = ' ';
			l++;
			*l = ' ';
			l++;

			char *textsect = l;
			
			l = l + sectcols;
			*l = 0;

			char *c = cont, *cs = c + contsize;
			size_t rowcount = 0;

			while (1)
			{
				hexfmt.width = maxrowcountlen;
				mtt_str_ival_to_fstr(line, rowcount, hexfmt);
				hexfmt.width = 2;
				sectcol = 1;

				char *hs = hexsect, *ts = textsect;

				while (1)
				{
					unsigned char cc = *c;

					mtt_str_ival_to_fstr(hs, cc, hexfmt);
					*ts = cc < 32 || cc > 126 ? '.' : cc;

					c++;

					if (c == cs)
					{
						ts++;

						while (sectcol < sectcols)
						{
							hs = hs + 3;
							hs[0] = ' ';
							hs[1] = ' ';
							sectcol++;
						}

						*ts = 0;
						puts(line);

						goto exit;
					}

					if (sectcol == sectcols) break;

					sectcol++;
					hs = hs + 3;
					ts++;
				}

				puts(line);
				rowcount++;
			}

		exit:
			free(line);
		}

		free(cont);
	}

	return 0;
}