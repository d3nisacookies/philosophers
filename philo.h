#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

typedef struct s_params	t_params;

typedef struct s_philo
{
	int					id;
	int					times_eaten;
	int					pick_right_first;
	unsigned long long	last_meal;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_t			thread;
	t_params			*params;
}	t_philo;

typedef struct s_params
{
	int					n;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	int					must_eat;
	unsigned long long	start_time;
	bool				someone_died;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	t_philo				*philos;
}	t_params;

/* utils.c */
void				ft_bzero(void *s, size_t n);
unsigned long long	get_time(void);
void				ft_sleep(unsigned long long time, t_params *params);
void				print_state(t_philo *p, char *msg);
int					ft_atoi_philo(const char *str);
int					should_philo_exit(t_philo *p);

/* sim_utils.c */
int					simulation_end(t_philo *p);
void				eat(t_philo *p);
int					check_philo_death(t_params *params, int i);
int					check_all_fed(t_params *params);

/* philos.c */
int					init_philos(t_params *t);
void				take_forks(t_philo *p);
void				put_forks(t_philo *p);
void				single_philo_eat(t_philo *p);

/* simulation.c */
void				*routine(void *arg);
void				*monitor(void *arg);
int					start_simulation(t_params *t);

/* main.c */
int					init_forks(t_params *t);
int					parse_args(int ac, char **av, t_params *t);
int					init_mutexes(t_params *t);

/* cleanup.c */
void				destroy_forks(t_params *t);
void				destroy_philos(t_params *t);
void				cleanup(t_params *t);

#endif