/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011 William Braik, Yann Courtois, Jean-Marie Couteyen, Anthony
 * Roy
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <starpu_config.h>

#ifdef STARPU_HAVE_WINDOWS
#  include <w32api.h>
#  define WINVER WindowsXP
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netdb.h>
#endif

#include <top/starpu_top_core.h>
#include <top/starpu_top_connection.h>
#include <top/starpu_top_message_queue.h>
#include <starpu_top.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

const char *STARPU_TOP_PORT = "2011";
const int STARPU_TOP_BUFFER_SIZE=1024;

extern struct _starpu_top_message_queue*  _starpu_top_mt;

//client socket after fopen
FILE* starpu_top_socket_fd_read;
FILE* starpu_top_socket_fd_write;
//client socket (file descriptor)
int starpu_top_socket_fd;

static
void * message_from_ui(void * p)
{
	(void) p;
	char str[STARPU_TOP_BUFFER_SIZE];
	while(1)
	{
		char * check=fgets (str, STARPU_TOP_BUFFER_SIZE, starpu_top_socket_fd_read);

		printf("Message from UI : %s",str);
		if (check)
		{
			_starpu_top_process_input_message(str);
		}
		else
		{
			fprintf(stderr,"Connection dropped\n");
			//unlocking StarPU.
			_starpu_top_process_input_message("GO\n");
			_starpu_top_process_input_message("DEBUG;OFF\n");
			_starpu_top_process_input_message("STEP\n");
			return NULL;
		}
	}
}

static
void * message_to_ui(void * p)
{
	(void) p;
	while(1)
	{
		char* message = _starpu_top_message_remove(_starpu_top_mt);
		int len=strlen(message);
		int check=fwrite(message, sizeof(char), len, starpu_top_socket_fd_write);
		int check2=fflush(starpu_top_socket_fd_write);
		free(message);
		if (check!=len || check2==EOF )
		{
			fprintf(stderr,"Connection dropped : message no longer send\n");
			while(1)
			{
				message=_starpu_top_message_remove(_starpu_top_mt);
				free(message);
			}
		}
	}
	return NULL;
}

void _starpu_top_communications_threads_launcher()
{
	pthread_t from_ui;
	pthread_t to_ui;
	pthread_attr_t threads_attr;


	//Connection to UI & Socket Initilization
	printf("%s:%d Connection to UI initilization\n",__FILE__, __LINE__);
	struct sockaddr_storage from;
	struct addrinfo req, *ans;
	int code;
	req.ai_flags = AI_PASSIVE;
	req.ai_family = PF_UNSPEC;
	req.ai_socktype = SOCK_STREAM;
	req.ai_protocol = 0;

	if ((code = getaddrinfo(NULL, STARPU_TOP_PORT, &req, &ans)) != 0)
	{
		fprintf(stderr, " getaddrinfo failed %d\n", code);
		exit(EXIT_FAILURE);
   	}
  	int sock=socket(ans->ai_family, ans->ai_socktype, ans->ai_protocol);
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	if (bind(sock, ans->ai_addr, ans->ai_addrlen) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	listen(sock, 2);

	socklen_t len = sizeof(from);

   	if ((starpu_top_socket_fd=accept(sock, (struct sockaddr *) &from, &len)) ==-1)
	{
		fprintf(stderr, "accept error\n");
		perror("accept");
		exit(EXIT_FAILURE);
	}

	if ( (starpu_top_socket_fd_read=fdopen(starpu_top_socket_fd, "r")) == NULL)
	{
		perror("fdopen");
		exit(EXIT_FAILURE);
	}

	starpu_top_socket_fd=dup(starpu_top_socket_fd);

	if ((starpu_top_socket_fd_write=fdopen(starpu_top_socket_fd, "w")) == NULL)
	{
		perror("fdopen");
		exit(EXIT_FAILURE);
	}

	close(sock);

	//Threads creation
	fprintf(stderr,"Threads Creation\n");
	pthread_attr_init(&threads_attr);
	pthread_attr_setdetachstate(&threads_attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&from_ui, &threads_attr, message_from_ui, NULL);
	pthread_create(&to_ui, &threads_attr, message_to_ui, NULL);
}

