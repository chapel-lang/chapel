/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */


#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h> /* isprint() */
#include <signal.h>
#include <execinfo.h>
#include <fcntl.h>
#include <ucontext.h>
#include "utils_user.h"
#include "psm_user.h"
#include "../psm_log.h"

unsigned psm3_dbgmask = __HFI_DEBUG_DEFAULT;
char psm3_mylabel[1024];
static int psm3_myrank = -1;
static int psm3_myrank_count = -1;
static int psm3_mylocalrank = -1;
static int psm3_mylocalrank_count = -1;
FILE *psm3_dbgout;
static void psm3_init_mylabel(void) __attribute__ ((constructor));
static void psm3_init_dbgfile(void) __attribute__ ((constructor));
static void psm3_init_backtrace(void) __attribute__ ((constructor));
static void psm3_fini_backtrace(void) __attribute__ ((destructor));
static struct sigaction SIGSEGV_old_act;
static struct sigaction SIGBUS_old_act;
static struct sigaction SIGILL_old_act;
static struct sigaction SIGABRT_old_act;
static struct sigaction SIGINT_old_act;
static struct sigaction SIGTERM_old_act;
#ifdef PSM3_BRAKE_DEBUG
static void hfi_brake_debug(void) __attribute__ ((constructor));

/*
  How to use hfi_break_debug code:

  1. Build psm with PSM3_BRAKE_DEBUG set in the environment.
  2. Create a script for your test case (e.g. mpistress?).  In the script
     make sure to choose a PSM3 brake file that corresponds to a network
     file system that is common to all hosts where you will run your code.
     Also, in the script, make sure to propagate the "PSM3_BRAKE_FILE_NAME"
     env var to all hosts.
     Note: this variable can only be set in the environment, it cannot be
     set in /etc/psm3.conf
  3. Bring up 3 putty sessions to one of the hosts that your script uses.
  4. In putty session number 1, touch the PSM3_BRAKE_FILE and sync.
  5. In putty session number 1, start the script.   You should see messages
     of the form:
-bash-4.2$ ./mpistress.0304.sc
<hostname>:5716 remove the file: "/nfs/user/PSM3_BRAKE"  to continue
<hostname>:5717 remove the file: "/nfs/user/PSM3_BRAKE"  to continue
<hostname>:3456 remove the file: "/nfs/user/PSM3_BRAKE"  to continue
<hostname>:3457 remove the file: "/nfs/user/PSM3_BRAKE"  to continue

     Note that the hostname and process id are shown for all of the processes that are started
     by your script.
  6. In putty session 2, bring up gdb, and debug the program that is referenced in your script.
     For example: /usr/mpi/gcc/openmpi-1.10.2-ofi/tests/intel/mpi_stress
  7. In putty session 2 / gdb, attach to one of the processes that is shown in putty session 1.
  8. Note, at this point, you have only one gdb session.  I leave it as an exercise to the reader to
     determine how to bring up multiple gdb sessions.
  9. In putty session 3, rm the PSM3_BRAKE_FILE.
 10. You are now debugging a live session of psm.
 */

static void hfi_brake_debug(void)
{
	struct stat buff;
	char hostname[80];
	// can't use psm3_env_get since called in a constructor before psm3_init
	// so /etc/psm3.conf can't control this setting
	const char *hfi_brake_file_name = getenv("PSM3_BRAKE_FILE_NAME");
	gethostname(hostname, 80);
	hostname[sizeof(hostname) - 1] = '\0';

	if (!hfi_brake_file_name)
		hfi_brake_file_name = "/tmp/PSM3_BRAKE_FILE";
	printf("%s:%d remove the file: \"%s\"  to continue\n",hostname,getpid(),hfi_brake_file_name);
	while (0 == stat(hfi_brake_file_name, &buff))
	{
		printf("%s:pid%d remove the file: \"%s\"  to continue\n",hostname,getpid(),hfi_brake_file_name);
		sleep(10);
	}
	printf("%s:pid%d continuing.\n",hostname,getpid());
}
#endif

