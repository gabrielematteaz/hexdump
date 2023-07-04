#include <stdio.h>
#include "mttstr.h"
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
	char *fname = "mttstr.h";
	size_t ofs = 0, lim = 0, sects = 2, cols = 8;

	FILE* file = fopen(fname, "rb");

	if (file)
	{
		char *cont;

		if (lim == 0) fseek(file, 0, SEEK_END), lim = ftell(file);

		cont = malloc(lim);

		if (cont)
		{
			size_t halflinesize, linesize, maxrnlen;
			char rownum[16], *line, *contc = cont, *cl;

			fseek(file, ofs, SEEK_SET), lim = fread(cont, 1, lim, file), cont = realloc(cont, lim);
			maxrnlen = mttstr_uval_to_str_s(rownum, sizeof(rownum), lim - lim % sects * cols, 16, 0), halflinesize = maxrnlen + 3 + (sects * (cols * 2 + cols - 1) + 2 * (sects - 1)) +  3,  linesize = halflinesize + (sects * cols + sects - 1);
			line = malloc(linesize + 1);

			if (line)
			{
				char *lc = line, *lca;
				size_t c, sect = 0, col;

				line[linesize] = 0;

				lc = lc + maxrnlen;

				c = 0;

				while (c < 3) *lc++ = ' ', c++;

				while (1)
				{
					col = 0;

					while (1)
					{
						lc = lc + 2;

						col++;

						if (col == cols) break;

						c = 0;

						while (c < 1) *lc++ = ' ', c++;
					}

					sect++;

					if (sect == sects) break;

					c = 0;

					while (c < 2) *lc++ = ' ', c++;
				}

				c = 0;

				while (c < 3) *lc++ = ' ', c++;

				sect = 0;

				while (1)
				{
					lc = lc + cols;

					sect++;

					if (sect == sects) break;

					c = 0;

					while (c < 1) *lc++ = ' ', c++;
				}

				cl = contc + lim;

				while (1)
				{
					sect = 0, lc = line + mttstr_uval_to_fstr(line, contc - cont, 16, maxrnlen, VTFS_PREP_ZEROS) + 3, lca = line + halflinesize;

					while (1)
					{
						col = 0;

						while (1)
						{
							char ccc;

							if (contc == cl)
							{
								while (lc < line + halflinesize) *lc++ = ' ';

								while (lca < line + linesize) *lca++ = ' ';

								puts(line);

								goto exit;
							}

							ccc = *contc, lc = lc + mttstr_uval_to_fstr(lc, ccc, 16, 2, VTFS_PREP_ZEROS), *lca++ = isprint(ccc) ? ccc : '.';

							contc++;

							col++;

							if (col == cols) break;

							lc++;
						}

						sect++;

						if (sect == sects) break;

						lca++, lc = lc + 2;
					}

					puts(line);
				}

			exit:
				free(line);
			}

			free(cont);
		}

		fclose(file);
	}

	return 0;
}