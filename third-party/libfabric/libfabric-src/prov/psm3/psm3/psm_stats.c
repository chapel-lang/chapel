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

#include "psm_user.h"
#include "psm_mq_internal.h"
#include <sys/syscall.h>

struct psmi_stats_type {
	STAILQ_ENTRY(psmi_stats_type) next;
	struct psmi_stats_entry *entries;

	int num_entries;
	const char *heading;
	const char *help;
	uint32_t statstype;
	char *id;	// identifier to include in output, typically epid
	void *context;
	char *info;
	pid_t tid;	// thread id, useful for multi-ep
	int help_shown;
};

static STAILQ_HEAD(, psmi_stats_type) psmi_stats =
STAILQ_HEAD_INITIALIZER(psmi_stats);

pthread_spinlock_t psm3_stats_lock;	// protects psmi_stats list
static int perf_stats_initialized;
// stats output
static int print_stats_help;
static char perf_help_file_name[PATH_MAX];
static FILE *perf_help_fd;
static int print_statsmask;
static time_t stats_start;
static char perf_file_name[PATH_MAX];
static FILE *perf_stats_fd;
// stats thread
static int print_stats_freq;
static int print_stats_running;
static pthread_t perf_print_thread;

// any psm3 env variables parsed prior to psm3_stats_initialize get stashed here
#define MAX_SAVED_ENV 40	// only a handful parsed early, more than enough
static char perf_stats_env[MAX_SAVED_ENV][80];
static int perf_stats_env_num = 0;

// initialize early so available to handle env print calls
static void __attribute__ ((constructor)) __psm3_stats_lock_constructor(void)
{
	pthread_spin_init(&psm3_stats_lock, PTHREAD_PROCESS_PRIVATE);
}

// we attempt open only once and only output error once
// this prevents multiple failures and also prevents reopen during finalize
static void psmi_open_stats_fd()
{
	static int attempted_open;

	if (! attempted_open && ! perf_stats_fd) {
		perf_stats_fd = fopen(perf_file_name, "w+");
		if (!perf_stats_fd)
			_HFI_ERROR("Failed to create fd for performance logging: %s: %s\n",
				perf_file_name, strerror(errno));
		attempted_open = 1;
	}
}

#define PSM3_LINE_LEN 80
// output help and as needed wrap at PSM3_LINE_LEN with ident before
// each subsequent line.  Wrap occurs at a space.
// Automatic wrap when hit a newline in help.  A single trailing newline
// is ignored.  Do not use tabs, they are treated like normal characters
// and may result in output lines too long.
// assumes when called indent characters already output on current line
// returns number of lines output
static int
psm3_print_wrapped_help(int indent, const char *help)
{
	int len = strlen(help);
	int i = 0;
	int per_line = PSM3_LINE_LEN-indent;
	int next_len;
	int lines = 0;

	// each loop outputs a section of help up to a '\n' in help[]
	// next_len is length before '\n', so we output help[i to next_len-1]
	for (i=0; i < len; i = next_len+1) {
		const char *p = strchr(&help[i], '\n');
		if (p)
			next_len = p - help;	// newline in help[]
		else
			next_len = len;			// no more newlines, output the rest
		if (i == next_len) {
			// empty line
			fprintf(perf_help_fd, "\n");
			lines++;
			continue;
		}
		// output help[i to next_len-1], wrapping as needed to fit in per_line
		for (; i<next_len;) {
			int j;
			if (i)	// indent each subsequent line of output
				fprintf(perf_help_fd, "%*s", indent, " ");
			if (next_len-i <= per_line) {
				// next section of output fits on line
				fprintf(perf_help_fd, "%-.*s\n", next_len-i, &help[i]);
				lines++;
				break;
			}
			// output up to a space limiting to fit in per_line
			for (j=i+per_line-1; j > i; j--) {
				if (help[j] == ' ') {
					// space closest to end of line
					fprintf(perf_help_fd, "%-.*s\n", j-i, &help[i]);
					lines++;
					i=j+1;	// skip space
					break;
				}
			}
			if (j == i) {
				// unexpected, no spaces within per_line characters, just print
				// rest of this help line
				fprintf(perf_help_fd, "%-.*s\n", next_len-i, &help[i]);
				lines++;
				break;
			}
		}
	}
	return lines;
}

//  if *type is on the list, caller must hold psm3_stats_lock
static void
psm3_stats_print_help(struct psmi_stats_type *type)
{
	static int had_first_type = 0;

	if (! perf_help_fd)
		return;
	if (! type->help_shown) {
		int i;
		struct psmi_stats_entry *entry;
		int need_dashes = 0;
		int had_first_entry = 0;
		int need_dots = 0;

		if (had_first_type)
			fprintf(perf_help_fd, "================================================================================\n");

		fprintf(perf_help_fd, "%s, Mask 0x%x:\n",
				type->heading, type->statstype);
		if (type->help) {
			fprintf(perf_help_fd, "  ");
			need_dashes = (psm3_print_wrapped_help(2, type->help) > 1);
		}
		for (i=0, entry=&type->entries[0]; i<type->num_entries; i++, entry++) {
			if (entry->desc
			    && ((need_dashes && ! had_first_entry) || need_dots)) {
					fprintf(perf_help_fd, "    ............................................................................\n");
					need_dots = 0;
			}
			if (entry->desc && entry->help) {
				fprintf(perf_help_fd, "    %s: ", entry->desc);
				(void)psm3_print_wrapped_help(strlen(entry->desc)+6,
						entry->help);
			} else if (entry->desc) {
				fprintf(perf_help_fd, "    %s\n", entry->desc);
			} else if (entry->help) { // pure help, describes a group of entries
				if(need_dashes)
					fprintf(perf_help_fd, "   -----------------------------------------------------------------------------\n");
				fprintf(perf_help_fd, "   ");
				need_dots = (psm3_print_wrapped_help(3, entry->help) > 1);
				need_dashes = 1;
			}
			had_first_entry = 1;
		}
		type->help_shown = 1;
		had_first_type = 1;
	}
}

