/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultis.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:13:32 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 18:57:02 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	init_config(t_config *config, char **argv)
{
	config->stop = 0;
	pthread_mutex_init(&config->state_mutex, NULL);
	config->number_of_coders = atoi(argv[1]);
	config->time_to_burnout = atoi(argv[2]);
	config->time_to_compile = atoi(argv[3]);
	config->time_to_debug = atoi(argv[4]);
	config->time_to_refactor = atoi(argv[5]);
	config->number_of_compiles_required = atoi(argv[6]);
	config->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		config->scheduler = 0;
	else
		config->scheduler = 1;
	config->start_time = get_time_ms();
	pthread_mutex_init(&config->log_mutex, NULL);
}

void	print_log(t_coder *coder, char *msg)
{
	long	time;

	pthread_mutex_lock(&coder->config->log_mutex);
	if (!is_stopped(coder->config))
	{
		time = get_time_ms() - coder->config->start_time;
		printf("%ld %d %s\n", time, coder->id, msg);
	}
	pthread_mutex_unlock(&coder->config->log_mutex);
}

t_coder	*init_coders(t_config *config)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * config->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < config->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].config = config;
		coders[i].compiles = 0;
		coders[i].last_compile = config->start_time;
		coders[i].left = &config->dongles[i];
		coders[i].right = &config->dongles[(i + 1) % config->number_of_coders];
		i++;
	}
	return (coders);
}

void	ms_sleep(long ms)
{
	long	end;

	end = get_time_ms() + ms;
	while (get_time_ms() < end)
		usleep(500);
}
