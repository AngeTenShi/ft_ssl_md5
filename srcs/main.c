#include "ft_ssl_md5.h"
#include <math.h>

void print_usage(void)
{
	write(1, "usage: ft_ssl command [flags] [file/string]\n", 44);
}

void print_error(const char *error)
{
	write(2, "ft_ssl: Error: ", 15);
	write(2, error, ft_strlen(error));
}


void sha256(const char *input, t_options *options)
{
	(void)input;
	(void)options;
}


void execute_function(t_hash_function *hash_functions, const char *command, t_options *options)
{
	t_hash_function *current_function;

	current_function = hash_functions;
	while (current_function)
	{
		if (ft_strncmp(command, current_function->name, ft_strlen(current_function->name)) == 0)
		{
			current_function->function(options);
			return;
		}
		current_function = current_function->next;
	}
	print_error(command);
	write(2, " is an invalid command.\n\nCommands:\nmd5\nsha256\n\nFlags:\n-p -q -r -s\n", 66);
}

int main(int ac, char **av)
{
	t_hash_function *hash_functions = NULL;
	t_options *options = malloc(sizeof(t_options));
	t_string_list *hash_list = NULL;

	if (ac < 3)
	{
		print_usage();
		free(options);
		free(hash_list);
		return (1);
	}
	options->strings = hash_list;
	parse_options(options, ac, av);
	add_function_to_list(&hash_functions, "md5", md5_helper);
	add_function_to_list(&hash_functions, "sha256", sha256_helper);
	execute_function(hash_functions, av[1], options);
	free(hash_functions);
	free_hash_list(hash_list);
	free(options);
	return (0);
}