static void psm3_init_mylabel(void)
{
	char hostname[80];
	char *e;
	/* By default, try to come up with a decent default label, it will be
	 * overridden later.  Try getting rank, if that's not available revert to
	 * pid. */
	gethostname(hostname, 80);
	psm3_mylabel[0] = '\0';
	hostname[sizeof(hostname) - 1] = '\0';

#if 0
	/* DEBUG: Used to selectively test possible NIC selection,
	 * shared context and shm-only settings */
	unsetenv("PSC_MPI_NODE_RANK");
	unsetenv("PSC_MPI_PPN");
	unsetenv("MPI_LOCALRANKID");
	unsetenv("MPI_LOCALRANKS");
#endif

	if ((((e = getenv("PMI_SIZE")) && *e))	// MPICH & IMPI
	    || (((e = getenv("OMPI_COMM_WORLD_SIZE")) && *e)) // OMPI
	    || (((e = getenv("MPI_NRANKS")) && *e)) // Platform MPI
	    || (((e = getenv("MPIRUN_NPROCS")) && *e)) // older MPICH
	    // N/A || (((e = getenv("PSC_MPI_TBD")) && *e)) // pathscale MPI
	    || (((e = getenv("WORLD_SIZE")) && *e)) // pyTorch torchrun
	    || (((e = getenv("SLURM_NTASKS")) && *e)) // SLURM
	    || (((e = getenv("SLURM_NPROCS")) && *e)) // older SLURM
	    || (((e = getenv("CCL_LOCAL_SIZE")) && *e)) // oneCCL 1 node w/o launcher
	) {
		char *ep;
		unsigned long val;
		val = strtoul(e, &ep, 10);
		if (ep != e) /* valid conversion */
			psm3_myrank_count = val;
	}

	if ((((e = getenv("MPI_LOCALRANKID")) && *e))	// MPICH and IMPI
	    || (((e = getenv("OMPI_COMM_WORLD_LOCAL_RANK")) && *e)) // OMPI
	    || (((e = getenv("MPI_LOCALRANKID")) && *e)) // Platform MPI
	    // N/A | (((e = getenv("MPIRUN_TBD")) && *e)) // older MPICH
	    || (((e = getenv("PSC_MPI_NODE_RANK")) && *e)) // pathscale MPI
	    || (((e = getenv("LOCAL_RANK")) && *e)) // pyTorch torchrun
	    || (((e = getenv("SLURM_LOCALID")) && *e)) // SLURM
	    || (((e = getenv("CCL_LOCAL_RANK")) && *e)) // oneCCL 1 node w/o launcher
	) {
		char *ep;
		unsigned long val;
		val = strtoul(e, &ep, 10);
		if (ep != e) /* valid conversion */
			psm3_mylocalrank = val;
	}

	if ((((e = getenv("MPI_LOCALNRANKS")) && *e))	// MPICH and IMPI
	    || (((e = getenv("OMPI_COMM_WORLD_LOCAL_SIZE")) && *e)) // OMPI
	    || (((e = getenv("MPI_LOCALNRANKS")) && *e)) // Platform MPI
	    // N/A || (((e = getenv("MPIRUN_TBD")) && *e)) // older MPICH
	    || (((e = getenv("PSC_MPI_PPN")) && *e)) // pathscale MPI
	    || (((e = getenv("LOCAL_WORLD_SIZE")) && *e)) // pyTorch torchrun
	    || (((e = getenv("SLURM_NTASKS_PER_NODE")) && *e)) // SLURM
	    || (((e = getenv("CCL_LOCAL_SIZE")) && *e)) // oneCCL 1 node w/o launcher
	) {
		char *ep;
		unsigned long val;
		val = strtoul(e, &ep, 10);
		if (ep != e) /* valid conversion */
			psm3_mylocalrank_count = val;
	}

	if ((((e = getenv("PMI_RANK")) && *e))	// MPICH and *_SIZE
	    || (((e = getenv("OMPI_COMM_WORLD_RANK")) && *e)) // OMPI and *_SIZE
	    || (((e = getenv("MPI_RANKID")) && *e)) // Platform MPI and *_NRANKS
	    || (((e = getenv("MPIRUN_RANK")) && *e)) // older MPICH and *_NPROCS
	    || (((e = getenv("PSC_MPI_RANK")) && *e)) // pathscale MPI
	    || (((e = getenv("RANK")) && *e)) // pyTorch torchrun
	    || (((e = getenv("SLURM_TASKID")) && *e)) // SLURM
	    || (((e = getenv("SLURM_PROCID")) && *e)) // SLURM
	    || (((e = getenv("CCL_LOCAL_RANK")) && *e)) // oneCCL 1 node w/o launcher
	) {
		char *ep;
		unsigned long val;
		val = strtoul(e, &ep, 10);
		if (ep != e) {	/* valid conversion */
			snprintf(psm3_mylabel, sizeof(psm3_mylabel),
				"%s:rank%lu", hostname, val);
			psm3_myrank = val;
		}
	}
	if (psm3_mylabel[0] == '\0')
		snprintf(psm3_mylabel, sizeof(psm3_mylabel),
			"%s:pid%u", hostname, getpid());
}