// caller must get psm3_stats_lock
static psm2_error_t
psm3_stats_deregister_type_internal(uint32_t statstype,
					 void *context, int show_help)
{
	struct psmi_stats_type *type;

	STAILQ_FOREACH(type, &psmi_stats, next) {
		if (type->statstype == statstype && type->context == context) {
			// for statistics groups using reregister (fault inj)
			// just output on final deregister
			if (show_help && ! type->help_shown)
				psm3_stats_print_help(type);
			STAILQ_REMOVE(&psmi_stats, type, psmi_stats_type, next);
			psmi_free(type->entries);
			if (type->info)
				psmi_free(type->info);
			if (type->id)
				psmi_free(type->id);
			psmi_free(type);
			return PSM2_OK;
		}
	}
	return PSM2_INTERNAL_ERR;	// not found
}

// caller must hold psm3_stats_lock
static void
psm3_stats_show_help(struct psmi_stats_type *new_type)
{
	struct psmi_stats_type *type;

	if (! perf_help_fd)
		return;
	// see if help already shown for this type, we compare based on heading
	STAILQ_FOREACH(type, &psmi_stats, next) {
		// we could compare actual string pointers
		//if (type->heading != new_type->heading)
		if (0 != strcmp(type->heading, new_type->heading))
			continue;
		if (type->help_shown) {
			new_type->help_shown = 1;
			break;
		}
	}
	if (! new_type->help_shown)
		psm3_stats_print_help(new_type);
}

static psm2_error_t
psm3_stats_register_type_internal(const char *heading, const char *help,
			 uint32_t statstype,
			 const struct psmi_stats_entry *entries_i,
			 int num_entries, const char *id, void *context,
			 const char *info, bool rereg)
{
	struct psmi_stats_entry *entries;
	struct psmi_stats_type *type;
	int i;
	psm2_error_t err = PSM2_OK;

	if (! heading || ! context || ! statstype || ! num_entries || ! entries_i)
		return PSM2_PARAM_ERR;

	entries =
	    psmi_calloc(PSMI_EP_NONE, STATS, num_entries,
			sizeof(struct psmi_stats_entry));
	type =
	    psmi_calloc(PSMI_EP_NONE, STATS, 1, sizeof(struct psmi_stats_type));
	PSMI_CHECKMEM(err, entries);
	PSMI_CHECKMEM(err, type);

	type->entries = entries;
	type->num_entries = num_entries;
	type->statstype = statstype;
	if (id)
		type->id = psmi_strdup(NULL, id);
	type->context = context;
	type->heading = heading;
	type->help = help;
	if (info)
		type->info = psmi_strdup(NULL, info);
#ifdef SYS_gettid
	type->tid = (long int)syscall(SYS_gettid); // gettid();
#else
	type->tid = 0;
#endif
	type->help_shown = 0;

	for (i = 0; i < num_entries; i++) {
		type->entries[i].desc = entries_i[i].desc;
		type->entries[i].help = entries_i[i].help;
		type->entries[i].flags = entries_i[i].flags;
		type->entries[i].getfn = entries_i[i].getfn;
		type->entries[i].u.val = entries_i[i].u.val;
	}

	pthread_spin_lock(&psm3_stats_lock);
	// for statistics groups which repeatedly reregister (fault inj is the
	// only one and does this to grow it's statistics as injectors are
	// encountered), we only output help once on file deregister
	// If needed we could have it re-output the help on each reregistration
	// or just identify how num_entries grew and output the heading
	// and new entry.  We take this approach now a a small job with
	// PSM3_FI=2 and no faults can quickly produce help text for a
	// potentially slower running job with some faults enabled.
	if (rereg)
		(void) psm3_stats_deregister_type_internal(statstype, context, 0);
	else
		psm3_stats_show_help(type);
	STAILQ_INSERT_TAIL(&psmi_stats, type, next);
	pthread_spin_unlock(&psm3_stats_lock);
	return err;

fail:
	if (entries)
		psmi_free(entries);
	if (type) {
		if (type->info)
			psmi_free(type->info);
		if (type->id)
			psmi_free(type->id);
		psmi_free(type);
	}
	return err;
}

psm2_error_t
psm3_stats_register_type(const char *heading, const char *help,
			 uint32_t statstype,
			 const struct psmi_stats_entry *entries_i,
			 int num_entries, const char *id, void *context,
			 const char* info)
{
	return psm3_stats_register_type_internal(heading, help, statstype,
			entries_i, num_entries, id, context, info, 0);
}

