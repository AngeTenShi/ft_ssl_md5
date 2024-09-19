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
	int i = 2; // 1 is command
	int last_flag = 0;
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
			{
				if (i + 1 < ac)
					add_string_to_list(&options->strings, ft_strdup(av[i + 1]), 0);
				else
					write(2, "Warning : ft_ssl: option requires an argument -- s\n", 51);
			}
			last_flag = i;
		}
		// if not a flag and not an argument of flag it's a file
		else if (last_flag + 1 != i)
			add_string_to_list(&options->strings, ft_strdup(av[i]), 1); // 1 for file input
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

void ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
}

void print_hexa(char *str, int len)
{
	int i = 0;
	char hex[16] = "0123456789abcdef";

	while (i < len)
	{
		char dig[2];
		dig[0] = hex[(str[i] >> 4) & 0xf]; // partie haute
		dig[1] = hex[str[i] & 0xf]; // partie basse
		write(1, dig, 2);
		i++;
	}
	write(1, "\n", 1);
}

void add_string_to_list(t_string_list **list, char *str, int from_file)
{
	t_string_list *new_node;
	t_string_list *current_node;

	new_node = (t_string_list *)malloc(sizeof(t_string_list));
	new_node->str = str;
	new_node->from_file = from_file;
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current_node = *list;
	while (current_node->next)
		current_node = current_node->next;
	current_node->next = new_node;
}

void free_hash_list(t_string_list *list)
{
	t_string_list *current_node;
	t_string_list *next_node;

	current_node = list;
	while (current_node)
	{
		next_node = current_node->next;
		free(current_node->str);
		free(current_node);
		current_node = next_node;
	}
}

char *ft_strdup(const char *s)
{
	char *str;
	int i;

	str = (char *)malloc(ft_strlen(s) + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
