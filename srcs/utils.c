#include "ft_ssl_md5.h"

int ft_strlen(const char *str)
{
	int i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void parse_options(t_options *options, int ac, char **av)
{
	// make a function that parses the options
	int i = 0;

	while(i < ac)
	{
		if (ft_strlen(av[i]) == 2 && av[i][0] == '-')
		{
			if (av[i][1] == 'p')
				options->p = 1;
			else if (av[i][1] == 'q')
				options->q = 1;
			else if (av[i][1] == 'r')
				options->r = 1;
			else if (av[i][1] == 's')
				options->s = 1;
		}
		i++;
	}
}


void ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}