psm2_error_t
psm3_stats_reregister_type(const char *heading, const char *help,
			 uint32_t statstype,
			 const struct psmi_stats_entry *entries_i,
			 int num_entries, const char *id, void *context,
			 const char *info)
{
	return psm3_stats_register_type_internal(heading, help, statstype,
			entries_i, num_entries, id, context, info, 1);
}

void psm3_stats_print_env(const char *name, const char *value)
{
	if (! perf_stats_initialized) {
		// stash for possible output when psm3_stats_initialize()
		pthread_spin_lock(&psm3_stats_lock);
		if (perf_stats_env_num < MAX_SAVED_ENV) {
			if (snprintf(perf_stats_env[perf_stats_env_num],
						sizeof(perf_stats_env[0]), "%s=%s\n", name, value))
				perf_stats_env_num++;
		}
		pthread_spin_unlock(&psm3_stats_lock);
	} else if (print_stats_freq && (print_statsmask & PSMI_STATSTYPE_ENV)) {
		pthread_spin_lock(&psm3_stats_lock);
		psmi_open_stats_fd();
		if (perf_stats_fd)
			fprintf(perf_stats_fd, "%s=%s\n", name, value);
		fflush(perf_stats_fd);
		pthread_spin_unlock(&psm3_stats_lock);
	}
}

void psm3_stats_print_env_val(const char *name, int type,
								const union psmi_envvar_val val)
{
	if (! perf_stats_initialized) {
		// stash for possible output when psm3_stats_initialize()
		pthread_spin_lock(&psm3_stats_lock);
		if (perf_stats_env_num < MAX_SAVED_ENV) {
			if (psm3_env_snprint_val(perf_stats_env[perf_stats_env_num],
									sizeof(perf_stats_env[0]), name, type, val))
				perf_stats_env_num++;
		}
		pthread_spin_unlock(&psm3_stats_lock);
	} else if (print_stats_freq && (print_statsmask & PSMI_STATSTYPE_ENV)) {
		pthread_spin_lock(&psm3_stats_lock);
		psmi_open_stats_fd();
		if (perf_stats_fd)
			psm3_env_print_val(perf_stats_fd, name, type, val);
		fflush(perf_stats_fd);
		pthread_spin_unlock(&psm3_stats_lock);
	}
}

void psm3_stats_print_msg(const char *msg)
{
	psmi_assert(perf_stats_initialized);
	if (print_stats_freq && (print_statsmask & PSMI_STATSTYPE_ENV)) {
		pthread_spin_lock(&psm3_stats_lock);
		psmi_open_stats_fd();
		if (perf_stats_fd)
			fputs(msg, perf_stats_fd);
		fflush(perf_stats_fd);
		pthread_spin_unlock(&psm3_stats_lock);
	}
}

void psm3_stats_show(uint32_t statsmask)
{
	struct psmi_stats_type *type;
	time_t now;
	char buf[100];

	pthread_spin_lock(&psm3_stats_lock);
	psmi_open_stats_fd();
	if (! perf_stats_fd)
		goto unlock;

	now = time(NULL);

	fprintf(perf_stats_fd, "\nTime Delta %u seconds %s",
		(unsigned)(now - stats_start), ctime_r(&now, buf));

	STAILQ_FOREACH(type, &psmi_stats, next) {
		int i;
		struct psmi_stats_entry *entry;

		if (! (type->statstype & statsmask))
			continue;
		// when id == NULL, we expect 1 report of given type per
		// process, so we also omit tid.  In which case info probably
		// NULL but show it if provided when stats_register called.
		if (type->id)
			fprintf(perf_stats_fd, " %s id %s%s%s tid %d\n",
				type->heading, type->id, type->info?" ":"",
				type->info?type->info:"", type->tid);
		else
			fprintf(perf_stats_fd, " %s%s%s\n",
				type->heading, type->info?" ":"",
				type->info?type->info:"");
		for (i=0, entry=&type->entries[0]; i<type->num_entries; i++, entry++) {
			uint64_t value;
			if (! entry->desc)	// help text only
				continue;
			value = (entry->getfn != NULL)? entry->getfn(type->context)
										: *entry->u.val;
			if (value || ! (entry->flags & MPSPAWN_STATS_SKIP_IF_ZERO)
					|| (statsmask & _PSMI_STATSTYPE_SHOWZERO))
				fprintf(perf_stats_fd, "    %s %"PRIu64" (%"PRId64")\n", entry->desc,
						 value, (int64_t)value - (int64_t)entry->old_value);
			entry->old_value = value;
		}
	}
	fflush(perf_stats_fd);
unlock:
	pthread_spin_unlock(&psm3_stats_lock);
}

psm2_error_t psm3_stats_deregister_type(uint32_t statstype, void *context)
{
	psm2_error_t err;

	pthread_spin_lock(&psm3_stats_lock);
	err = psm3_stats_deregister_type_internal(statstype, context, 1);
	pthread_spin_unlock(&psm3_stats_lock);
	return err;
}

