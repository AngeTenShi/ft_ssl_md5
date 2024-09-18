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

void md5(const char *input, t_options *options)
{
	unsigned int A = 0x67452301;
	unsigned int B = 0xefcdab89;
	unsigned int C = 0x98badcfe;
	unsigned int D = 0x10325476;

	size_t len = ft_strlen(input);
	size_t new_len = len + 1;
	while (new_len % 512 != 448)
		new_len++;
	new_len += 64;
	char *msg = (char *)malloc(new_len);
	ft_strncpy(msg, input, len);
	msg[len] = 0x80; // ajout du bit 1 a la fin du message 10000000 ex : 01100001 01100010 01100011 + (10000000 = 0x80)
	for (size_t i = len + 1; i < new_len; i++)
		msg[i] = 0; // ajout de 0 apres le bit 1
	// ajouter la taille du message original en bits a la fin du message sur les 8 derniers bits
	for (int i = 0; i < 8; i++)
		msg[new_len - 8 + i] = (len * 8) >> (8 * (7 - i)) & 0xFF;
	printf("msg: %s\n", msg);


}

void sha256(const char *input, t_options *options)
{
}


void execute_function(t_hash_function *hash_functions, const char *command, const char *input, t_options *options)
{
	t_hash_function *current_function;

	current_function = hash_functions;
	while (current_function)
	{
		if (ft_strlen(command) == ft_strlen(current_function->name) && !ft_strncmp(command, current_function->name, ft_strlen(command)))
		{
			current_function->function(input, options);
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
	execute_function(hash_functions, av[1], av[2], options);
	free(hash_functions);
	free(options);
	return (0);
}
