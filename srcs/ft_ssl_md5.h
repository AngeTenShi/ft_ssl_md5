#ifndef FT_SSL_MD5_H
# define FT_SSL_MD5_H

#include <stdlib.h>
#include <unistd.h>

typedef void(*hash_function)(const char *input);

typedef struct s_hash_function {
	const char *name;
	hash_function function;
	struct s_hash_function *next;
} t_hash_function;

typedef struct options {
	int p;
	int q;
	int r;
	int s;
} t_options;

void add_function_to_list(t_hash_function **hash_functions, const char *name, hash_function function);
int ft_strlen(const char *str);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void parse_options(t_options *options, int ac, char **av);

#endif
