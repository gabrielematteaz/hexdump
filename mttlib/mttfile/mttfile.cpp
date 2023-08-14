#include "mttfile.hpp"

std::vector < char > mttfile_load_file(const char *name, std::streampos off, std::streamsize lim, bool str)
{
	std::ifstream file(name, std::ios::binary);

	if (file.is_open() == false) throw std::runtime_error("mttfile_load_file");

	std::streamsize size;

	if (lim == 0)
	{
		file.seekg(0, file.end);
		lim = file.tellg();
		size = lim;

		if (str) size++;
	}
	else
	{
		size = lim;

		if (str) lim--;
	}

	file.seekg(off);

	std::vector < char > cont(size);

	file.read(cont.data(), lim);
	lim = file.gcount();
	file.close();

	if (str) cont[lim] = 0;

	return cont;
}