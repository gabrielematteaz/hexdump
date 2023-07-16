#include "mttfile.h"

size_t mttfile_load_file(const char *name, char **cont, long off, size_t *lim, enum mttfile_flags_t flags)
{
	FILE* file;
	size_t l, size;
	char *c;

	if (name != NULL && cont != NULL)
	{
		file = fopen(name, "rb");

		if (file != NULL)
		{
			l = lim == NULL ? 0 : *lim;

			if (l == 0)
			{
				fseek(file, 0, SEEK_END);
				l = ftell(file);
				size = l;

				if (flags & STRING) size++;
			}
			else
			{
				size = l;

				if (flags & STRING) l--;
			}

			if (fseek(file, off, SEEK_SET) == 0)
			{
				c = malloc(size);

				if (c != NULL)
				{
					l = fread(c, 1, l, file);

					if (flags & STRING)
					{
						c[l] = 0;

						if (flags & RESIZE)
						{
							size = l + 1;
							c = realloc(c, size);
						}
					}
					else if (flags & RESIZE)
					{
						size = l;
						c = realloc(c, size);
					}

					*cont = c;
					
					if (lim) *lim = l;

					fclose(file);

					return size;
				}
			}

			fclose(file);
		}
	}

	return 0;
}