/* FIXME: This signal handler does not conform to the posix standards described
   in 'man 7 signal' due to it calling unsafe functions.

   See 'CALLS UNSAFE FUNCTION' notes below for examples.
 */
static void hfi_sighdlr(int sig, siginfo_t *p1, void *ucv)
{
	/* we make these static to try and avoid issues caused
	   by stack overflow that might have gotten us here. */
	static void *backaddr[128];	/* avoid stack usage */
	static char buf[150], hname[64], fname[128];
	static int i, j, fd, id;
	extern char *__progname;
	PSM2_LOG_DECLARE_BT_BUFFER();

	/* CALLS UNSAFE FUNCTION when PSM_LOG is defined. */
	PSM2_LOG_BT(100,__FUNCTION__);
	/* If this is a SIGINT do not display backtrace. Just invoke exit
	   handlers */
	if ((sig == SIGINT) || (sig == SIGTERM))
		/* CALLS UNSAFE FUNCTION (exit) */
		exit(1);

	/* CALLS UNSAFE FUNCTION (snprintf) */
	id = snprintf(buf, sizeof(buf),
		      "\n%.60s:pid%u terminated with signal %d", __progname,
		      getpid(), sig);
	if (ucv) {
		static ucontext_t *uc;
		uc = (ucontext_t *) ucv;
		id += snprintf(buf + id, sizeof(buf) - id, " at PC=%lx SP=%lx",
#if defined(__x86_64__)
			       (unsigned long)uc->uc_mcontext.gregs[REG_RIP],
			       (unsigned long)uc->uc_mcontext.gregs[REG_RSP]);
#elif defined(__i386__)
			       (unsigned long)uc->uc_mcontext.gregs[REG_EIP],
			       (unsigned long)uc->uc_mcontext.gregs[REG_ESP]);
#else
			       0ul, 0ul);
#warning No stack pointer or instruction pointer for this arch
#endif
	}
	id += snprintf(buf + id, sizeof(buf) - id, ". Backtrace:\n");
	/* CALLS UNSAFE FUNCTION (fprintf) */
	fprintf(stderr, "%.*s", id, buf);

	i = backtrace(backaddr, sizeof(backaddr) / sizeof(backaddr[0]));
	if (i > 2)		/* skip ourselves and backtrace */
		j = 2, i -= j;
	else
		j = 0;

	backtrace_symbols_fd(backaddr + j, i, 2);
	(void)fsync(2);

	/* Try to write it to a file as well, in case the rest doesn't make it
	   out. Do it second, in case we get a second failure (more likely).
	   We might eventually want to print some more of the registers to the
	   btr file, to aid debugging, but not for now.  Truncate the program
	   name if overly long, so we always get pid and (at least part of)
	   hostname. */
	/* CALLS UNSAFE FUNCTION (gethostname) */
	(void)gethostname(hname, sizeof(hname));
	hname[sizeof(hname) - 1] = '\0';
	snprintf(fname, sizeof(fname), "%s.80s-%u,%.32s.btr", __progname,
		 getpid(), hname);
	if ((fd = open(fname, O_CREAT | O_WRONLY, 0644)) >= 0) {
		/* CALLS UNSAFE FUNCTION (fdopen) */
		FILE *fp = fdopen(fd, "w");
		if (fp)
			fprintf(fp, "%.*s", id, buf);
		backtrace_symbols_fd(backaddr + j, i, fd);
		if (fp)
			/* CALLS UNSAFE FUNCTION (fclose) */
			fclose(fp);
	}
	switch (sig){
        case SIGSEGV:
                (*SIGSEGV_old_act.sa_sigaction)(sig,p1,ucv);
                break;
        case SIGBUS:
                (*SIGBUS_old_act.sa_sigaction)(sig,p1,ucv);
                break;
        case SIGILL:
                (*SIGILL_old_act.sa_sigaction)(sig,p1,ucv);
                break;
        case SIGABRT:
                (*SIGABRT_old_act.sa_sigaction)(sig,p1,ucv);
                break;
        default:
                break;
        }
	exit(1);		/* not _exit(), want atexit handlers to get run */
}

