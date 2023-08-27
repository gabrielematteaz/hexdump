#include "file.h"

char *mtt_file_load(const char *name, long off, size_t count, size_t *size, int str)
{
	FILE* file = fopen(name, "rb");

	if (file)
	{
		size_t s;
	
		if (count == 0)
		{
			fseek(file, 0, SEEK_END);
			s = ftell(file);
			count = s;

			if (str) s++;
		}
		else
		{
			s = count;

			if (str) count--;
		}

		char *cont = malloc(s);

		if (cont)
		{
			if (fseek(file, off, SEEK_SET) == 0)
			{
				count = fread(cont, 1, count, file);
				fclose(file);

				if (str) cont[count] = 0;

				if (size) *size = count;

				return cont;
			}

			free(cont);
		}

		fclose(file);
	}

	return NULL;
}