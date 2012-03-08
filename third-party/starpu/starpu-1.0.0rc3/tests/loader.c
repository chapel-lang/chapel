/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
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

#include <common/config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define  DEFAULT_TIMEOUT       600
#define  AUTOTEST_SKIPPED_TEST 77

static pid_t child_pid = 0;
static int   timeout;

static void launch_gdb(const char *exe)
{
#ifdef STARPU_GDB_PATH
# define CORE_FILE "core"
# define GDB_COMMAND "thread apply all bt full"
	int err;
	pid_t pid;
	struct stat st;
	const char *top_builddir;
	char *gdb;

	err = stat(CORE_FILE, &st);
	if (err != 0)
	{
		fprintf(stderr, "while looking for core file of %s: %s: %m\n",
			exe, CORE_FILE);
		return;
	}

	if (!(st.st_mode & S_IFREG))
	{
		fprintf(stderr, CORE_FILE ": not a regular file\n");
		return;
	}

	top_builddir = getenv("top_builddir");

	pid = fork();
	switch (pid)
	{
	case 0:					  /* kid */
		if (top_builddir != NULL)
		{
			/* Run gdb with Libtool.  */
			gdb = alloca(strlen(top_builddir)
				     + sizeof("libtool") + 1);
			strcpy(gdb, top_builddir);
			strcat(gdb, "/libtool");
			err = execl(gdb, "gdb", "--mode=execute",
				    STARPU_GDB_PATH, "--batch",
				    "-ex", GDB_COMMAND,
				    exe, CORE_FILE, NULL);
		}
		else
		{
			/* Run gdb directly  */
			gdb = STARPU_GDB_PATH;
			err = execl(gdb, "gdb", "--batch", "-ex", GDB_COMMAND,
				    exe, CORE_FILE, NULL);
		}
		if (err != 0)
		{
			fprintf(stderr, "while launching `%s': %m\n", gdb);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
		break;

	case -1:
		fprintf(stderr, "fork: %m\n");
		return;

	default:				  /* parent */
		{
			pid_t who;
			int status;
			who = waitpid(pid, &status, 0);
			if (who != pid)
				fprintf(stderr, "while waiting for gdb "
					"process %d: %m\n", pid);
		}
	}
# undef GDB_COMMAND
# undef CORE_FILE
#endif	/* STARPU_GDB_PATH */
}

static void test_cleaner(int sig)
{
	pid_t child_gid;

	// send signal to all loader family members
	fprintf(stderr, "[error] test has been blocked for %d seconds. Mark it as failed\n", timeout);
	child_gid = getpgid(child_pid);
	kill(-child_gid, SIGKILL);
	exit(EXIT_FAILURE);
}

static void decode(char **src, char *motif, char *value)
{
     if (*src) {
	  char *y = strstr(*src, motif);
	  while (y) {
	       char *neo = malloc((strlen(*src)-strlen(motif)+strlen(value)) * sizeof(char));
	       char *to = neo;

	       to = strncpy(to, *src, strlen(*src)-strlen(y)); to += strlen(*src)-strlen(y);
	       to = strcpy(to, value); to += strlen(value);
	       to = stpcpy(to, y+strlen(motif));

	       *src = strdup(neo);
	       y = strstr(*src, motif);
	  }
     }
}

int main(int argc, char *argv[])
{
	int   child_exit_status;
	char *test_name;
	char *test_args;
	int   status;
	char *launcher;
	char *launcher_args;
	char *top_srcdir;
	struct sigaction sa;

	test_args = NULL;
	timeout = 0;
	test_name = argv[1];

	if (!test_name)
	{
		fprintf(stderr, "[error] Need name of program to start\n");
		exit(EXIT_FAILURE);
	}

	if (strstr(test_name, "spmv/dw_block_spmv"))
	{
		test_args = (char *) malloc(150*sizeof(char));
		sprintf(test_args, "%s/examples/spmv/matrix_market/examples/fidapm05.mtx", STARPU_SRC_DIR);
	}

	/* get launcher program */
	launcher=getenv("STARPU_CHECK_LAUNCHER");
	launcher_args=getenv("STARPU_CHECK_LAUNCHER_ARGS");
	top_srcdir = getenv("top_srcdir");
	decode(&launcher_args, "@top_srcdir@", top_srcdir);

	/* get user-defined iter_max value */
	if (getenv("STARPU_TIMEOUT_ENV"))
		timeout = strtol(getenv("STARPU_TIMEOUT_ENV"), NULL, 10);
	if (timeout <= 0)
		timeout = DEFAULT_TIMEOUT;

	/* set SIGALARM handler */
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = test_cleaner;
	if (-1 == sigaction(SIGALRM, &sa, NULL))
		perror("sigaction");

	child_pid = fork();
	if (child_pid == 0)
	{
		// get a new pgid
		if (setpgid(0, 0) == -1)
		{
			perror("setpgid");
			fprintf(stderr, "[error] setpgid. Mark test as failed\n");
			exit(EXIT_FAILURE);
		}
		if (launcher)
		{
			/* "Launchers" such as Valgrind need to be inserted
			 * after the Libtool-generated wrapper scripts, hence
			 * this special-case.  */
			const char *top_builddir = getenv ("top_builddir");
			if (top_builddir != NULL)
			{
				char *argv[100];
				int i=3;
				char libtool[strlen(top_builddir)
					     + sizeof("libtool") + 1];
				strcpy(libtool, top_builddir);
				strcat(libtool, "/libtool");

				argv[0] = libtool;
				argv[1] = "--mode=execute";
				argv[2] = launcher;
				argv[i] = strtok(launcher_args, " ");
				while (argv[i])
				{
					i++;
					argv[i] = strtok(NULL, " ");
				}
				argv[i] = test_name;
				argv[i+1] = NULL;
				execvp(*argv, argv);
			}
			else
			{
				fprintf(stderr,
					"warning: $top_builddir undefined, "
					"so $STARPU_CHECK_LAUNCHER ignored\n");
				execl(test_name, test_name, test_args, NULL);
			}
		}
		else
			execl(test_name, test_name, test_args, NULL);

		fprintf(stderr, "[error] '%s' failed to exec. test marked as failed\n", test_name);
		exit(EXIT_FAILURE);
	}
	if (child_pid == -1)
	{
		fprintf(stderr, "[error] fork. test marked as failed\n");
		exit(EXIT_FAILURE);
	}

	alarm(timeout);
	if (child_pid == waitpid(child_pid, &child_exit_status, 0))
	{
		if (WIFEXITED(child_exit_status))
		{
			status = WEXITSTATUS(child_exit_status);
			if (status == EXIT_SUCCESS)
			{
				alarm(0);
			}
			else
			{
				if (status != AUTOTEST_SKIPPED_TEST)
					fprintf(stdout, "`%s' exited with return code %d\n",
						test_name, status);
				return status;
			}
		}
		else if (WIFSIGNALED(child_exit_status))
		{
			fprintf(stderr, "[error] `%s' killed with signal %d; test marked as failed\n",
				test_name, WTERMSIG(child_exit_status));
			launch_gdb(test_name);
			return EXIT_FAILURE;
		}
		else
		{
			fprintf(stderr, "[error] `%s' did not terminate normally; test marked as failed\n",
				test_name);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
