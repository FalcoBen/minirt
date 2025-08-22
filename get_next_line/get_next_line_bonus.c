/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:25:43 by fbenalla          #+#    #+#             */
/*   Updated: 2024/12/13 21:13:54 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strjoin_free(char *dst, char *src)
{
	char	*block;

	block = ft_strjoin(dst, src);
	free(dst);
	return (block);
}

char	*read_from_file(int fd, char *block)
{
	char	*buffer;
	ssize_t	bytes;

	if (!block)
		block = ft_strdup("");
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (free(block), NULL);
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(block), free(buffer), NULL);
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		block = ft_strjoin_free(block, buffer);
		if (ft_strchr(block, '\n'))
			break ;
	}
	free(buffer);
	return (block);
}

char	*get_preffix(char *str)
{
	char	*preffix;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	preffix = malloc((i + 1) * sizeof(char));
	if (!preffix)
		return (NULL);
	ft_strlcpy(preffix, str, i + 1);
	return (preffix);
}

char	*get_suffix(char *str)
{
	char	*suffix;
	int		i;
	int		j;

	if (!str || !*str)
		return (free(str), NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	if (str[i] == '\0')
		return (free(str), NULL);
	suffix = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!suffix)
		return (free(str), NULL);
	j = 0;
	while (str[i])
		suffix[j++] = str[i++];
	suffix[j] = '\0';
	free(str);
	return (suffix);
}
#include <fcntl.h>
#include <libc.h>

char	*get_next_line(int fd)
{
	static char	*blocks[OPEN_MAX] = {NULL};
	char		*line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, NULL, 0) < 0)
		return (free(blocks[fd]), blocks[fd] = NULL, NULL);
	blocks[fd] = read_from_file(fd, blocks[fd]);
	if (!blocks[fd] || !*blocks[fd])
		return (free(blocks[fd]), blocks[fd] = NULL, NULL);
	line = get_preffix(blocks[fd]);
	if (!line)
		return (free(blocks[fd]), blocks[fd] = NULL, NULL);
	blocks[fd] = get_suffix(blocks[fd]);
	return (line);
}
int main()
{
	int fd1= open("text.txt", O_RDWR | O_CREAT, 0644);
	int fd2= open("text1.txt", O_RDWR | O_CREAT, 0644);
	char *line1 = get_next_line(fd1);
	char *line2 = get_next_line(fd2);

	while(line1 || line2)
	{
		if(line1)
		{
			printf("fd 1: %s", line1);
			free(line1);
		}
		if(line2)
		{
			printf("fd 2 : %s", line2);
			free(line2);
		}

		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);

	}


	close(fd1);
	close(fd2);
	return 0;
}