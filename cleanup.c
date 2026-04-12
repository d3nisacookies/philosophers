/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaung <akaung@student.42.sg>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:40:45 by akaung            #+#    #+#             */
/*   Updated: 2026/04/12 15:40:46 by akaung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_params *t)
{
	int	i;

	if (!t || !t->forks)
		return ;
	i = 0;
	while (i < t->n)
	{
		pthread_mutex_destroy(&t->forks[i]);
		i++;
	}
	free(t->forks);
	t->forks = NULL;
}

void	destroy_philos(t_params *t)
{
	int	i;

	if (!t || !t->philos)
		return ;
	i = 0;
	while (i < t->n)
	{
		pthread_mutex_destroy(&t->philos[i].meal_mutex);
		i++;
	}
	free(t->philos);
	t->philos = NULL;
}

void	cleanup(t_params *t)
{
	if (!t)
		return ;
	destroy_philos(t);
	destroy_forks(t);
	pthread_mutex_destroy(&t->print_mutex);
	pthread_mutex_destroy(&t->death_mutex);
	pthread_mutex_destroy(&t->fork_lock);
}
