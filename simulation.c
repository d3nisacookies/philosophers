/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaung <akaung@student.42.sg>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:42:16 by akaung            #+#    #+#             */
/*   Updated: 2026/04/12 15:51:49 by akaung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_philosopher_threads(t_params *t)
{
	int	i;

	i = 0;
	while (i < t->n)
	{
		t->philos[i].last_meal = t->start_time;
		if (pthread_create(&t->philos[i].thread, NULL, routine,
				&t->philos[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(t->philos[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	join_all_threads(t_params *t, pthread_t monitor_thread)
{
	int	i;

	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < t->n)
	{
		pthread_join(t->philos[i].thread, NULL);
		i++;
	}
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		usleep(1000);
	while (!simulation_end(p))
	{
		eat(p);
		if (should_philo_exit(p))
			break ;
		if (simulation_end(p))
			break ;
		print_state(p, "is sleeping");
		ft_sleep(p->params->time_to_sleep, p->params);
		if (simulation_end(p))
			break ;
		print_state(p, "is thinking");
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_params	*params;
	int			i;

	params = (t_params *)arg;
	while (!params->someone_died)
	{
		i = 0;
		while (i < params->n)
		{
			if (check_philo_death(params, i))
				return (NULL);
			i++;
		}
		if (params->must_eat > 0 && check_all_fed(params))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	start_simulation(t_params *t)
{
	pthread_t	monitor_thread;

	t->start_time = get_time();
	if (create_philosopher_threads(t) != 0)
		return (printf("thread creation failed\n"), 1);
	if (pthread_create(&monitor_thread, NULL, monitor, t) != 0)
		return (printf("monitor thread creation failed\n"), 1);
	join_all_threads(t, monitor_thread);
	return (0);
}
