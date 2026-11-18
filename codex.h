/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:12:25 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 18:56:01 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				taken;
	long			available_at;
}	t_dongle;

typedef struct s_config
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	int		scheduler;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
	int				stop;
	t_dongle	*dongles;
	long	start_time;
}	t_config;

typedef struct s_coder
{
	int			id;
	int			compiles;
	long		last_compile;
	pthread_t	thread;
	t_config	*config;
	t_dongle	*left;
	t_dongle	*right;
}	t_coder;

//int
int is_valid_number(char *str);
int is_max(char *str);
int	init_dongles(t_config *config);
int	is_stopped(t_config *config);

//long
long	get_time_ms(void);

//void
void	init_config(t_config *config, char **argv);
void	print_log(t_coder *coder, char *msg);
void	*coder_routine(void *arg);
void	ms_sleep(long ms);
void	put_dongles(t_coder *coder);
void	*monitor_routine(void *arg);


//t
t_coder	*init_coders(t_config *config);


#endif