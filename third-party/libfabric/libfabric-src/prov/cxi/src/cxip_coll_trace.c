/*
 * Copyright (c) 2021-2024 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */

/**
 * @brief TRACE function for producing runtime debugging logs
 *
 * This creates log files on each running node of a multinode job, which
 * contain trace information produced on that node. SLURM nodes inherit
 * stdout/stderr from the launch node.
 *
 * ENABLE_DEBUG is the libfabric logging flag that enables debug message
 * logging, and is required to use TRACE calls, which produce more output than
 * DEBUG, and are specific to collectives. If ENABLE_DEBUG is unset, or
 * evaluates to false at compile time, CXIP_COLL_TRACE is a syntactically robust
 * NOOP which results in no code being generated, ensuring that these trace
 * calls do not affect performance in production. ENABLE_DEBUG is normally true
 * for development builds, and false for production builds.
 *
 * This module is initialized on the first call to cxip_ep_enable(), which
 * initializes its ep.coll structure using cxip_coll_enable(). Multiple
 * endpoints can be enabled, and each associated ep.coll object will be
 * initialized, but this module will only be initialized once, and is not
 * dependent on the ep or the ep.coll object. This module preserves a count
 * of initializations, and is closed when the last ep is closed.
 *
 * Unlike normal libfabric output, the CXIP_COLL_TRACE() output opens a trace
 * file on the node it is running on, with a unique name that identifies the
 * node so that a shared file system can be used among the nodes. Since the
 * collectives code is running in parallel on multiple nodes, mixing the output
 * through a single file descriptor (like stdout) scrambles the traces, and
 * makes it difficult to follow the flow. Using separate output files keeps the
 * sequences intact without needing to post-process the data.
 *
 * In addition, this allows character buffered output, which generally runs
 * faster, but can interleave lines in ways that makes post-processing
 * impossible. The normal solution is to line-buffer the output, but this
 * slows operations considerably and can mask timing-related problems.
 *
 * The output-selection flags are of the form CXIP_TRC_COLL_*, and are specified
 * in the CXIP_COLL_TRACE() macros to differentiate the kind of output desired.
 * These are activated by defining environment variables of the same name.
 *
 * To use this feature, create a .bashrc file in the runtime directory of each
 * node in the job (or use a shared FS for all nodes in the job), containing one
 * or more of the CXIP_TRC_* environment variables.
 *
 * Note that the environment settings for each node in a multi-node job can be
 * set differently to capture different traces.
 *
 * Example:
 * export CXIP_TRC_PATHNAME = "/mypath/myfile"
 * export CXIP_TRC_LINEBUF=1
 * export CXIP_TRC_COLL_JOIN=1
 * export CXIP_TRC_COLL_PKT=1
 *
 * All instances of the following in the code will result in output:
 * CXIP_COLL_TRACE(CXIP_TRC_COLL_JOIN, fmt, ...);
 * CXIP_COLL_TRACE(CXIP_TRC_COLL_PKT, fmt, ...);
 * All instances of other CXIP_TRC_* values will be silent.
 *
 * Environment variables used in setup:
 * CXIP_TRC_PATHNAME defines the output path name, which defaults to "trace".
 * CXIP_TRC_LINEBUF sets or clears line buffering out output, and defaults to 0.
 * CXIP_TRC_APPEND sets or clears open append mode, and defaults to 0.
 *
 * CXIP_TRC_APPEND is needed for NETSIM tests under Criterion, since each
 * test is run in a separate process and closes all files at completion of
 * each test. If CXIP_TRC_APPEND is not set, you will see only the tracie of
 * the last test run.
 *
 * Specifying PMI_RANK as a rank value will apply a prefix to the trace lines
 * that identifies the rank of the trace. Note that this is normally exported
 * by the SLURM environment, or the multinode test framework.
 *
 * Specifying PMI_SIZE will expand the prefix to show the number of ranks.
 * Note that this is normally exported by the SLURM environment, or the
 * multinode test framework.
 *
 * cxip_coll_trace_flush() forces all output be flushed AND written to disk, but
 * leaves the file open for more writing.
 *
 * cxip_coll_trace_close() flushes all output and closes the file.
 *
 * cxip_coll_trace() is used to generate trace messages, and is normally called
 * through the CXIP_COLL_TRACE() macro.
 */
#include "config.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cxip.h"

