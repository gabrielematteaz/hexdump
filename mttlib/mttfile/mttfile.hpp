#ifndef MTTFILE_HPP
#define MTTFILE_HPP

#include <vector>
#include <fstream>

std::vector < char > mttfile_load_file(const char *name, std::streampos off, std::streamsize lim, bool str);

#endif