#include "philo.h"

int simulation_end(t_philo *p)
{
    int end;
    
    pthread_mutex_lock(&p->params->death_mutex);
    end = p->params->someone_died;
    pthread_mutex_unlock(&p->params->death_mutex);
    return (end);
}