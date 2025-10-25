#include "minirt.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	lens;
	size_t	n;

	lens = strlen(src);
	n = dstsize - 1;
	if (dstsize == 0)
	{
		return (lens);
	}
	while (*src && n > 0)
	{
		*dst++ = *src++;
		n--;
	}
	*dst = '\0';
	return (lens);
}

bool ispace(char c)
{
	if(c == ' ' || (c >= 9 && c <= 13))
		return true;
	return false;
}

int count_word(char *str)
{
	int i = 0;
	int flag = 0;
	int counter = 0;
	while(str[i])
	{
		if(ispace(str[i]))
			flag = 0;
		else if (flag == 0)
		{
			flag = 1;
			counter++;
		}
		i++;
	}
	return counter;
}

int	lenght_word(char *str)
{
	int i = 0;
	while(str[i] && str[i] != ' ' && str[i] != '\t')
		i++;
	return i;
}

char **ft_split_white(char *str)
{
	int counts = count_word(str);
	char **arr = malloc(sizeof(char *) * (counts + 1));
	if(!arr)
		return NULL;
	int i = 0;
	while(i < counts)
	{
		while(*str == ' ' || (*str >= 9 && *str <= 13))
			str++;
		arr[i] = malloc(sizeof(char ) * lenght_word(str) + 1);
		if(!arr[i])
			return NULL;
		ft_strlcpy(arr[i], str, lenght_word(str) + 1);
		str = str + lenght_word(str);
		i++;
	}
	arr[i] = NULL;
	return arr;
}

int main()
{
	char *str = "  			fa	hd ben allal    fef is hoat";
	char **splited = ft_split_white(str);
	for(int i = 0; splited[i]; i++)
		printf("[%s]\n", splited[i]);
}


