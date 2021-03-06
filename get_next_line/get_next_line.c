/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungsle <seungsle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 14:28:24 by seungsle          #+#    #+#             */
/*   Updated: 2021/06/04 14:28:33 by seungsle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		verify_buff(char **backup)
{
	int			i;
	char		*temp;

	i = 0;
	if (!*backup)
		return (0);
	temp = *backup;
	while (temp[i])
	{
		if (temp[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char		*get_line(char **backup)
{
	int			i;
	char		*temp_backup;
	char		*ret;

	i = 0;
	temp_backup = *backup;
	if (!*backup)
		return (0);
	while (temp_backup[i] && temp_backup[i] != '\n')
		i++;
	if (!(ret = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	i = 0;
	while (temp_backup[i] && temp_backup[i] != '\n')
	{
		ret[i] = temp_backup[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

static char		*get_backup(char **backup)
{
	int			i;
	int			j;
	char		*temp_backup;
	char		*ret;

	i = 0;
	j = 0;
	temp_backup = *backup;
	if (!*backup)
		return (0);
	while (temp_backup[i] && temp_backup[i] != '\n')
		i++;
	if (!temp_backup[i])
	{
		free(*backup);
		return (0);
	}
	if (!(ret = (char *)malloc(sizeof(char) * ((ft_strlen(*backup) - i) + 1))))
		return (NULL);
	i++;
	while (temp_backup[i])
		ret[j++] = temp_backup[i++];
	ret[j] = '\0';
	free(*backup);
	return (ret);
}

int				get_next_line(int fd, char **line)
{
	static char	*backup[OPEN_MAX];
	char		*buff;
	int			read_size;

	read_size = 1;
	if (!line || BUFFER_SIZE <= 0 || (fd < 0 || fd >= OPEN_MAX) ||
			!(buff = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		return (-1);
	while (verify_buff(&backup[fd]) == 0 && read_size != 0)
	{
		if ((read_size = read(fd, buff, BUFFER_SIZE)) == -1)
		{
			free(buff);
			return (-1);
		}
		buff[read_size] = '\0';
		backup[fd] = ft_strjoin(backup[fd], buff);
	}
	free(buff);
	*line = get_line(&backup[fd]);
	backup[fd] = get_backup(&backup[fd]);
	if (read_size == 0)
		return (0);
	return (1);
}
