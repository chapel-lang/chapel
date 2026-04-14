/*
* Copyright (c) 2025 Philipp A. Friese, Technical University of Munich
*
* This software is available to you under a choice of one of two
* licenses.  You may choose to be licensed under the terms of the GNU
* General Public License (GPL) Version 2, available from the file
* COPYING in the main directory of this source tree, or the
* BSD license below:
*
*     Redistribution and use in source and binary forms, with or
*     without modification, are permitted provided that the following
*     conditions are met:
*
*      - Redistributions of source code must retain the above
*        copyright notice, this list of conditions and the following
*        disclaimer.
*
*      - Redistributions in binary form must reproduce the above
*        copyright notice, this list of conditions and the following
*        disclaimer in the documentation and/or other materials
*        provided with the distribution.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <config.h>

#include <unistd.h>
#include <getopt.h>
#include <inttypes.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>

#include <ofi_mem.h>
#include <rdma/fi_errno.h>

#include <sys/stat.h>

#ifdef __APPLE__
#include <sys/mman.h>
#endif

#include <libgen.h>
#include <dirent.h>
#include <prov/hook/monitor/include/hook_monitor.h>

static volatile sig_atomic_t running = 0;

static void signal_handler(int signal) {
	running = 0;
}

enum ms_formats {
	MS_CSV=0,
};

struct ms_opts {
	char *target_path;
	char *output;
	unsigned long watch_usec;
	enum ms_formats format;
};

struct file_entry {
	char in_path[PATH_MAX];
	char out_path[PATH_MAX];
	struct monitor_mapped_data* share;
	struct file_entry *next;
	FILE *output;
	bool is_mapped;
	bool finalize;
	bool header_written;
};

struct ct_mon_sampler {
	struct ms_opts opts;
	struct monitor_data data[mon_api_size];
	mode_t target_mode;
	struct file_entry *files;
};

// Note: keep in-sync with prov/hook/monitor/include/hook_monitor.h
static const char *mon_functions[] = {
	"mon_recv",         "mon_recvv",        "mon_recvmsg",
	"mon_trecv",        "mon_trecvv",       "mon_trecvmsg",
	"mon_send",         "mon_sendv",        "mon_sendmsg",
	"mon_inject",       "mon_senddata",     "mon_injectdata",
	"mon_tsend",        "mon_tsendv",       "mon_tsendmsg",
	"mon_tinject",      "mon_tsenddata",    "mon_tinjectdata",
	"mon_read",         "mon_readv",        "mon_readmsg",
	"mon_write",        "mon_writev",       "mon_writemsg",
	"mon_inject_write", "mon_writedata",    "mon_inject_writedata",
	"mon_mr_reg",       "mon_mr_regv",      "mon_mr_regattr",
	"mon_cq_read",      "mon_cq_readfrom",  "mon_cq_readerr",
	"mon_cq_sread",     "mon_cq_sreadfrom", "mon_cq_ctx",
	"mon_cq_msg_tx",    "mon_cq_msg_rx",    "mon_cq_data_tx",
	"mon_cq_data_rx",   "mon_cq_tagged_tx", "mon_cq_tagged_rx",
};

static const char* mon_buckets[] = {
	"0_64",	    "64_512",  "512_1K", "1K_4K", "4K_64K",
	"64K_256K", "256K_1M", "1M_4M",	 "4M_UP",
};

bool file_exists(struct file_entry *file_ptr, char *in_path) {
	while (file_ptr != NULL) {
		if (strncmp(in_path, file_ptr->in_path, PATH_MAX) == 0)
			return true;
		file_ptr = file_ptr->next;
	}
	return false;
}

/*******************************************************************************
 *                         Output Functions
 ******************************************************************************/

static int ms_write_csv(struct monitor_data data[mon_api_size], struct file_entry *file) {
	if (!file->header_written) {
		for(int i = 0; i < mon_api_size; i++) {
			for (int j = 0; j < MON_SIZE_MAX; j++) {
				fprintf(file->output, "%s_%s_c,%s_%s_s",
					mon_functions[i], mon_buckets[j],
					mon_functions[i], mon_buckets[j]);
				if (!(i+1 == mon_api_size
				      && j+1 == MON_SIZE_MAX))
					fprintf(file->output, ",");
			}

		}
		fprintf(file->output, "\n");
		file->header_written = true;
	}

	for(int i = 0; i < mon_api_size; i++) {
		for (int j = 0; j < MON_SIZE_MAX; j++) {
			fprintf(file->output, "%lu,%lu",
				data[i].count[j],
				data[i].sum[j]);
			if (!(i+1 == mon_api_size && j+1 == MON_SIZE_MAX))
				fprintf(file->output, ",");
		}
	}
	fprintf(file->output, "\n");

	return 0;
}

static void ms_output_data(struct ct_mon_sampler *ct,
			   struct file_entry *file) {
	switch (ct->opts.format) {
	case MS_CSV:
		ms_write_csv(ct->data, file);
		break;
	default:
		break;
	}
}

/*******************************************************************************
 *                         Resource Management Functions
 ******************************************************************************/

