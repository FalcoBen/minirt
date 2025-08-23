#include "minirt.h"

void s()
{
	system("leaks miniRT");
}
char	*rev_string(char *str)
{
	char	t;
	int		f;
	int		l;
	f = 0;
	l = ft_strlen(str) - 1;
	while (f < l)
	{
		t = str[f];
		str[f] = str[l];
		str[l] = t;
		f++;
		l--;
	}
	char *container = strdup(str);
	return (container);
}

bool check_extension(char *str)
{
	char *extension = "tr.";
	char *input_extension = rev_string(str);
	if(strncmp(input_extension, extension, 3) == 0)
	{
		free(input_extension);
		// printf("the file is good \n");
		return true;
	}
	free(input_extension);
	return false;
}


int main(int ac, char **av)
{
	// atexit(s);
	if(ac != 2)
		return 1;
	char *file_name = av[1];
	if(!check_extension(file_name))
	{
		printf("Error\n, bad extension\n");
		return 1;
	}
	char *file_to_open = rev_string(av[1]);
	int fd = open(file_to_open, O_RDONLY, 0644);
	if(fd < 0)
	{
		free(file_to_open);
		printf("failed to open file\n");
		return 2;
	}
	free(file_to_open);
	t_container *list = NULL;
	t_container *head = NULL;
	char *line = get_next_line(fd);
	while(line != NULL)
	{
		list = ft_lstnew(line);
		ft_lstadd_back(&head, list);
		line = get_next_line(fd);
	}
	// t_container *tmp = head;
	// while(tmp)
	// {
	// 	printf("--%s\n", tmp->line);
	// 	tmp = tmp->next;
	// }
	return 0;
}