#include "minishell.h"

void test()
{
	t_dict * dict = init_dict();
	dict_add(dict, "hey\0", "hello\0");
	dict_add(dict, "huy\0", "penis\0");
	dict_add(dict, "hello\0", "world\0");

	printf("%s", dict_get(dict, "hey\0", "nothing found"));
	printf("%s", dict_get(dict, "huy\0", "nothing found"));
	printf("%s\n", dict_get(dict, "hello\0", "nothing found"));
	dict_delete(dict, "hey\0");
	printf("deleted 1 of 3\n");
	dict_delete(dict, "huy\0");
	printf("deleted 2 of 3\n");
	printf("%s\n", dict_get(dict, "hello\0", "nothing found"));
	free_dict(dict);
}

void test2()
{
	char test[] = "$hey hello hi\0";
	t_settings *settings = create_setttings();
	t_next_arg_return *res;
	int i;

	dict_add(settings->env, "hey\0", "hello\0");
	printf("created test string\n");
	res=get_next_arg(test, settings);
	printf("%s\n", res->arg);
	i = res->last_index;
	free_next_arg_return(res);
	res = get_next_arg(test + i, NULL);
	printf("%s\n", res->arg);
	i += res->last_index;
	free_next_arg_return(res);
	res = get_next_arg(test + i, NULL);
	printf("%s\n", res->arg);
	i += res->last_index;
	free_next_arg_return(res);
	res = get_next_arg(test + i, NULL);
	printf("%s\n", res->arg);
	i += res->last_index;
	free_next_arg_return(res);
	clear_settings(settings);


}

int main()
{	
	test2();
	system("leaks a.out");

}