psm2_error_t psm3_stats_deregister_all(void)
{
	struct psmi_stats_type *type;

	/* Currently our mpi still reads stats after finalize so this isn't safe
	 * yet */
	pthread_spin_lock(&psm3_stats_lock);
	while ((type = STAILQ_FIRST(&psmi_stats)) != NULL) {
		// for statistics groups using reregister (fault inj)
		// just output on final deregister
		if (! type->help_shown)
			psm3_stats_print_help(type);
		STAILQ_REMOVE_HEAD(&psmi_stats, next);
		psmi_free(type->entries);
		if (type->info)
			psmi_free(type->info);
		if (type->id)
			psmi_free(type->id);
		psmi_free(type);
	}
	pthread_spin_unlock(&psm3_stats_lock);

	return PSM2_OK;
}

static
void
*psm3_print_stats_thread(void *unused)
{
	if (print_stats_freq <= 0)
		goto end;

	psmi_open_stats_fd();
	if (!perf_stats_fd)
		goto end;

	/* Performance stats will be printed every $PSM3_PRINT_STATS seconds */
	do {
		psm3_stats_show(print_statsmask);
		usleep(MICRO_SEC * print_stats_freq);
	} while (print_stats_running);

end:
	pthread_exit(NULL);
}

static void
psm3_print_stats_init_thread(void)
{
	print_stats_running = 1;
	if (pthread_create(&perf_print_thread, NULL,
				psm3_print_stats_thread, (void*)NULL))
	{
		print_stats_running = 0;
		_HFI_ERROR("Failed to create logging thread\n");
	}
}

static void print_job_info_help(void)
{
	if (! perf_help_fd)
		return;
	if (! (print_statsmask & PSMI_STATSTYPE_ENV))
		return;

	fprintf(perf_help_fd, "Job_information, Mask 0x%x:\n",
			PSMI_STATSTYPE_ENV);
	fprintf(perf_help_fd, "  ");
	psm3_print_wrapped_help(2, "The command line, complete environment, PSM3 parameters and the IDENTIFY information will be output once at job start.  PSM3 parameters are output when parsed, and if multiple EPs are opened may be shown repeatedly as each EP parses them (and may receive different settings from the middeware).");
	fprintf(perf_help_fd, "    ............................................................................\n");
	fprintf(perf_help_fd, "    cmdline: ");
	(void)psm3_print_wrapped_help(strlen("cmdline")+6,
					"The command line for the process");
	fprintf(perf_help_fd, "    environ: ");
	(void)psm3_print_wrapped_help(strlen("environ")+6,
					"The complete environment for the process");
	fprintf(perf_help_fd, "    PSM3 settings: ");
	(void)psm3_print_wrapped_help(strlen("PSM3 settings")+6,
					"Each PSM3_ or FI_PSM3_ setting as parsed from the environment or /etc/psm3.conf is shown");
	fprintf(perf_help_fd, "    PSM3_IDENTIFY: ");
	(void)psm3_print_wrapped_help(strlen("PSM3_IDENTIFY")+6,
					"The PSM3_IDENTIFY information for each process is output to its psm3-perf-stat output file, even if the PSM3_IDENTIFY setting is not specified");
	fprintf(perf_help_fd, "================================================================================\n");
}

// output information about our process as found in
// /proc/PID/name
// any NUL characters found in the file are replaced with "replace"
static void print_proc_info(FILE* out, char *name, char replace)
{
	char filename[80];
	FILE *in;

	snprintf(filename, sizeof(filename), "/proc/%d/%s", getpid(), name);
	in = fopen(filename, "r");
	if (in) {
			int c;
			while ((c = fgetc(in)) != EOF) {
				if (c == 0)
					fputc(replace, out);
				else
					fputc(c, out);
			}
			fclose(in);
	} else {
		_HFI_ERROR("Failed to open fd for process info: %s: %s\n",
			filename, strerror(errno));
	}
}

static void print_basic_job_info(void)
{
	int i;

	if (! print_stats_freq)
		return;
	if (! (print_statsmask & PSMI_STATSTYPE_ENV))
		return;

	pthread_spin_lock(&psm3_stats_lock);
	psmi_open_stats_fd();
	if (perf_stats_fd) {

		// OS puts NUL in cmdline in place of spaces
		fprintf(perf_stats_fd, "cmdline: ");
		print_proc_info(perf_stats_fd, "cmdline", ' ');
		fprintf(perf_stats_fd, "\n");

		// OS puts NUL between env variables
		fprintf(perf_stats_fd, "environ:\n");
		print_proc_info(perf_stats_fd, "environ", '\n');
		fprintf(perf_stats_fd, "-------------------------------------------------------------------------------\n");

		// show stashed env information prior to this function being called
		for (i=0; i< perf_stats_env_num; i++)
			fprintf(perf_stats_fd, "%s", perf_stats_env[i]);
		fflush(perf_stats_fd);
	}
	pthread_spin_unlock(&psm3_stats_lock);
}

