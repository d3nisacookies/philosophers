#include "philo.h"

void *routine(void *arg)
{
    t_philo *p;

    p = (t_philo *)arg;
    if (p->id % 2 == 0)
        usleep(1000);
    while (!simulation_end(p))
    {
        eat(p);
        if (p->params->must_eat > 0)
        {
            pthread_mutex_lock(&p->meal_mutex);
            if (p->times_eaten >= p->params->must_eat)
            {
                pthread_mutex_unlock(&p->meal_mutex);
                break;
            }
            pthread_mutex_unlock(&p->meal_mutex);
        }
        if (simulation_end(p))
            break;
        print_state(p, "is sleeping");
        ft_sleep(p->params->time_to_sleep, p->params);
        if (simulation_end(p))
            break;
        print_state(p, "is thinking");
    }
    return (NULL);
}

void *monitor(void *arg)
{
    t_params *params = (t_params *)arg;
    int i;
    unsigned long long now;

    while (!params->someone_died)
    {
        i = 0;
        while (i < params->n)
        {
            pthread_mutex_lock(&params->philos[i].meal_mutex);
            now = get_time();
            if (now - params->philos[i].last_meal > params->time_to_die)
            {
                pthread_mutex_lock(&params->death_mutex);
                params->someone_died = true;
                pthread_mutex_unlock(&params->death_mutex);
                pthread_mutex_lock(&params->print_mutex);
                printf("%llu %d died\n", now - params->start_time, params->philos[i].id);
                pthread_mutex_unlock(&params->print_mutex);
                pthread_mutex_unlock(&params->philos[i].meal_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&params->philos[i].meal_mutex);
            i++;
        }
        if (params->must_eat > 0)
        {
            i = 0;
            while (i < params->n)
            {
                pthread_mutex_lock(&params->philos[i].meal_mutex);
                if (params->philos[i].times_eaten < params->must_eat)
                {
                    pthread_mutex_unlock(&params->philos[i].meal_mutex);
                    break;
                }
                pthread_mutex_unlock(&params->philos[i].meal_mutex);
                i++;
            }
            if (i == params->n)
                return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}


int start_simulation(t_params *t)
{
    int         i;
    pthread_t   monitor_thread;

    t->start_time = get_time();
    i = 0;
    while (i < t->n)
    {
        t->philos[i].last_meal = t->start_time;
        if (pthread_create(&t->philos[i].thread, NULL, routine, &t->philos[i]) != 0)
        {
            while (--i >= 0)
                pthread_join(t->philos[i].thread, NULL);
            return (printf("thread creation failed\n"), 1);
        }
        i++;
    }
    if (pthread_create(&monitor_thread, NULL, monitor, t) != 0)
        return (printf("monitor thread creation failed\n"), 1);
    pthread_join(monitor_thread, NULL);
    i = 0;
    while (i < t->n)
    {
        pthread_join(t->philos[i].thread, NULL);
        i++;
    }
    return (0);
}