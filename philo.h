/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaung <akaung@student.42.sg>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 15:40:26 by akaung            #+#    #+#             */
/*   Updated: 2026/04/12 15:51:41 by akaung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_params
{
	int					n;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	int					must_eat;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	bool				someone_died;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		fork_lock;
	unsigned long long	start_time;
	t_philo				*philos;
}						t_params;

typedef struct s_philo
{
	int					id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	unsigned long long	last_meal;
	pthread_mutex_t		meal_mutex;
	int					times_eaten;
	int					pick_right_first;
	t_params			*params;
	pthread_t			thread;
}						t_philo;

unsigned long long		get_time(void);
void					ft_sleep(unsigned long long time, t_params *params);
void					print_state(t_philo *p, char *msg);
int						ft_atoi_philo(const char *str);
int						should_philo_exit(t_philo *p);
int						init_forks(t_params *t);
int						parse_args(int ac, char **av, t_params *t);
int						init_mutexes(t_params *t);
int						init_philos(t_params *t);
void					take_forks(t_philo *p);
void					put_forks(t_philo *p);
void					single_philo_eat(t_philo *p);
void					destroy_forks(t_params *t);
void					destroy_philos(t_params *t);
void					cleanup(t_params *t);
int						start_simulation(t_params *t);
void					*routine(void *arg);
void					*monitor(void *arg);
void					eat(t_philo *p);
int						simulation_end(t_philo *p);
int						check_philo_death(t_params *params, int i);
int						check_all_fed(t_params *params);

#endif