psm2_error_t
psm3_stats_initialize(void)
{
	union psmi_envvar_val env_stats_freq;
	union psmi_envvar_val env_stats_prefix;
	union psmi_envvar_val env_stats_help;
	union psmi_envvar_val env_statsmask;
	int noenv_stats_freq;	// env var not specified, used default
	int noenv_stats_prefix;	// env var not specified, used default
	int noenv_stats_help;	// env var not specified, used default
	int noenv_statsmask;	// env var not specified, used default

	psmi_assert(! perf_stats_initialized);

	noenv_stats_freq = (0 < psm3_getenv_range("PSM3_PRINT_STATS",
			"Prints performance stats every n seconds",
			"  0 - disable output\n"
			"  -1 - only output once at end of job on 1st ep close\n"
			"  >=1 - output every n seconds\n"
			"  val: - limit output to rank 0 (for val of -1 or >=1)\n"
			"  val:pattern - limit output to processes whose label matches\n    "
#ifdef FNM_EXTMATCH
                                "extended "
#endif
                                "glob pattern (for val of -1 or >=1)\n"
			"Output goes to file ${PSM3_PRNT_STATS_PREFIX}psm3-perf-stat-[hostname]-pid-[pid]",
			PSMI_ENVVAR_LEVEL_USER|PSMI_ENVVAR_FLAG_NOABBREV,
			PSMI_ENVVAR_TYPE_STR_VAL_PAT_INT,
			(union psmi_envvar_val)"0",
			(union psmi_envvar_val)-1, (union psmi_envvar_val)INT_MAX,
                        NULL, NULL, &env_stats_freq));
	(void)psm3_parse_val_pattern_int(env_stats_freq.e_str, 0,
			&print_stats_freq,
			PSMI_ENVVAR_FLAG_NOABBREV, -1, INT_MAX);

	noenv_stats_prefix = (0 < psm3_getenv_range("PSM3_PRINT_STATS_PREFIX",
			"Prefix for filename for performance stats output",
			"May be used to add a prefix possibly including directory for output",
			PSMI_ENVVAR_LEVEL_USER|PSMI_ENVVAR_FLAG_NOABBREV,
			PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"./",
			(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
                        NULL, NULL, &env_stats_prefix));

	noenv_stats_help = (0 < psm3_getenv("PSM3_PRINT_STATS_HELP",
			"Prints performance stats help text on rank 0 to file "
			"${PSM3_PRINT_STATS_PREFIX}psm3-perf-stat-help-[hostname]-pid-[pid]",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val) 0, &env_stats_help));
	print_stats_help = env_stats_help.e_uint && (psm3_get_myrank() == 0);

	noenv_statsmask = (0 < psm3_getenv("PSM3_PRINT_STATSMASK",
			"Mask of statistic types to print: "
			"MQ=1, RCVTHREAD=0x100, IPS=0x200"
#if   defined(PSM_HAVE_REG_MR)
			", RDMA=0x400, MRCache=0x800"
#endif
#ifdef PSM_DEBUG
			", MEMORY=0x1000"
#endif
#if defined(PSM_HAVE_REG_MR) && defined(PSM_HAVE_RNDV_MOD)
			", RVEvents=0x2000, RVRDMA=0x4000"
#endif
#ifdef PSM_FI
			", FaultInj=0x8000"
#endif
			".  0x100000 causes zero values to also be shown",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			(union psmi_envvar_val) PSMI_STATSTYPE_ALL, &env_statsmask));
	print_statsmask = env_statsmask.e_uint;

	stats_start = time(NULL);

	snprintf(perf_file_name, sizeof(perf_file_name),
			"%spsm3-perf-stat-%s-pid-%d",
			env_stats_prefix.e_str, psm3_gethostname(), getpid());

	if (print_stats_help) {
		// a few optons, such as CUDA, ONEAPI_ZE, RDMA affect what is
		// included in help, so use a unique filename per job
		snprintf(perf_help_file_name, sizeof(perf_help_file_name),
				"%spsm3-perf-stat-help-%s-pid-%d",
				env_stats_prefix.e_str, psm3_gethostname(), getpid());
		perf_help_fd = fopen(perf_help_file_name, "w");
		if (!perf_help_fd)
			_HFI_ERROR("Failed to create fd for performance logging help: %s: %s\n",
				perf_help_file_name, strerror(errno));
	}
	perf_stats_initialized = 1;

	print_job_info_help();
	print_basic_job_info();

	// if got a valid value or an invalid value, psm3_getenv will have
	// stashed it and print_basic_job_info will have put in stats file
	// otherwise we want to always report the STATS variable settings
	if (noenv_stats_freq)
		psm3_stats_print_env_val("PSM3_PRINT_STATS",
								PSMI_ENVVAR_TYPE_UINT, env_stats_freq);
	if (noenv_stats_prefix)
		psm3_stats_print_env_val("PSM3_PRINT_STATS_PREFIX",
								PSMI_ENVVAR_TYPE_STR, env_stats_prefix);
	if (noenv_stats_help)
		psm3_stats_print_env_val("PSM3_PRINT_STATS_HELP",
								PSMI_ENVVAR_TYPE_UINT, env_stats_help);
	if (noenv_statsmask)
		psm3_stats_print_env_val("PSM3_PRINT_STATSMASK",
								PSMI_ENVVAR_TYPE_UINT_FLAGS, env_statsmask);

	if (print_stats_freq > 0)
		psm3_print_stats_init_thread();
	return PSM2_OK;
}

void
psm3_stats_finalize(void)
{
	if (print_stats_freq == -1) {
		psm3_stats_show(print_statsmask);
	} else if (print_stats_running) {
		print_stats_running = 0;
		pthread_join(perf_print_thread, NULL);
	}
	if (perf_stats_fd) {
		fclose(perf_stats_fd);
		perf_stats_fd = NULL;
	}
	if (perf_help_fd) {
		fclose(perf_help_fd);
		perf_help_fd = NULL;
	}
	psm3_stats_deregister_all();
	perf_stats_env_num = 0;
	perf_stats_initialized = 0;
}

