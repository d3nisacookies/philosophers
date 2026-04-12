/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaung <akaung@student.42.sg>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:52:36 by akaung            #+#    #+#             */
/*   Updated: 2026/04/12 15:52:37 by akaung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL));
}

void	ft_sleep(unsigned long long time, t_params *params)
{
	unsigned long long	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		pthread_mutex_lock(&params->death_mutex);
		if (params->someone_died)
		{
			pthread_mutex_unlock(&params->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&params->death_mutex);
		usleep(500);
	}
}

void	print_state(t_philo *p, char *msg)
{
	unsigned long long	time;

	pthread_mutex_lock(&p->params->death_mutex);
	if (p->params->someone_died)
	{
		pthread_mutex_unlock(&p->params->death_mutex);
		return ;
	}
	pthread_mutex_lock(&p->params->print_mutex);
	pthread_mutex_unlock(&p->params->death_mutex);
	time = get_time() - p->params->start_time;
	printf("%llu %d %s\n", time, p->id, msg);
	pthread_mutex_unlock(&p->params->print_mutex);
}

int	ft_atoi_philo(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	if (str[i] != '\0' || result > INT_MAX || result <= 0)
		return (-1);
	return (result);
}

int	should_philo_exit(t_philo *p)
{
	if (p->params->must_eat > 0)
	{
		pthread_mutex_lock(&p->meal_mutex);
		if (p->times_eaten >= p->params->must_eat)
		{
			pthread_mutex_unlock(&p->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&p->meal_mutex);
	}
	return (0);
}
