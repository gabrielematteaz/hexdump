#include "../mttlib/mttopt/mttopt.h"
#include "../mttlib/mttfile/mttfile.h"
#include "../mttlib/mttstr/mttstr.h"

int main(int argc, char *argv[])
{
	struct mttopt_opt_t optv[] = {
		{ 'o', 0, MUST_HAVE_ARG, 0 },
		{ 'l', 0, MUST_HAVE_ARG, 0 },
		{ 's', 0, MUST_HAVE_ARG, 0 },
		{ 'c', 0, MUST_HAVE_ARG, 0 },
	};

	int avoff;
	char *cont, *line, *lhs, *lcs, *c, *cs;
	size_t off, lim, size, sects, cols, bytesxrow, rowcount, rownummaxwidth, hexsectoff, charsectoff, linesize, sect, col, rownum;
	unsigned char cc;
	
	avoff = mttopt_extract_optv(argc, argv, sizeof(optv) / sizeof(*optv), optv);
	off = optv[0].status == FOUND ? mttstr_fstr_to_ival(optv[0].arg, NULL, 10, '+', '-', ' ', LEFT, 0) : 0;
	lim = optv[1].status == FOUND ? mttstr_fstr_to_ival(optv[1].arg, NULL, 10, '+', '-', ' ', LEFT, 0) : 256;
	size = mttfile_load_file(argv[avoff], &cont, off, &lim, RESIZE);

	if (size)
	{
		if (optv[2].status == FOUND)
		{
			sects = mttstr_fstr_to_ival(optv[2].arg, NULL, 10, '+', '-', ' ', LEFT, 0);

			if (sects == 0) sects = 2;
		}
		else sects = 2;

		if (optv[3].status == FOUND)
		{
			cols = mttstr_fstr_to_ival(optv[3].arg, NULL, 10, '+', '-', ' ', LEFT, 0);

			if (cols == 0) sects = 8;
		}
		else cols = 8;

		bytesxrow = sects * cols;
		rowcount = size / bytesxrow;

		if (size % bytesxrow) rowcount++;

		rownummaxwidth = mttstr_ival_to_fstr(NULL, rowcount, 16, 0, 0, 0, 0, 0, 0);
		hexsectoff = rownummaxwidth;
		charsectoff = 3 + hexsectoff + sects * (2 * cols + cols - 1) + 2 * (sects - 1);
		linesize = charsectoff + 3 + sects * cols + sects;
		line = malloc(linesize);
		if (line != NULL)
		{
			lhs = line + hexsectoff;
			lcs = line + charsectoff;
			*lhs = ' ';
			lhs++;
			*lhs = ' ';
			lhs++;
			*lhs = ' ';
			lhs++;
			*lcs = ' ';
			lcs++;
			*lcs = ' ';
			lcs++;
			*lcs = ' ';
			lcs++;
			sect = 0;
			while (1)
			{
				col = 0;
				while (1)
				{
					lhs = lhs + 2;
					col++;
					if (col == cols) break;
					*lhs = ' ';
					lhs++;
				}
				lcs = lcs + cols;
				sect++;
				if (sect == sects) break;
				*lhs = ' ';
				lhs++;
				*lhs = ' ';
				lhs++;
				*lcs = ' ';
				lcs++;
			}
			*lcs = 0;
			c = cont;
			cs = c + size;
			rownum = 0;
			while (1)
			{
				mttstr_ival_to_fstr(line, rownum, 16, 0, 0, '0', LEFT, rownummaxwidth, 0);
				lhs = line + hexsectoff + 3;
				lcs = line + charsectoff + 3;
				sect = 0;
				while (1)
				{
					col = 0;
					while (1)
					{
						cc = *c;
						lhs = lhs + mttstr_ival_to_fstr(lhs, cc, 16, 0, 0, '0', LEFT, 2, 0);
						*lcs = 32 <= cc && cc <= 126 ? cc : '.';
						lcs++;
						c++;
						if (c == cs)
						{
							col++;
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
								sect++;
								if (sect == sects) break;
								lhs = lhs + 2;
								lcs++;
								col = 0;
								goto next;
							}
							puts(line);
							goto exit;
						}
						col++;
						if (col == cols) break;
						lhs++;
					}
					sect++;
					if (sect == sects) break;
					lhs = lhs + 2;
					lcs++;
				}
				puts(line);
				rownum++;
			}
		exit:
			free(line);
		}
		free(cont);
	}


	return 0;
}