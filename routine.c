/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:13:30 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 18:13:31 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

int	is_stopped(t_config *config)
{
    int is_stopped;

    pthread_mutex_lock(&config->state_mutex);
    is_stopped = config->stop;
    pthread_mutex_unlock(&config->state_mutex);
    return (is_stopped);
}

void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 1)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
    pthread_mutex_lock(&first->mutex);
    print_log(coder, "has taken a dongle");
    pthread_mutex_lock(&second->mutex);
    print_log(coder, "has taken a dongle");

}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (!is_stopped(coder->config))
	{
		take_dongles(coder);
		pthread_mutex_lock(&coder->config->state_mutex);
		coder->last_compile = get_time_ms();
		pthread_mutex_unlock(&coder->config->state_mutex);
		print_log(coder, "is compiling");
		ms_sleep(coder->config->time_to_compile);
		pthread_mutex_lock(&coder->config->state_mutex);
		coder->compiles++;
		pthread_mutex_unlock(&coder->config->state_mutex);
		put_dongles(coder);
		print_log(coder, "is debugging");
		ms_sleep(coder->config->time_to_debug);
		print_log(coder, "is refactoring");
		ms_sleep(coder->config->time_to_refactor);
		i++;
	}
	return (NULL);
}

int	init_dongles(t_config *config)
{
    config->dongles = malloc(sizeof(t_dongle) * config->number_of_coders);
    if (!config->dongles)
	    return (0);
    int i;
    i = 0;
    while (i < config->number_of_coders)
    {
        pthread_mutex_init(&config->dongles[i].mutex, NULL);
        config->dongles[i].taken = 0;
        config->dongles[i].available_at = 0;
        i++;
    }
    return (1);
}

void	put_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}
