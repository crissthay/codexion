/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:13:24 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 18:56:30 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int check_arg(char *arg, int index)
{
	if (index == 8)
	{
		if (strcmp(arg, "fifo") != 0 && strcmp(arg, "edf") != 0)
		{
			write(2, "ERROR - Arg 8 just can be fifo/edf\n",
				sizeof("ERROR - Arg 8 just can be fifo/edf\n") - 1);
			return (0);
		}
		return (1);
	}
	if (is_valid_number(arg) == 0 || is_max(arg) == 0)
	{
		write(2, "ERROR - Invalid number\n",
			sizeof("ERROR - Invalid number\n") - 1);
		return (0);
	}
	if (index != 7 && atoi(arg) == 0)
	{
		write(2, "ERROR - Args 1 to 6 cannot be zero\n",
			sizeof("ERROR - Args 1 to 6 cannot be zero\n") - 1);
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	int         i;
	t_config    config;
	t_coder *coders;
	pthread_t	monitor;


	if (argc != 9)
	{
		write(2, "ERROR - Arguments missing\n",
			sizeof("ERROR - Arguments missing\n") - 1);
		return (1);
	}

	i = 1;
	while (i < argc)
	{
		if (check_arg(argv[i], i) == 0)
			return (1);
		i++;
	}
	init_config(&config, argv);
	init_dongles(&config);
	coders = init_coders(&config);
	if (!coders)
		return (1);
	i = 0;
	while (i < config.number_of_coders)
	{
		pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, coders);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < config.number_of_coders)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
	free(coders);
	free(config.dongles);
	free(coders);

	long	a;
	long	b;

	a = get_time_ms();
	ms_sleep(500);
	b = get_time_ms();
	printf("%ld ms\n", b - a);
	return (0);
	//printf("coders: %d\n", config.number_of_coders);
	//printf("cooldown: %d\n", config.dongle_cooldown);
	//printf("scheduler: %d\n", config.scheduler);
	//printf("start: %ld\n", config.start_time);
}
