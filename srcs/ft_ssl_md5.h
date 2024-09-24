#ifndef FT_SSL_MD5_H
# define FT_SSL_MD5_H

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct string_list {
	char *str;
	int from_file;
	struct string_list *next;
} t_string_list;

typedef struct options {
	int p;
	int q;
	int r;
	t_string_list *strings; // for -s option
} t_options;

typedef void(*hash_function)(t_options *options);

typedef struct s_hash_function {
	const char *name;
	hash_function function;
	struct s_hash_function *next;
} t_hash_function;

// libft
int ft_strlen(const char *str);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void ft_memcpy(void *dst, const void *src, size_t n);
char *ft_strdup(const char *s);

// utils
void parse_options(t_options *options, int ac, char **av);
void print_hexa(const char *str, int len);

// list operations
void add_function_to_list(t_hash_function **hash_functions, const char *name, hash_function function);
void add_string_to_list(t_string_list **list, char *str, int from_file);

// free
void free_function_list(t_hash_function *list);
void free_hash_list(t_string_list *list);

// helpers
void md5_helper(t_options *options);
void sha256_helper(t_options *options);
void whirlpool_helper(t_options *options);
#endif