int ms_free_file_entry(struct file_entry *file) {
	if (file->output != NULL && file->output != stdout) {
		fsync(fileno(file->output));
		fclose(file->output);
	}
	if (file->is_mapped)
		munmap(file->share, sizeof(struct monitor_mapped_data));
	if (file->finalize)
		remove(file->in_path);
	fprintf(stderr, "Closed %s\n", file->in_path);
	return 0;
}

// linked list helper functions
void ms_add_file_entry(struct ct_mon_sampler *ct, struct file_entry *entry) {
	if (ct->files == NULL)
		ct->files = entry;
	else {
		struct file_entry *file_ptr = ct->files;
		while (file_ptr->next != NULL)
			file_ptr = file_ptr->next;
		file_ptr->next = entry;
	}
}

struct file_entry* ms_remove_file_entry(struct ct_mon_sampler *ct,
					struct file_entry *entry) {
	if (ct->files == entry) {
		ct->files = ct->files->next;
		ms_free_file_entry(entry);
		free(entry);
		return ct->files;
	}

	struct file_entry *file_ptr = ct->files;
	while (file_ptr != NULL) {
		if (file_ptr->next == entry) {
			file_ptr = file_ptr->next->next;
			break;
		}
		file_ptr = file_ptr->next;
	}
	ms_free_file_entry(entry);
	free(entry);
	return file_ptr;
}

// file entry management functions
int ms_create_file_entry(struct ct_mon_sampler *ct, struct file_entry *file) {
	if (file->is_mapped) {
		return 0;
	}
	int fd = open(file->in_path, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Could not open %s: %s\n", file->in_path, strerror(errno));
		return -ENOENT;
	}
	file->share = mmap(0, sizeof (struct monitor_mapped_data),
			   PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	if (file->share == MAP_FAILED) {
		fprintf(stderr, "Could not mmap %s: %s\n", file->in_path, strerror(errno));
		return -EINVAL;
	}
	file->is_mapped = true;

	char format[16] = "";
	switch (ct->opts.format) {
	case MS_CSV:
		snprintf(format, 16, "csv");
		break;
	default:
		break;
	}

	if (file->output == NULL) {
		if (snprintf(file->out_path, PATH_MAX, "%s/%s.%s", ct->opts.output,
			     basename(file->in_path), format) < 0) {
			fprintf(stderr, "Could not format out_path for file %s\n",
			       file->in_path);
			return -EINVAL;
		}
		file->output = fopen(file->out_path, "a");
		if (file->output == NULL) {
			fprintf(stderr, "Could not open out_path %s: %s\n",
			       file->out_path, strerror(errno));
			return -EINVAL;
		}

		// make sure file got created so that below stat-call works in all cases
		fsync(fileno(file->output));
		struct stat st;
		if (stat(file->out_path, &st) == -1) {
			fprintf(stderr, "Could not stat %s: %s\n",
			       file->out_path, strerror(errno));
			return -EINVAL;
		}
		if (st.st_size > 0)
			file->header_written = true;

		fprintf(stderr, "Writing sampled data to %s\n", file->out_path);
	}
	return 0;
}

int ms_update_file_entries(struct ct_mon_sampler *ct) {
	if (S_ISREG(ct->target_mode)) { // regular file
		if (ct->files == NULL) {
			struct file_entry *fentry = calloc(1, sizeof(struct file_entry));
			strncpy(fentry->in_path, ct->opts.target_path, PATH_MAX-1);

			// for single-file sampling, print to stdout unless specified otherwise
			if (ct->opts.output == NULL)
				fentry->output = stdout;
			int ret = ms_create_file_entry(ct, fentry);
			if (ret != 0)
				return ret;

			ms_add_file_entry(ct, fentry);
		}
	}
	else if (S_ISDIR(ct->target_mode)) { // directory
		struct dirent *dir_entry;
		DIR *dr;
		if ((dr = opendir(ct->opts.target_path)) == NULL) {
			fprintf(stderr, "Could not open directory %s: %s\n",
			       ct->opts.target_path, strerror(errno));
			return -EEXIST;
		}
		while ((dir_entry = readdir(dr)) != NULL) {
			if (dir_entry->d_type != DT_REG)
				continue;

			char in_path[PATH_MAX];
			if (snprintf(in_path, PATH_MAX, "%s/%s",
				     ct->opts.target_path, dir_entry->d_name) < 0) {
				fprintf(stderr, "Could not format in_path for file %s\n",
				       dir_entry->d_name);
				closedir(dr);
				return -EINVAL;
			}
			if (!file_exists(ct->files, in_path)) {
				struct file_entry *fentry = calloc(1, sizeof(struct file_entry));
				if (fentry == NULL) {
					closedir(dr);
					return -ENOMEM;
				}
				strncpy(fentry->in_path, in_path, PATH_MAX);
				int ret = ms_create_file_entry(ct, fentry);
				if (ret != 0) {
					closedir(dr);
					return ret;
				}
				ms_add_file_entry(ct, fentry);
			}
		}
		closedir(dr);
	}

	// check whether any files have to be deleted
	struct file_entry *file_ptr = ct->files;
	while (file_ptr != NULL) {
		if (file_ptr->finalize) {
			file_ptr = ms_remove_file_entry(ct, file_ptr);
			continue;
		}
		struct stat st;
		if (stat(file_ptr->in_path, &st) == -1) {
			file_ptr = ms_remove_file_entry(ct, file_ptr);
			continue;
		}
		file_ptr = file_ptr->next;
	}

	return 0;
}

static void ms_cleanup(struct ct_mon_sampler *ct) {
	// free all file_entry entries
	struct file_entry *file_ptr = ct->files;
	while (file_ptr != NULL) {
		struct file_entry *del_ptr = file_ptr;
		ms_free_file_entry(del_ptr);
		file_ptr = file_ptr->next;
		free(del_ptr);
	}
	ct->files = NULL;
}

/*******************************************************************************
 *                         Data Extraction Function
 ******************************************************************************/

static int ms_extract_data(struct ct_mon_sampler *ct, struct file_entry *entry) {
	// check if data request is still pending
	if (entry->share->flags & 0b1)
		return -1;

	memcpy(ct->data, entry->share, sizeof (ct->data));

	// set request bit again
	entry->share->flags |= 0b1;

	// check if hook provider indicated end-of-data & that we should delete the file
	if ((entry->share->flags & 0b10) >> 1)
		entry->finalize = true;
	return 0;
}

/*******************************************************************************
 *                         Main Run Loop
 ******************************************************************************/

int ms_run(struct ct_mon_sampler *ct) {
	int ret = ms_update_file_entries(ct);
	if (ret) {
		running = 0;
		return ret;
	}

	struct file_entry *file_ptr = ct->files;
	while (file_ptr != NULL) {
		ret = ms_extract_data(ct, file_ptr);

		switch (ret) {
		case 0:
			ms_output_data(ct, file_ptr);
			break;
		case -1:
		default:
			break;
		}
		file_ptr = file_ptr->next;
	}

	return 0;
}

/*******************************************************************************
*                         CLI: Usage and Options parsing
 ******************************************************************************/

static void ms_usage(char *name) {
	fprintf(stderr, "Sampler for ofi_hook_monitor provider\n\n");

	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  %s [OPTIONS] <file>\t"
			"start sampler on ofi_hook_monitor output file <file>\n",
		name);

	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, " %-20s %s\n", "-w <msec>",
		"watch file for changes, wait <msec> milliseconds between checks");
	fprintf(stderr, " %-20s %s\n", "-f <format>",
		"output format (CSV)");
	fprintf(stderr, " %-20s %s\n", "-o <outpath>",
		"output file (stdout if unset)");
}

