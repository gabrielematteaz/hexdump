#include <unistd.h>
#include "mttstr.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	size_t ofs = 0;
	size_t lim = 0;
	size_t sects = 2;
	size_t cols = 8;
	FILE* file;

	while (1)
	{
		int opt = getopt(argc, argv, "o:l:s:c:");

		if (opt == -1) break;

		switch (opt)
		{
		case 'o':
			ofs = mttstr_str_to_uval(optarg, NULL, 10, 0);

			break;
		case 'l':
			lim = mttstr_str_to_uval(optarg, NULL, 10, 0);

			break;
		case 's':
			sects = mttstr_str_to_uval(optarg, NULL, 10, 0);

			if (sects == 0) sects = 2;

			break;
		case 'c':
			cols = mttstr_str_to_uval(optarg, NULL, 10, 0);

			if (cols == 0) cols = 8;

			break;
		}
	}

	file = fopen(argv[optind], "rb");

	if (file)
	{
		char *cont;

		if (lim == 0) fseek(file, 0, SEEK_END), lim = ftell(file) - ofs;

		cont = malloc(lim);

		if (cont)
		{
			size_t linesize = sects * (cols * 2 + cols - 1) + 2 * (sects - 1);
			char *line = malloc(linesize + sects * cols + sects + 1);

			if (line)
			{
				size_t sect = 0, col;
				char *l = line, *line2 = l + linesize + 2, *l2 = line2, *c = cont, *cl = c + lim;

				fseek(file, ofs, SEEK_SET), lim = fread(cont, 1, lim, file), cont = realloc(cont, lim);

				while (1)
				{
					sect++, col = 0, l2 = l2 + cols;

					while (1)
					{
						l = l + 2, col++;

						if (col == cols) break;

						*l++ = ' ';
					}

					if (sect == sects) break;

					*l++ = ' ', *l++ = ' ', *l2++ = ' ';
				}

				*l++ = ' ', *l++ = ' ', *l2 = 0;

				while (c < cl)
				{
					sect = 0, l = line, l2 = line2;

					while (1)
					{
						sect++, col = 0;

						while (1)
						{
							unsigned char cc = *c;

							l = l + mttstr_uval_to_fstr(l, cc, 16, 2, VTFS_PREP_ZEROS), *l2++ = isprint(cc) ? cc : '.', c++;

							if (c == cl)
							{
								*l = 0, puts(line);

								goto end;
							}

							col++;

							if (col == cols) break;

							l++;
						}

						if (sect == sects) break;

						l = l + 2, l2++;
					}

					puts(line);
				}

			end:
				free(line);
			}

			free(cont);
		}

		fclose(file);
	}

	return 0;
}