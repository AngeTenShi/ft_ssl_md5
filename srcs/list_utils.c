#include "ft_ssl_md5.h"

void add_function_to_list(t_hash_function **hash_functions, const char *name, hash_function function)
{
	t_hash_function *new_function;

	if (*hash_functions == NULL)
	{
		*hash_functions = (t_hash_function *)malloc(sizeof(t_hash_function));
		(*hash_functions)->name = name;
		(*hash_functions)->function = function;
		(*hash_functions)->next = NULL;
		return ;
	}
	new_function = *hash_functions;
	while (new_function->next != NULL)
		new_function = new_function->next;
	new_function->next = (t_hash_function *)malloc(sizeof(t_hash_function));
	new_function->next->name = name;
	new_function->next->function = function;
	new_function->next->next = NULL;
}


void	free_function_list(t_hash_function *list)
{
	t_hash_function *current_node;
	t_hash_function *next_node;

	current_node = list;
	while (current_node)
	{
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}

}
