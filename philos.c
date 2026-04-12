#include "philo.h"

static int init_one_philo(t_params *t, int i)
{
    t->philos[i].id = i + 1;
    t->philos[i].times_eaten = 0;
    t->philos[i].params = t;
    t->philos[i].left_fork = &t->forks[i];
    t->philos[i].right_fork = &t->forks[(i + 1) % t->n];
    t->philos[i].pick_right_first = ((t->philos[i].id % 2) == 0);
    t->philos[i].last_meal = 0ULL;
    if (pthread_mutex_init(&t->philos[i].meal_mutex, NULL) != 0)
        return (1);
    return (0);
}

int init_philos(t_params *t)
{
    int i;

    t->philos = malloc(sizeof(*t->philos) * t->n);
    if (!t->philos)
        return (printf("philo malloc failed\n"), 1);
    i = 0;
    while (i < t->n)
    {
        if (init_one_philo(t, i))
        {
            while (--i >= 0)
                pthread_mutex_destroy(&t->philos[i].meal_mutex);
            free(t->philos);
            t->philos = NULL;
            return (printf("meal mutex init failed\n"), 1);
        }
        i++;
    }
    return (0);
}

static void take_forks(t_philo *p)
{
    pthread_mutex_lock(&p->params->fork_lock);
    pthread_mutex_lock(p->left_fork);
    print_state(p, "has taken a fork");
    pthread_mutex_lock(p->right_fork);
    print_state(p, "has taken a fork");
    pthread_mutex_unlock(&p->params->fork_lock);
}

static void put_forks(t_philo *p)
{
    pthread_mutex_unlock(p->left_fork);
    pthread_mutex_unlock(p->right_fork);
}

static void single_philo_eat(t_philo *p)
{
    pthread_mutex_lock(p->left_fork);
    print_state(p, "has taken a fork");
    print_state(p, "is eating");
    ft_sleep(p->params->time_to_eat, p->params);
    pthread_mutex_unlock(p->left_fork);
}

void eat(t_philo *p)
{
    if (p->params->n == 1)
    {
        single_philo_eat(p);
        return;
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
