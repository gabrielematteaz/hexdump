#include "../mttlib/mttfile/mttfile.h"
#include "../mttlib/mttopt/mttopt.h"
#include "../mttlib/mttstr/mttstr.h"

int main(int argc, char *argv[])
{
	struct mttopt_opt_t optv[] = {
		{ 'o', 0, MUST_HAVE_ARG, 0 },
		{ 'l', 0, MUST_HAVE_ARG, 0 },
		{ 's', 0, MUST_HAVE_ARG, 0 },
		{ 'c', 0, MUST_HAVE_ARG, 0 }
	};

	int avoff = mttopt_extract_optv(argc, argv, sizeof(optv) / sizeof(*optv), optv);
	struct mttstr_fmt_t optfmt = { 0, '-', ' ', 10, 0 }, rowfmt, hexfmt;
	long off = mttstr_fstr_to_ival(optv[0].arg, NULL, optfmt);
	char *cont, *line, *l, *lhexsect, *lcharsect, *lhs, *lcs, *c, *cs;
	size_t lim = optv[1].status == FOUND ? mttstr_fstr_to_ival(optv[1].arg, NULL, optfmt) : 256, size = mttfile_load_file(argv[avoff], &cont, off, &lim, RESIZE), sects, cols, bytesxrow, rowcount, linesize, sect, col, row;
	unsigned char cc;

	if (size)
	{
		if (optv[2].status == FOUND)
		{
			sects = mttstr_fstr_to_ival(optv[2].arg, NULL, optfmt);

			if (sects == 0) sects = 2;
		}
		else sects = 2;

		if (optv[3].status == FOUND)
		{
			cols = mttstr_fstr_to_ival(optv[3].arg, NULL, optfmt);

			if (cols == 0) cols = 8;
		}
		else cols = 8;

		bytesxrow = sects * cols;
		rowcount = size / bytesxrow;

		if (size % bytesxrow) rowcount++;

		rowfmt.plus = 0;
		rowfmt.minus = 0;
		rowfmt.fill = '0';
		rowfmt.base = 16;
		rowfmt.fillmode = LEFT;
		rowfmt.flags = 0;
		rowfmt.width = 0;
		rowfmt.width = mttstr_ival_to_fstr(NULL, rowcount, rowfmt);
		printf("%zu\n", rowfmt.width);
		linesize = rowfmt.width + 3 + sects * (2 * cols + cols - 1) + 2 * (sects - 1) + 3 + sects * cols + sects;
		line = malloc(linesize);

		if (line != NULL)
		{
			l = line + rowfmt.width;
			*l = ' ';
			l++;
			*l = ' ';
			l++;
			*l = ' ';
			l++;
			lhexsect = l;
			sect = 0;

			while (1)
			{
				sect++;
				col = 0;

				while (1)
				{
					l = l + 2;
					col++;

					if (col == cols) break;

					*l = ' ';
					l++;
				}

				if (sect == sects) break;

				*l = ' ';
				l++;
				*l = ' ';
				l++;
			}

			*l = ' ';
			l++;
			*l = ' ';
			l++;
			*l = ' ';
			l++;
			lcharsect = l;
			sect = 0;

			while (1)
			{
				l = l + cols;
				sect++;

				if (sect == sects) break;

				*l = ' ';
				l++;
			}

			hexfmt.plus = 0;
			hexfmt.minus = 0;
			hexfmt.fill = '0';
			hexfmt.base = 16;
			hexfmt.fillmode = LEFT;
			hexfmt.flags = 0;
			hexfmt.width = 2;
			*l = 0;
			c = cont;
			cs = c + size;
			row = 0;

			while (1)
			{
				mttstr_ival_to_fstr(line, row, rowfmt);
				lhs = lhexsect;
				lcs = lcharsect;
				sect = 0;
				row++;

				while (1)
				{
					sect++;
					col = 1;

					while (1)
					{
						cc = *c;
						lhs = lhs + mttstr_ival_to_fstr(lhs, cc, hexfmt);
						*lcs = 32 <= cc && cc <= 126 ? cc : '.';
						lcs++;
						c++;

						if (c == cs)
						{
							while (1)
							{
								while (col < cols)
								{
									lhs++;
								next:
									*lhs = ' ';
									lhs++;
									*lhs = ' ';
									lhs++;
									*lcs = ' ';
									lcs++;
									col++;
								}

								if (sect == sects) break;

								lhs = lhs + 2;
								lcs++;
								sect++;
								col = 0;

								goto next;
							}

							puts(line);

							goto exit;
						}

						if (col == cols) break;

						lhs++;
						col++;
					}

					if (sect == sects) break;

					lhs = lhs + 2;
					lcs++;
				}

				puts(line);
			}

		exit:
			free(line);
		}

		free(cont);
	}

	return 0;
}