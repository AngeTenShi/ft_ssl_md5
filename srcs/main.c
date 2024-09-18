#include "ft_ssl_md5.h"

void print_usage(void)
{
	write(1, "usage: ft_ssl command [flags] [file/string]\n", 44);
}

void print_error(const char *error)
{
	write(2, "ft_ssl: Error: ", 15);
	write(2, error, ft_strlen(error));
}

void md5(const char *input)
{
	(void)input;
}

void sha256(const char *input)
{
	(void)input;
}

void execute_function(t_hash_function *hash_functions, const char *command, const char *input)
{
	t_hash_function *current_function;

	current_function = hash_functions;
	while (current_function)
	{
		if (ft_strlen(command) == ft_strlen(current_function->name) && !ft_strncmp(command, current_function->name, ft_strlen(command)))
		{
			current_function->function(input);
			return ;
		}
		current_function = current_function->next;
	}
	print_error(command);
	write(2, " is an invalid command.\n\nCommands:\nmd5\nsha256\n\nFlags:\n-p -q -r -s\n", 66);
}

int main(int ac, char **av)
{
	t_hash_function *hash_functions = NULL;
	t_options *options = (t_options *)malloc(sizeof(t_options));

	if (ac < 3)
	{
		print_usage();
		return (1);
	}
	parse_options(options, ac, av);
	add_function_to_list(&hash_functions, "md5", md5);
	add_function_to_list(&hash_functions, "sha256", sha256);
	execute_function(hash_functions, av[1], av[2]);
	free(hash_functions);
	free(options);
	return (0);
}
