#ifndef MTT_FILE_H
#define MTT_FILE_H

#include <stdio.h>
#include <stdlib.h>

char *mtt_file_load(const char *name, long off, size_t count, size_t *size, int str);

#endif