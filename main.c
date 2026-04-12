#include "philo.h"

static int set_ull_param(unsigned long long *param, const char *val)
{
    int v;

    v = ft_atoi_philo(val);
    if (v <= 0)
        return (1);
    *param = (unsigned long long)v;
    return (0);
}

int init_forks(t_params *t)
{
    int i;

    t->forks = malloc(sizeof(pthread_mutex_t) * t->n);
    if (!t->forks)
        return (printf("malloc failed\n"), 1);
    i = 0;
    while (i < t->n)
    {
        if (pthread_mutex_init(&t->forks[i], NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&t->forks[i]);
            free(t->forks);
            t->forks = NULL;
            return (printf("mutex init failed\n"), 1);
        }
        i++;
    }
    return (0);
}

int parse_args(int ac, char **av, t_params *t)
{
    if (ac != 5 && ac != 6)
        return (printf("Error: wrong number of arguments\n"), 1);
    t->n = ft_atoi_philo(av[1]);
    if (t->n <= 0)
        return (printf("Error: invalid number of philosophers\n"), 1);
    if (set_ull_param(&t->time_to_die, av[2]))
        return (printf("Error: invalid time to die\n"), 1);
    if (set_ull_param(&t->time_to_eat, av[3]))
        return (printf("Error: invalid time to eat\n"), 1);
    if (set_ull_param(&t->time_to_sleep, av[4]))
        return (printf("Error: invalid time to sleep\n"), 1);
    if (ac == 6)
    {
        t->must_eat = ft_atoi_philo(av[5]);
        if (t->must_eat == -1)
            return (printf("Error: invalid must eat\n"), 1);
    }
    else
        t->must_eat = -1;
    return (0);
}

int init_mutexes(t_params *t)
{
    if (pthread_mutex_init(&t->print_mutex, NULL) != 0)
        return (printf("print mutex init failed\n"), 1);
    if (pthread_mutex_init(&t->death_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&t->print_mutex);
        return (printf("death mutex init failed\n"), 1);
    }
    if (pthread_mutex_init(&t->fork_lock, NULL) != 0)
    {
        pthread_mutex_destroy(&t->print_mutex);
        pthread_mutex_destroy(&t->death_mutex);
        return (printf("fork_lock mutex init failed\n"), 1);
    }
    return (0);
}

void debug_print_init(t_params *t)
{
    int i;

    printf("Number of philosophers: %d\n", t->n);

    i = 0;
    while (i < t->n)
    {
        printf("Philo %d:\n", t->philos[i].id);
        printf("  left fork index: %ld\n",
            t->philos[i].left_fork - t->forks);
        printf("  right fork index: %ld\n",
            t->philos[i].right_fork - t->forks);
        printf("  times eaten: %d\n", t->philos[i].times_eaten);
        printf("  last meal: %llu\n", t->philos[i].last_meal);
        i++;
    }
}

int main(int ac, char **av)
{
    t_params t;

    if (parse_args(ac, av, &t))
        return (1);
    t.someone_died = false;
    if (init_forks(&t))
        return (1);
    if (init_mutexes(&t))
        return (1);
    if (init_philos(&t))
        return (1);

    if (start_simulation(&t))
    {
        cleanup(&t);
        return (1);
    }

    cleanup(&t);
    return (0);
}