// called at start of ep_close so we can output 1 shot as needed while
// most of the interesting stats are available
// we only output if we have done no previous outputs, so
// if there are multiple EPs this only outputs on 1st EP close
void
psm3_stats_ep_close(void)
{
	static int last_stats_printed = 0;
	if (print_stats_freq != 0 && !last_stats_printed) {
		psm3_stats_show(print_statsmask);
		last_stats_printed = 1;
	}
}

#if 0   // unused code, specific to QLogic MPI
static uint32_t typestring_to_type(const char *typestr)
{
	if (strncasecmp(typestr, "all", 4) == 0)
		return PSMI_STATSTYPE_ALL;
	else if (strncasecmp(typestr, "p2p", 4) == 0)
		return PSMI_STATSTYPE_P2P;
	else if (strncasecmp(typestr, "hfi", 6) == 0)
		return PSMI_STATSTYPE_HFI;
	else if (strncasecmp(typestr, "ips", 4) == 0)
		return PSMI_STATSTYPE_IPSPROTO;
	else if ((strncasecmp(typestr, "intr", 5) == 0) ||
		 (strncasecmp(typestr, "thread", 7) == 0) ||
		 (strncasecmp(typestr, "rcvthread", 10) == 0))
		return PSMI_STATSTYPE_RCVTHREAD;
	else if ((strncasecmp(typestr, "mq", 3) == 0) ||
		 (strncasecmp(typestr, "mpi", 4) == 0))
		return PSMI_STATSTYPE_MQ;
	else if ((strncasecmp(typestr, "tid", 4) == 0) ||
		 (strncasecmp(typestr, "tids", 5) == 0))
		return PSMI_STATSTYPE_RDMA;
	else if ((strncasecmp(typestr, "memory", 7) == 0) ||
		 (strncasecmp(typestr, "alloc", 6) == 0) ||
		 (strncasecmp(typestr, "malloc", 7) == 0))
		return PSMI_STATSTYPE_MEMORY;
	else
		return 0;
}

static uint32_t stats_parse_enabled_mask(const char *stats_string)
{
	char *b = (char *)stats_string;
	char *e = b;
	char buf[128];

	uint32_t stats_enabled_mask = 0;

	while (*e) {
		b = e;
		while (*e && *e != ',' && *e != '+' && *e != '.' &&
		       *e != '|' && *e != ':')
			e++;
		if (e > b) {	/* something new to parse */
			int len = ((e - b) > (sizeof(buf) - 1)) ?
			    (sizeof(buf) - 1) : (e - b);
			strncpy(buf, b, len);
			buf[len] = '\0';
			stats_enabled_mask |= typestring_to_type(buf);
		}
		if (*e)
			e++;	/* skip delimiter */
	}
	return stats_enabled_mask;
}

static
void psmi_stats_mpspawn_callback(struct mpspawn_stats_req_args *args)
{
	const struct psmi_stats_entry *entry;
	struct psmi_stats_type *type = (struct psmi_stats_type *)args->context;
	int i, num = args->num;
	uint64_t *stats = args->stats;
	uint64_t *c = NULL;
	uint64_t *s = NULL;

	psmi_assert(num == type->num_entries);

	 if (type->statstype == PSMI_STATSTYPE_MEMORY) {
		for (i = 0; i < num; i++) {
			entry = &type->entries[i];
			psmi_assert(entry->desc);	// dead code, broken if pure help entry
			stats[i] =
			    *(uint64_t *) ((uintptr_t) &psm3_stats_memory +
					   (uintptr_t) entry->u.off);
		}
	} else {
		for (i = 0; i < num; i++) {
			entry = &type->entries[i];
			psmi_assert(entry->desc);	// dead code, broken if pure help entry
			if (entry->getfn != NULL)
				stats[i] = entry->getfn(type->context);
			else
				stats[i] = *entry->u.val;
		}
	}

	if (c != NULL)
		psmi_free(c);
	if (s != NULL)
		psmi_free(s);
}

static
void
stats_register_mpspawn_single(mpspawn_stats_add_fn add_fn,
			      char *heading,
			      int num_entries,
			      struct psmi_stats_entry *entries,
			      mpspawn_stats_req_fn req_fn, void *context)
{
	int i;
	struct mpspawn_stats_add_args mp_add;

	mp_add.version = MPSPAWN_STATS_VERSION;
	mp_add.num = num_entries;
	mp_add.header = heading;
	mp_add.req_fn = req_fn;
	mp_add.context = context;

	mp_add.desc = (char **)alloca(sizeof(char *) * num_entries);

	mp_add.flags = (uint16_t *) alloca(sizeof(uint16_t *) * num_entries);

	for (i = 0; i < num_entries; i++) {
		mp_add.desc[i] = (char *)entries[i].desc;
		mp_add.flags[i] = entries[i].flags;
	}

	/* Ignore return code, doesn't matter to *us* if register failed */
	add_fn(&mp_add);

	return;
}

static void stats_register_mem_stats(psm2_ep_t ep);
static psm2_error_t psmi_stats_epaddr_register(struct mpspawn_stats_init_args
					      *args);

/*
 * Downcall from QLogic MPI into PSM, so we can register stats
 */