/* We do this as a constructor so any user program that sets signal handlers
   for these will override our settings, but we still get backtraces if they
   don't.
*/
static void psm3_init_backtrace(void)
{
	/* we need to track memory corruption */
	static struct sigaction act;	/* easier than memset */
	act.sa_sigaction = hfi_sighdlr;
	act.sa_flags = SA_SIGINFO;

	// since this is called in a constructor, prior to psm3_init
	// we must use getenv and /etc/psm3.conf can't control this setting
	if (getenv("PSM3_BACKTRACE")) {
		/* permanent, although probably
		   undocumented way to disable backtraces. */
		(void)sigaction(SIGSEGV, &act, &SIGSEGV_old_act);
		(void)sigaction(SIGBUS, &act, &SIGBUS_old_act);
		(void)sigaction(SIGILL, &act, &SIGILL_old_act);
		(void)sigaction(SIGABRT, &act, &SIGABRT_old_act);
		(void)sigaction(SIGINT, &act, &SIGINT_old_act);
		(void)sigaction(SIGTERM, &act, &SIGTERM_old_act);
	}
}

/* Retrun env if valid, else return NULL */ 
static char *check_dbgfile_env(char *env) {
	char *c;
	if (!env) return NULL;
	for (c = env; *c; c++) {
		if (!isprint(*c)) return NULL;
	}
	return env;
}

/* if PSM3_DEBUG_FILENAME is set in the environment, then all the
   debug prints (not info and error) will go to that file.
   %h is expanded to the hostname, and %p to the pid, if present. */
static void psm3_init_dbgfile(void)
{
	// since this is called in a constructor, prior to psm3_init,
	// we must use getenv and /etc/psm3.conf can't control this setting
	char *fname = getenv("PSM3_DEBUG_FILENAME");
	char *fname1, *fname2; /* for dups */
	char *dname, *bname, *exph, *expp, tbuf[1024], rbuf[PATH_MAX], fnbuf[PATH_MAX];
	FILE *newf;

	if (!fname) {
		psm3_dbgout = stdout;
		return;
	} else if ((fname = check_dbgfile_env(fname)) == NULL) {
		_HFI_ERROR
		    ("Invalid format for debug filename, using stdout: %s\n",
		     "non-printable charaters in string");
		psm3_dbgout = stdout;
		return;
	}
	exph = strstr(fname, "%h");	/* hostname */
	expp = strstr(fname, "%p");	/* pid */
	if (exph || expp) {
		int baselen;
		char hname[256], pid[12];
		if (exph) {
			*hname = hname[sizeof(hname) - 1] = 0;
			gethostname(hname, sizeof(hname) - 1);
			if (!*hname)
				strcpy(hname, "[unknown]");
		}
		if (expp)
			snprintf(pid, sizeof(pid), "%d", getpid());
		if (exph && expp) {
			if (exph < expp) {
				baselen = exph - fname;
				snprintf(tbuf, sizeof(tbuf), "%.*s%s%.*s%s%s",
					 baselen, fname, hname,
					 (int)(expp - (exph + 2)), exph + 2,
					 pid, expp + 2);
			} else {
				baselen = expp - fname;
				snprintf(tbuf, sizeof(tbuf), "%.*s%s%.*s%s%s",
					 baselen, fname, pid,
					 (int)(exph - (expp + 2)), expp + 2,
					 hname, exph + 2);
			}
		} else if (exph) {
			baselen = exph - fname;
			snprintf(tbuf, sizeof(tbuf), "%.*s%s%s",
				 baselen, fname, hname, exph + 2);
		} else {
			baselen = expp - fname;
			snprintf(tbuf, sizeof(tbuf), "%.*s%s%s",
				 baselen, fname, pid, expp + 2);
		}
		fname = tbuf;
	}
	fname1 = psmi_strdup(NULL, fname);
	fname2 = psmi_strdup(NULL, fname);
	bname = basename(fname1);
	dname = realpath(dirname(fname2), rbuf);
	if (!dname) {
		_HFI_ERROR
		    ("Unable to resolve directory \"%s\" for debug output, using stdout: %s\n",
		     dirname(fname), strerror(errno));
		psm3_dbgout = stdout;
		psmi_free(fname1);
		psmi_free(fname2);
		return;
	}
	snprintf(fnbuf, sizeof(fnbuf), "%s/%s",
		dname, bname);
	newf = fopen(fnbuf, "a");
	if (!newf) {
		_HFI_ERROR
		    ("Unable to open \"%s\" for debug output, using stdout: %s\n",
		     fnbuf, strerror(errno));
		psm3_dbgout = stdout;
	} else {
		psm3_dbgout = newf;
		setlinebuf(psm3_dbgout);
	}
	psmi_free(fname1);
	psmi_free(fname2);
}