bool cxip_coll_trace_initialized;
bool cxip_coll_trace_muted;
bool cxip_coll_trace_append;
bool cxip_coll_trace_linebuf;
int cxip_coll_trace_rank;
int cxip_coll_trace_numranks;
char *cxip_coll_trace_pathname;
FILE *cxip_coll_trace_fid;
uint64_t cxip_coll_trace_mask;

/* Get environment variable as string representation of int */
static int getenv_int(const char *name)
{
	char *env;
	int value;

	value = -1;
	env = getenv(name);
	if (env)
		sscanf(env, "%d", &value);
	return value;
}

void cxip_coll_trace_init(void)
{
	const char *fpath;
	int ret;

	/* can only initialize once */
	if (cxip_coll_trace_initialized)
		return;

	cxip_coll_trace_mask = 0L;
	cxip_coll_trace_fid = NULL;
	cxip_coll_trace_pathname = NULL;

	cxip_coll_trace_append = !!getenv("CXIP_TRC_APPEND");
	cxip_coll_trace_linebuf = !!getenv("CXIP_TRC_LINEBUF");
	cxip_coll_trace_rank = getenv_int("PMI_RANK");
	cxip_coll_trace_numranks = getenv_int("PMI_SIZE");
	fpath = getenv("CXIP_TRC_PATHNAME");

	/* set bits in cxip_coll_trace_mask */
	if (getenv("CXIP_TRC_CTRL"))
		cxip_coll_trace_set(CXIP_TRC_CTRL);
	if (getenv("CXIP_TRC_ZBCOLL"))
		cxip_coll_trace_set(CXIP_TRC_ZBCOLL);
	if (getenv("CXIP_TRC_COLL_CURL"))
		cxip_coll_trace_set(CXIP_TRC_COLL_CURL);
	if (getenv("CXIP_TRC_COLL_PKT"))
		cxip_coll_trace_set(CXIP_TRC_COLL_PKT);
	if (getenv("CXIP_TRC_COLL_JOIN"))
		cxip_coll_trace_set(CXIP_TRC_COLL_JOIN);
	if (getenv("CXIP_TRC_COLL_DEBUG"))
		cxip_coll_trace_set(CXIP_TRC_COLL_DEBUG);
	if (getenv("CXIP_TRC_TEST_CODE"))
		cxip_coll_trace_set(CXIP_TRC_TEST_CODE);

	/* if no trace masks set, do nothing */
	if (!cxip_coll_trace_mask)
		return;

	if (!fpath || !*fpath)
		fpath = "trace";
	ret = asprintf(&cxip_coll_trace_pathname, "%s%d", fpath, cxip_coll_trace_rank);
	if (ret <= 0) {
		fprintf(stderr, "asprintf() failed = %s\n", strerror(ret));
		return;
	}
	cxip_coll_trace_fid =
		fopen(cxip_coll_trace_pathname, cxip_coll_trace_append ? "a" : "w");
	if (!cxip_coll_trace_fid) {
		fprintf(stderr, "open('%s') failed: %s\n", cxip_coll_trace_pathname,
			strerror(errno));
		free(cxip_coll_trace_pathname);
		cxip_coll_trace_pathname = NULL;
		return;
	}
	if (cxip_coll_trace_linebuf && cxip_coll_trace_fid)
		setlinebuf(cxip_coll_trace_fid);

	cxip_coll_trace_initialized = true;
}

void cxip_coll_trace_flush(void)
{
	if (cxip_coll_trace_fid) {
		fflush(cxip_coll_trace_fid);
		fsync(fileno(cxip_coll_trace_fid));
	}
}

void cxip_coll_trace_close(void)
{
	if (cxip_coll_trace_fid) {
		cxip_coll_trace_flush();
		fclose(cxip_coll_trace_fid);
		cxip_coll_trace_fid = NULL;
		if (cxip_coll_trace_pathname) {
			free(cxip_coll_trace_pathname);
			cxip_coll_trace_pathname = NULL;
		}
	}
	cxip_coll_trace_initialized = false;
}

int cxip_coll_trace_attr cxip_coll_trace(const char *fmt, ...)
{
	va_list args;
	char *str;
	int len;

	va_start(args, fmt);
	len = vasprintf(&str, fmt, args);
	va_end(args);
	if (len >= 0) {
		len = fprintf(cxip_coll_trace_fid, "[%2d|%2d] %s", cxip_coll_trace_rank,
			      cxip_coll_trace_numranks, str);
		free(str);
	}
	return len;
}