void *psmi_stats_register(struct mpspawn_stats_init_args *args)
{
	struct psmi_stats_type *type;
	uint32_t statsmask;

	/*
	 * Args has a version string in it, but we can ignore it since mpspawn
	 * will decide if it supports *our* version
	 */

	/*
	 * Eventually, parse the stats_types to add various "flavours" of stats
	 */
	if (args->stats_types == NULL)
		return NULL;

	statsmask = stats_parse_enabled_mask(args->stats_types);

	/* MQ (MPI-level) statistics */
	if (statsmask & PSMI_STATSTYPE_MQ)
		psm3_mq_stats_register(args->mq, args->add_fn);


	if (statsmask & PSMI_STATSTYPE_MEMORY)
		stats_register_mem_stats(args->mq->ep);

	/*
	 * At this point all PSM and hfi-level components have registered stats
	 * with the PSM stats interface.  We register with the mpspawn stats
	 * interface with an upcall in add_fn
	 */
	STAILQ_FOREACH(type, &psmi_stats, next) {
		if (type->statstype & statsmask)
			stats_register_mpspawn_single(args->add_fn,
						      type->heading,
						      type->num_entries,
						      type->entries,
						      psmi_stats_mpspawn_callback,
						      type);
	}

	/*
	 * Special handling for per-endpoint statistics
	 * Only MPI knows what the endpoint-addresses are in the running program,
	 * PSM has no sense of MPI worlds.  In stats register, MPI tells PSM how
	 * many endpoints it anticipates having and PSM simply reserves that amount
	 * of stats entries X the amount of per-endpoint stats.
	 */
	if (statsmask & PSMI_STATSTYPE_P2P)
		psmi_stats_epaddr_register(args);

	return NULL;
}

struct stats_epaddr {
	psm2_ep_t ep;
	mpspawn_map_epaddr_fn epaddr_map_fn;
	int num_ep;
	int num_ep_stats;
};

static
void psmi_stats_epaddr_callback(struct mpspawn_stats_req_args *args)
{
	int i, num, off;
	uint64_t *statsp;
	struct stats_epaddr *stats_ctx = (struct stats_epaddr *)args->context;
	psm2_ep_t ep = stats_ctx->ep;
	psm2_epaddr_t epaddr;

	num = stats_ctx->num_ep * stats_ctx->num_ep_stats;

	/* First always NAN the entire stats request */
	for (i = 0; i < num; i++) {
		if (args->flags[i] & MPSPAWN_STATS_TYPE_DOUBLE)
			args->stats[i] = MPSPAWN_NAN;
		else
			args->stats[i] = MPSPAWN_NAN_U64;
	}

	for (i = 0; i < stats_ctx->num_ep; i++) {
		statsp = args->stats + i * stats_ctx->num_ep_stats;
		off = 0;
		epaddr = stats_ctx->epaddr_map_fn(i);
		if (epaddr == NULL)
			continue;

		/* Self */
		if (&ep->ptl_self == epaddr->ptlctl) {
			if (ep->ptl_self.epaddr_stats_get != NULL)
				off +=
				    ep->ptl_self.epaddr_stats_get(epaddr,
								  statsp + off);
		} else {
			if (ep->ptl_self.epaddr_stats_num != NULL)
				off += ep->ptl_self.epaddr_stats_num();
		}

		/* Shm */
		if (&ep->ptl_amsh == epaddr->ptlctl) {
			if (ep->ptl_amsh.epaddr_stats_get != NULL)
				off +=
				    ep->ptl_amsh.epaddr_stats_get(epaddr,
								  statsp + off);
		} else {
			if (ep->ptl_amsh.epaddr_stats_num != NULL)
				off += ep->ptl_amsh.epaddr_stats_num();
		}

		/* ips */
		if (&ep->ptl_ips == epaddr->ptlctl) {
			if (ep->ptl_ips.epaddr_stats_get != NULL)
				off +=
				    ep->ptl_ips.epaddr_stats_get(epaddr,
								 statsp + off);
		} else {
			if (ep->ptl_ips.epaddr_stats_num != NULL)
				off += ep->ptl_ips.epaddr_stats_num();
		}
	}
	return;
}

