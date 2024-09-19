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

void add_function_to_list(t_hash_function **hash_functions, const char *name, hash_function function);
int ft_strlen(const char *str);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void parse_options(t_options *options, int ac, char **av);
void ft_strncpy(char *dst, const char *src, size_t n);
void ft_memcpy(void *dst, const void *src, size_t n);
void print_hexa(char *str, int len);
void add_string_to_list(t_string_list **list, char *str, int from_file);
void free_hash_list(t_string_list *list);
char *ft_strdup(const char *s);

// md5 operations
unsigned int F(unsigned int b, unsigned int c, unsigned int d);
unsigned int G(unsigned int b, unsigned int c, unsigned int d);
unsigned int H(unsigned int b, unsigned int c, unsigned int d);
unsigned int I(unsigned int b, unsigned int c, unsigned int d);
unsigned int leftrotate(unsigned int x, unsigned int c);
void md5_helper(t_options *options);
void sha256_helper(t_options *options);

#endif
