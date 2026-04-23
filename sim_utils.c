/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaung <akaung@student.42.sg>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:42:02 by akaung            #+#    #+#             */
/*   Updated: 2026/04/12 15:48:23 by akaung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation_end(t_philo *p)
{
	int	end;

	pthread_mutex_lock(&p->params->death_mutex);
	end = p->params->someone_died;
	pthread_mutex_unlock(&p->params->death_mutex);
	return (end);
}

void	eat(t_philo *p)
{
	if (p->params->n == 1)
	{
		single_philo_eat(p);
		return ;
	}
	take_forks(p);
	pthread_mutex_lock(&p->meal_mutex);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->meal_mutex);
	print_state(p, "is eating");
	ft_sleep(p->params->time_to_eat, p->params);
	put_forks(p);
	pthread_mutex_lock(&p->meal_mutex);
	p->times_eaten++;
	pthread_mutex_unlock(&p->meal_mutex);
}

int	check_philo_death(t_params *params, int i)
{
	unsigned long long	now;

	pthread_mutex_lock(&params->philos[i].meal_mutex);
	now = get_time();
	if (now - params->philos[i].last_meal > params->time_to_die)
	{
		pthread_mutex_lock(&params->death_mutex);
		params->someone_died = true;
		pthread_mutex_unlock(&params->death_mutex);
		pthread_mutex_lock(&params->print_mutex);
		printf("%llu %d died\n", now - params->start_time,
			params->philos[i].id);
		pthread_mutex_unlock(&params->print_mutex);
		pthread_mutex_unlock(&params->philos[i].meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&params->philos[i].meal_mutex);
	return (0);
}

int	check_all_fed(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		pthread_mutex_lock(&params->philos[i].meal_mutex);
		if (params->philos[i].times_eaten < params->must_eat)
		{
			pthread_mutex_unlock(&params->philos[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&params->philos[i].meal_mutex);
		i++;
	}
	return (1);
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