static
psm2_error_t
psmi_stats_epaddr_register(struct mpspawn_stats_init_args *args)
{
	int i = 0, j;
	int num_ep = args->num_epaddr;
	int num_ep_stats = 0;
	int nz;
	char **desc, **desc_i;
	uint16_t *flags, *flags_i;
	char *p;
	char buf[128];
	psm2_ep_t ep;
	struct mpspawn_stats_add_args mp_add;
	struct stats_epaddr *stats_ctx;
	psm2_error_t err = PSM2_OK;

	if (args->mq == NULL)
		return PSM2_OK;
	ep = args->mq->ep;

	/* Figure out how many stats there are in an endpoint from all devices */
	if (ep->ptl_self.epaddr_stats_num != NULL)
		num_ep_stats += ep->ptl_self.epaddr_stats_num();
	if (ep->ptl_amsh.epaddr_stats_num != NULL)
		num_ep_stats += ep->ptl_amsh.epaddr_stats_num();
	if (ep->ptl_ips.epaddr_stats_num != NULL)
		num_ep_stats += ep->ptl_ips.epaddr_stats_num();

	/* Allocate desc and flags and let each device initialize their
	 * descriptions and flags */
	desc =
	    psmi_malloc(ep, STATS,
			sizeof(char *) * num_ep_stats * (num_ep + 1));
	if (desc == NULL)
		return PSM2_NO_MEMORY;
	flags =
	    psmi_malloc(ep, STATS,
			sizeof(uint16_t) * num_ep_stats * (num_ep + 1));
	if (flags == NULL) {
		psmi_free(desc);
		return PSM2_NO_MEMORY;
	}

	/* Get the descriptions/flags from each device */
	i = 0;
	i += ep->ptl_self.epaddr_stats_num != NULL ?
	    ep->ptl_self.epaddr_stats_init(desc + i, flags + i) : 0;
	i += ep->ptl_amsh.epaddr_stats_num != NULL ?
	    ep->ptl_amsh.epaddr_stats_init(desc + i, flags + i) : 0;
	i += ep->ptl_ips.epaddr_stats_num != NULL ?
	    ep->ptl_ips.epaddr_stats_init(desc + i, flags + i) : 0;
	psmi_assert_always(i == num_ep_stats);

	/*
	 * Clone the descriptions for each endpoint but append "rank %d" to it
	 * beforehand.
	 */
	nz = (num_ep < 10 ? 1 : (num_ep < 100 ? 2 :	/* cheap log */
				 (num_ep < 1000 ? 3 : (num_ep < 1000 ? 4 :
						       (num_ep <
							10000 ? 5 : 6)))));

	desc_i = desc + num_ep_stats;
	flags_i = flags + num_ep_stats;
	memset(desc_i, 0, sizeof(char *) * num_ep * num_ep_stats);

	for (i = 0; i < num_ep; i++) {
		for (j = 0; j < num_ep_stats; j++) {
			snprintf(buf, sizeof(buf) - 1, "<%*d> %s", nz, i,
				 desc[j]);
			buf[sizeof(buf) - 1] = '\0';
			p = psmi_strdup(ep, buf);
			if (p == NULL) {
				err = PSM2_NO_MEMORY;
				goto clean;
			}
			desc_i[i * num_ep_stats + j] = p;
			flags_i[i * num_ep_stats + j] = flags[j];
		}
	}

	mp_add.version = MPSPAWN_STATS_VERSION;
	mp_add.num = num_ep_stats * num_ep;
	mp_add.header = "Endpoint-to-Endpoint Stats (by <rank>)";
	mp_add.req_fn = psmi_stats_epaddr_callback;
	mp_add.desc = desc_i;
	mp_add.flags = flags_i;
	stats_ctx = psmi_malloc(ep, STATS, sizeof(struct stats_epaddr));
	if (stats_ctx == NULL) {
		err = PSM2_NO_MEMORY;
		goto clean;
	}
	stats_ctx->ep = ep;
	stats_ctx->epaddr_map_fn = args->epaddr_map_fn;
	stats_ctx->num_ep = num_ep;
	stats_ctx->num_ep_stats = num_ep_stats;
	mp_add.context = stats_ctx;

	args->add_fn(&mp_add);

clean:
	/* Now we can free all the descriptions */
	for (i = 0; i < num_ep; i++) {
		for (j = 0; j < num_ep_stats; j++)
			if (desc_i[i * num_ep_stats + j])
				psmi_free(desc_i[i * num_ep_stats + j]);
	}

	psmi_free(desc);
	psmi_free(flags);

	return err;
}



#undef _SDECL
#define _SDECL(_desc, _param) {					\
	    .desc  = _desc,					\
	    .help  = NULL,					\
	    .flags = MPSPAWN_STATS_REDUCTION_ALL		\
		     | MPSPAWN_STATS_SKIP_IF_ZERO,		\
	    .getfn = NULL,					\
	    .u.off = offsetof(struct psmi_stats_malloc, _param)	\
	}

static
void stats_register_mem_stats(psm2_ep_t ep)
{
	struct psmi_stats_entry entries[] = {
		_SDECL("Total_(current)", m_all_total),
		_SDECL("Total_(max)", m_all_max),
		_SDECL("All_Peers_(current)", m_perpeer_total),
		_SDECL("All_Peers_(max)", m_perpeer_max),
		_SDECL("Network_Buffers_(current)", m_netbufs_total),
		_SDECL("Network_Buffers_(max)", m_netbufs_max),
		_SDECL("PSM_desctors_(current)", m_descriptors_total),
		_SDECL("PSM_desctors_(max)", m_descriptors_max),
		_SDECL("Unexp._buffers_(current)", m_unexpbufs_total),
		_SDECL("Unexp._Buffers_(max)", m_unexpbufs_max),
#ifdef PSM_HAVE_RNDV_MOD
		_SDECL("Peer_Rndv_(current)", m_peerrndv_total),
		_SDECL("Peer_Rndv_(max)", m_peerrndv_max),
#endif
		_SDECL("Other_(current)", m_undefined_total),
		_SDECL("Other_(max)", m_undefined_max),
	};

	// TBD - these are global, should only call once and not provide
	// ep nor device name
	psm3_stats_register_type("PSM_memory_allocation_statistics", NULL,
				 PSMI_STATSTYPE_MEMORY,
				 entries, PSMI_HOWMANY(entries), ep,
				 ep->dev_name);
}
#endif // 0   // unused code, specific to QLogic MPI