void psm3_set_mylabel(char *label)
{
	strncpy(psm3_mylabel, label, sizeof(psm3_mylabel));
	psm3_mylabel[sizeof(psm3_mylabel)-1] = '\0';
}

char *psm3_get_mylabel()
{
	return psm3_mylabel;
}

int psm3_get_myrank()
{
	return psm3_myrank;
}

int psm3_get_myrank_count()
{
	return psm3_myrank_count;
}

int psm3_get_mylocalrank()
{
	return psm3_mylocalrank;
}

int psm3_get_mylocalrank_count()
{
	return psm3_mylocalrank_count;
}

static void psm3_fini_backtrace(void)
{
  if (getenv("PSM3_BACKTRACE")) {
    (void)sigaction(SIGSEGV, &SIGSEGV_old_act, NULL);
    (void)sigaction(SIGBUS,  &SIGBUS_old_act, NULL);
    (void)sigaction(SIGILL,  &SIGILL_old_act, NULL);
    (void)sigaction(SIGABRT, &SIGABRT_old_act, NULL);
    (void)sigaction(SIGINT,  &SIGINT_old_act, NULL);
    (void)sigaction(SIGTERM, &SIGTERM_old_act, NULL);
  }
}

void psm3_dump_buf(uint8_t *buf, uint32_t len)
{
	int i, j, print_len;
	char tmp[1080] = {}; // max length is 1024 + 7 + 16*3 = 1078
	for (i=0; i<len; i += 16 ) {
		print_len = snprintf(tmp, sizeof(tmp), "%s: 0x%04x:", psm3_mylabel, i);
		for (j=0; j<16 && i+j < len && print_len < sizeof(tmp) - 1; j++)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, " %02x", (unsigned)buf[i+j]);

		fprintf(psm3_dbgout, "%s\n", tmp);
		fflush(psm3_dbgout);
	}
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
void psm3_dump_gpu_buf(uint8_t *buf, uint32_t len)
{
	int i, j, print_len;
	uint8_t hbuf[1024];
	char tmp[1080] = {};
	for (i=0; i<len; i += 16 ) {
		print_len = snprintf(tmp, sizeof(tmp), "%s: 0x%04x:", psm3_mylabel, i);
		if (0 == i % 1024)
			PSM3_GPU_MEMCPY_DTOH(hbuf, buf,
                                                min(len-i, 1024));
		for (j=0; j<16 && i+j < len && print_len < sizeof(tmp) - 1; j++)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, " %02x", (unsigned)hbuf[i%1024+j]);
		fprintf(psm3_dbgout, "%s\n", tmp);
		fflush(psm3_dbgout);
	}
}
#endif
