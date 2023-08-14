#include "../mttlib/mttopt/mttopt.hpp"
#include "../mttlib/mttstr/mttstr.hpp"
#include "../mttlib/mttfile/mttfile.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	mttopt_optv_t optv = {
		mttopt_opt_t('o', OPT_FLAGS_MUST_HAVE_ARG),
		mttopt_opt_t('l', OPT_FLAGS_MUST_HAVE_ARG),
		mttopt_opt_t('s', OPT_FLAGS_MUST_HAVE_ARG),
		mttopt_opt_t('c', OPT_FLAGS_MUST_HAVE_ARG)
	};

	char **av = argv + mttopt_extr_optv(argc, argv, optv);
	mttstr_fmt_t optfmt(10, '+', 0, ' ', FMT_FLAGS_LEFT_FILL, 0);
	std::streampos off = optfmt.fstr_to_ival(optv[0].arg, nullptr);
	std::streamsize lim = optv[1].found ? optfmt.fstr_to_ival(optv[1].arg, nullptr) : 256;
	std::vector < char > cont;

	try
	{
		cont = mttfile_load_file(*av, off, lim, false);
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
		
		return 1;
	}

	std::size_t sects, cols;
	
	if (optv[2].found)
	{
		sects = optfmt.fstr_to_ival(optv[2].arg, nullptr);

		if (sects == 0) sects = 2;
	}
	else sects = 2;

	if (optv[3].found)
	{
		cols = optfmt.fstr_to_ival(optv[3].arg, nullptr);

		if (cols == 0) cols = 8;
	}
	else cols = 8;

	std::size_t bytesxrow = sects * cols, contsize = cont.size(), rowcount = contsize / bytesxrow;

	if (contsize % bytesxrow) rowcount++;

	mttstr_fmt_t hexfmt(16, 0, 0, '0', FMT_FLAGS_LEFT_FILL, 0);
	std::size_t rcwidth = hexfmt.ival_to_fstr(nullptr, rowcount), linesize = rcwidth + 3 + sects * (2 * cols + cols - 1) + 2 * (cols - 1) + 3 + sects * cols + cols;
	char *line = new char[linesize];

	if (line == nullptr) return 2;

	char *l = line + rcwidth;

	*l = ' ';
	l++;
	*l = ' ';
	l++;
	*l = ' ';
	l++;

	std::size_t sect = 1;
	char *lhexsect = l;

	while (true)
	{
		std::size_t col = 1;

		while (true)
		{
			l = l + 2;

			if (col == cols) break;

			*l = ' ';
			l++;
			col++;
		}

		if (sect == sects) break;

		*l = ' ';
		l++;
		*l = ' ';
		l++;
		sect++;
	}

	*l = ' ';
	l++;
	*l = ' ';
	l++;
	*l = ' ';
	l++;

	char *lcharsect = l;

	sect = 1;

	while (true)
	{
		l = l + cols;

		if (sect == sects) break;

		sect++;
		*l = ' ';
		l++;
	}

	*l = 0;

	std::size_t row = 0;
	const char *c = cont.data(), *cs = c + contsize;

	while (true)
	{
		hexfmt.width = rcwidth;
		hexfmt.ival_to_fstr(line, row);
		hexfmt.width = 2;
		sect = 1;

		char *lhs = lhexsect, *lcs = lcharsect;

		while (true)
		{
			std::size_t col = 1;

			while (true)
			{
				if (c == cs)
				{
					if (sect > 1 || col > 1)
					{
						while (true)
						{
							if (sect > sects) break;

							while (true)
							{
								*lhs = ' ';
								lhs++;
								*lhs = ' ';
								lhs++;

								if (col == cols) break;

								lhs++;
								col++;
							}

							lhs = lhs + 2;
							sect++;
							col = 1;
						}

						*lcs = 0;
						std::cout << line << std::endl;
					}

					goto exit;
				}

				uint8_t cc = *c;

				c++;
				hexfmt.ival_to_fstr(lhs, cc);
				*lcs = 32 <= cc && cc <= 126 ? cc : '.';

				if (col == cols) break;

				lhs = lhs + 3;
				lcs++;
				col++;
			}

			if (sect == sects) break;

			sect++;
			lhs = lhs + 4;
			lcs = lcs + 2;
		}

		std::cout << line << std::endl;
		row++;
	}

exit:
	delete[] line;

	return 0;
}