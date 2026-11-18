/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burn.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:12:10 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 19:01:43 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	print_burnout(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->config->log_mutex);
	time = get_time_ms() - coder->config->start_time;
	printf("%ld %d burned out\n", time, coder->id);
	pthread_mutex_unlock(&coder->config->log_mutex);

}

static int	is_burned(t_coder *coder)
{
	long	last;

	pthread_mutex_lock(&coder->config->state_mutex);
	last = coder->last_compile;
	pthread_mutex_unlock(&coder->config->state_mutex);
	return (get_time_ms() - last > coder->config->time_to_burnout);
}

static void	set_stop(t_config *config)
{
	pthread_mutex_lock(&config->state_mutex);
	config->stop = 1;
	pthread_mutex_unlock(&config->state_mutex);
}

static int	all_done(t_coder *coders, t_config *config)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	while (i < config->number_of_coders)
	{
		pthread_mutex_lock(&config->state_mutex);
		if (coders[i].compiles < config->number_of_compiles_required)
			done = 0;
		pthread_mutex_unlock(&config->state_mutex);
		i++;
	}
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_coder		*coders;
	t_config	*config;
	int			i;

	coders = (t_coder *)arg;
	config = coders[0].config;
	while (1)
	{
		i = 0;
		while (i < config->number_of_coders)
		{
			if (is_burned(&coders[i]))
			{
				print_burnout(&coders[i]);
				return (NULL);
			}
			i++;
		}
		if (all_done(coders, config))
			return (set_stop(config), NULL);
		usleep(500);
	}
}