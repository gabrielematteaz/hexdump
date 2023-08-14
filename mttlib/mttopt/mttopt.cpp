#include "mttopt.hpp"

mttopt_opt_t::mttopt_opt_t() noexcept
	: shrt(0), flags(0), found(false), arg(nullptr)
{

}

mttopt_opt_t::mttopt_opt_t(uint8_t shrt, uint8_t flags) noexcept
	: shrt(shrt), flags(flags), found(false), arg(nullptr)
{

}

int mttopt_extr_optv(int argc, char *argv[], mttopt_optv_t &optv) noexcept
{
	if (argv == nullptr) return 0;

	char **av = argv + 1, **avc = argv + argc;
	mttopt_opt_t *ovc = optv.data() + optv.size();
	
	while (av < avc)
	{
		char *arg = *av;

		if (*arg == '-')
		{
			char *a = arg + 1;

			if (*a == '-')
			{
				av++;

				break;
			}

			char ac = *a;

			while (ac)
			{
				mttopt_opt_t *ov = optv.data();

				while (ov < ovc)
				{
					if (ac == ov->shrt)
					{
						uint8_t flags = ov->flags;

						if (ov->found)
						{
							if (flags & OPT_FLAGS_IGNORE_COPIES)
							{
								if (flags & OPT_FLAGS_CAN_HAVE_ARG)
								{
									if (flags & OPT_FLAGS_MUST_HAVE_ARG)
									{
										a++;

										if (*a == 0) av++;
									}

									goto next;
								}

								break;
							}
							else if (flags & OPT_FLAGS_EXIT_ON_COPY)
							{
								av++;

								if (flags & OPT_FLAGS_MUST_HAVE_ARG)
								{
									a++;

									if (*a == 0) av++;
								}

								goto exit;
							}
						}

						ov->found = 1;

						if (flags & OPT_FLAGS_CAN_HAVE_ARG)
						{
							a++;

							if ((flags & OPT_FLAGS_MUST_HAVE_ARG) == 12)
							{
								if (*a) ov->arg = a;
								else
								{
									av++;
									ov->arg = *av;
								}
							}
							else ov->arg = *a ? a : nullptr;

							goto next;
						}

						ov->arg = nullptr;

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