static int ms_parse_opts(struct ct_mon_sampler *ct, int op, char *current_optarg) {
	char *endptr;
	unsigned long out;
	switch (op) {
	case 'w':
		out = strtoul(current_optarg, &endptr, 0);
		if (errno == ERANGE || *endptr != '\0' || current_optarg == endptr) {
			fprintf(stderr, "Invalid watch time '%s'\n",
				current_optarg);
			return -EINVAL;
		}
		ct->opts.watch_usec = out * 1000;
		break;
	case 'f':
		if ((strlen(current_optarg) == 3) &&
		    strncasecmp("csv", current_optarg, 3) == 0) {
			ct->opts.format = MS_CSV;
		} else {
			fprintf(stderr, "Invalid format '%s'\n",
				current_optarg);
			return -EINVAL;
		}
		break;
	case 'o':
		ct->opts.output = current_optarg;
		break;
	default:
		break;
	}
	return 0;
}

int main(int argc, char **argv) {
	int op, ret = EXIT_SUCCESS;
	struct ct_mon_sampler ct = {};

	while ((op = getopt(argc, argv, "hw:f:o:")) != -1) {
		switch (op) {
		default:
			ret = ms_parse_opts(&ct, op, optarg);
			if (ret != 0) {
				ms_usage(argv[0]);
				return ret;
			}
			break;
		case '?':
		case 'h':
			ms_usage(argv[0]);
			return EXIT_FAILURE;
		}
	}
	if (optind == argc) {
		fprintf(stderr, "No target path specified!\n");
		ms_usage(argv[0]);
		return 0;
	}

	struct stat st;
	if (stat(argv[optind], &st) != 0) {
		fprintf(stderr, "Could not stat %s\n", argv[optind]);
		return -ENOENT;
	}
	ct.opts.target_path = argv[optind];
	ct.target_mode = st.st_mode;
	if (S_ISDIR(ct.target_mode) && ct.opts.output == NULL) {
		fprintf(stderr, "Target is a directory, cannot use stdout as output!\n");
		return -ENOENT;
	}


	signal(SIGINT, signal_handler);
	running = ct.opts.watch_usec > 0;

	do {
		ret = ms_run(&ct);
		usleep(ct.opts.watch_usec);
	} while (running);

	if (ret != 0)
		fprintf(stderr, "Error while running: %d\n", ret);

	ms_cleanup(&ct);
	return -ret;
}
