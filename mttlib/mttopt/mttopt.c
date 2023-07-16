#include "mttopt.h"

int mttopt_extract_optv(int argc, char *argv[], int optc, struct mttopt_opt_t *optv)
{
	char **av, **avc, *arg, *a, ac, argmode, copymode;
	struct mttopt_opt_t *ov, *ovc;

	if (argv != NULL && optv != NULL)
	{
		if (argc <= 0)
		{
			av = argv + 1;

			while (*av != NULL) av++;

			argc = av - argv;
		}

		av = argv + 1;
		avc = argv + argc;

		if (optc <= 0)
		{
			ov = optv;

			while (ov->val) ov++;

			optc = ov - optv;
		}

		ovc = optv + optc;
		
		while (av < avc)
		{
			arg = *av;

			if (*arg == '-')
			{
				a = arg + 1;

				if (*a == '-')
				{
					av++;

					break;
				}

				ac = *a;

				while (ac)
				{
					ov = optv;

					while (ov < ovc)
					{
						if (ac == ov->val)
						{
							if (ov->status == FOUND)
							{
								copymode = ov->copymode;

								if (copymode == IGNORE_COPIES)
								{
									argmode = ov->argmode;

									switch (argmode)
									{
									case MUST_HAVE_ARG:
										a++;
										ac = *a;

										if (ac == 0) av++;
									case CAN_HAVE_ARG:
										goto next;
									}

									break;
								}
								else if (copymode == EXIT_ON_COPY)
								{
									av++;

									if (ov->argmode == MUST_HAVE_ARG)
									{
										a++;
										ac = *a;

										if (ac == 0) av++;
									}

									goto exit;
								}
							}

							ov->status = FOUND;
							argmode = ov->argmode;

							if (argmode == CAN_HAVE_ARG)
							{
								a++;
								ac = *a;
								ov->arg = ac ? a : NULL;

								goto next;
							}
							else if (argmode == MUST_HAVE_ARG)
							{
								a++;
								ac = *a;

								if (ac) ov->arg = a;
								else
								{
									av++;
									ov->arg = *av;
								}
								
								goto next;
							}

							ov->arg = NULL;

							break;
						}

						ov++;
					}

					a++;
					ac = *a;
				}
			}
			else break;

		next:
			av++;
		}

	exit:
		return av - argv;
	}

	return 0;
}