/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011  Université de Bordeaux 1
 * Copyright (C) 2011  Centre National de la Recherche Scientifique
 * Copyright (C) 2011  Télécom-SudParis
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

#include <config.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>

#include <starpu.h>
#include <starpu_perfmodel.h>

#ifdef __MINGW32__
#include <windows.h>
#endif

#define PROGNAME "starpu_perfmodel_display"

static struct starpu_perfmodel model;

/* display all available models */
static int list = 0;
/* what kernel ? */
static char *symbol = NULL;
/* what parameter should be displayed ? (NULL = all) */
static char *parameter = NULL;
/* which architecture ? (NULL = all)*/
static char *arch = NULL;
/* should we display a specific footprint ? */
unsigned display_specific_footprint;
uint32_t specific_footprint;

static void usage(char **argv)
{
	fprintf(stderr, "Display a given perfmodel\n\n");
	fprintf(stderr, "Usage: %s [ options ]\n", PROGNAME);
        fprintf(stderr, "\n");
        fprintf(stderr, "One must specify either -l or -s\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "   -l                  display all available models\n");
        fprintf(stderr, "   -s <symbol>         specify the symbol\n");
        fprintf(stderr, "   -p <parameter>      specify the parameter (e.g. a, b, c, mean, stddev)\n");
        fprintf(stderr, "   -a <arch>           specify the architecture (e.g. cpu, cpu:k, cuda, gordon)\n");
	fprintf(stderr, "   -f <footprint>      display the history-based model for the specified footprint\n");
	fprintf(stderr, "   -h, --help          display this help and exit\n");
	fprintf(stderr, "   -v, --version       output version information and exit\n\n");
        fprintf(stderr, "Reports bugs to <"PACKAGE_BUGREPORT">.");
}

static void parse_args(int argc, char **argv)
{
	int c;

	static struct option long_options[] =
	{
		{"arch",      required_argument, NULL, 'a'},
		{"footprint", required_argument, NULL, 'f'},
		{"help",      no_argument,       NULL, 'h'},
		/* XXX Would be cleaner to set a flag */
		{"list",      no_argument,       NULL, 'l'},
		{"parameter", required_argument, NULL, 'p'},
		{"symbol",    required_argument, NULL, 's'},
		{"version",   no_argument,       NULL, 'v'},
		{0, 0, 0, 0}
	};

	int option_index;
	while ((c = getopt_long(argc, argv, "ls:p:a:f:h", long_options, &option_index)) != -1) {
	switch (c) {
                case 'l':
                        /* list all models */
                        list = 1;
                        break;

		case 's':
			/* symbol */
			symbol = optarg;
			break;

		case 'p':
			/* parameter (eg. a, b, c, mean, stddev) */
			parameter = optarg;
			break;

		case 'a':
			/* architecture (cpu, cuda, gordon) */
			arch = optarg;
			break;

		case 'f':
			/* footprint */
			display_specific_footprint = 1;
			sscanf(optarg, "%08x", &specific_footprint);
			break;

		case 'h':
			usage(argv);
			exit(EXIT_SUCCESS);

		case 'v':
			(void) fprintf(stdout, "%s %d.%d\n",
				PROGNAME, STARPU_MAJOR_VERSION,
				STARPU_MINOR_VERSION);
			exit(EXIT_SUCCESS);

		case '?':
		default:
			fprintf(stderr, "Unrecognized option: -%c\n", optopt);
	}
	}

	if (!symbol && !list)
	{
		fprintf(stderr, "Incorrect usage, aborting\n");
                usage(argv);
		exit(-1);
	}
}

static void display_history_based_perf_model(struct starpu_per_arch_perfmodel *per_arch_model)
{
	struct starpu_history_list *ptr;

	ptr = per_arch_model->list;

	if (!parameter && ptr)
		fprintf(stderr, "# hash\t\tsize\t\tmean\t\tdev\t\tn\n");

	while (ptr) {
		struct starpu_history_entry *entry = ptr->entry;
		if (!display_specific_footprint || (entry->footprint == specific_footprint))
		{
			if (!parameter)
			{	
				/* There isn't a parameter that is explicitely requested, so we display all parameters */
				printf("%08x\t%-15lu\t%-15le\t%-15le\t%u\n", entry->footprint,
					(unsigned long) entry->size, entry->mean, entry->deviation, entry->nsample);
			}
			else {
				/* only display the parameter that was specifically requested */
				if (strcmp(parameter, "mean") == 0) {
					printf("%-15le\n", entry->mean);
				}
		
				if (strcmp(parameter, "stddev") == 0) {
					printf("%-15le\n", entry->deviation);
					return;
				}
			}
		}

		ptr = ptr->next;
	}
}

static void display_perf_model(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, unsigned nimpl)
{
	struct starpu_per_arch_perfmodel *arch_model = &model->per_arch[arch][nimpl];
	char archname[32];

	if (arch_model->regression.nsample || arch_model->regression.valid || arch_model->regression.nl_valid || arch_model->list) {

		starpu_perfmodel_get_arch_name(arch, archname, 32, nimpl);
		fprintf(stderr, "performance model for %s\n", archname);
	}

	if (parameter == NULL)
	{
		/* no specific parameter was requested, so we display everything */
		if (arch_model->regression.nsample)
			fprintf(stderr, "\tRegression : #sample = %d\n",
				arch_model->regression.nsample);

		/* Only display the regression model if we could actually build a model */
		if (arch_model->regression.valid)
		{
			fprintf(stderr, "\tLinear: y = alpha size ^ beta\n");
			fprintf(stderr, "\t\talpha = %e\n", arch_model->regression.alpha);
			fprintf(stderr, "\t\tbeta = %e\n", arch_model->regression.beta);
		}
		else {
			//fprintf(stderr, "\tLinear model is INVALID\n");
		}
	
		if (arch_model->regression.nl_valid)
		{
			fprintf(stderr, "\tNon-Linear: y = a size ^b + c\n");
			fprintf(stderr, "\t\ta = %e\n", arch_model->regression.a);
			fprintf(stderr, "\t\tb = %e\n", arch_model->regression.b);
			fprintf(stderr, "\t\tc = %e\n", arch_model->regression.c);
		}
		else {
			//fprintf(stderr, "\tNon-Linear model is INVALID\n");
		}

		display_history_based_perf_model(arch_model);

#if 0
		char debugname[1024];
		starpu_perfmodel_debugfilepath(model, arch, debugname, 1024, nimpl);
		printf("\t debug file path : %s\n", debugname);
#endif
	}
	else {
		/* only display the parameter that was specifically requested */
		if (strcmp(parameter, "a") == 0) {
			printf("%e\n", arch_model->regression.a);
			return;
		}

		if (strcmp(parameter, "b") == 0) {
			printf("%e\n", arch_model->regression.b);
			return;
		}

		if (strcmp(parameter, "c") == 0) {
			printf("%e\n", arch_model->regression.c);
			return;
		}

		if (strcmp(parameter, "alpha") == 0) {
			printf("%e\n", arch_model->regression.alpha);
			return;
		}

		if (strcmp(parameter, "beta") == 0) {
			printf("%e\n", arch_model->regression.beta);
			return;
		}

		if (strcmp(parameter, "path-file-debug") == 0) {
			char debugname[256];
			starpu_perfmodel_debugfilepath(model, arch, debugname, 1024, nimpl);
			printf("%s\n", debugname);
			return;
		}

		if ((strcmp(parameter, "mean") == 0) || (strcmp(parameter, "stddev"))) {
			display_history_based_perf_model(arch_model);
			return;
		}

		/* TODO display if it's valid ? */

		fprintf(stderr, "Unknown parameter requested, aborting.\n");
		exit(-1);
	}
}

static void display_all_perf_models(struct starpu_perfmodel *model)
{
	if (arch == NULL)
	{
		/* display all architectures */
		unsigned archid;
		unsigned implid;
		for (archid = 0; archid < STARPU_NARCH_VARIATIONS; archid++) {
			for (implid = 0; implid < STARPU_MAXIMPLEMENTATIONS; implid++) { /* Display all codelets on each arch */
				display_perf_model(model, (enum starpu_perf_archtype) archid, implid);
			}
		}
	}
	else {
		if (strcmp(arch, "cpu") == 0) {
			unsigned implid;
			for (implid = 0; implid < STARPU_MAXIMPLEMENTATIONS; implid++)
				display_perf_model(model, STARPU_CPU_DEFAULT,implid); /* Display all codelets on cpu */
			return;
		}

		int k;
		if (sscanf(arch, "cpu:%d", &k) == 1)
		{
			/* For combined CPU workers */
			if ((k < 1) || (k > STARPU_MAXCPUS))
			{
				fprintf(stderr, "Invalid CPU size\n");
				exit(-1);
			}

			unsigned implid;
			for (implid = 0; implid < STARPU_MAXIMPLEMENTATIONS; implid++)
				display_perf_model(model, (enum starpu_perf_archtype) (STARPU_CPU_DEFAULT + k - 1), implid);
			return;
		}

		if (strcmp(arch, "cuda") == 0) {
			unsigned archid;
			unsigned implid;
			for (archid = STARPU_CUDA_DEFAULT; archid < STARPU_CUDA_DEFAULT + STARPU_MAXCUDADEVS; archid++) {
				for (implid = 0; implid <STARPU_MAXIMPLEMENTATIONS; implid ++) {
					char archname[32];
					starpu_perfmodel_get_arch_name((enum starpu_perf_archtype) archid, archname, 32, implid);
					fprintf(stderr, "performance model for %s\n", archname);
					display_perf_model(model, (enum starpu_perf_archtype) archid, implid);
				}
			}
			return;
		}

		/* There must be a cleaner way ! */
		int gpuid;
		int nmatched;
		nmatched = sscanf(arch, "cuda_%d", &gpuid);
		if (nmatched == 1)
		{
			unsigned archid = STARPU_CUDA_DEFAULT+ gpuid;
			unsigned implid;
			for (implid = 0; implid < STARPU_MAXIMPLEMENTATIONS; implid++)
				display_perf_model(model, (enum starpu_perf_archtype) archid, implid);
			return;
		}

		if (strcmp(arch, "gordon") == 0) {
			fprintf(stderr, "performance model for gordon\n");
			unsigned implid;
			for (implid = 0; implid < STARPU_MAXIMPLEMENTATIONS; implid++)
				display_perf_model(model, STARPU_GORDON_DEFAULT, implid);
			return;
		}

		fprintf(stderr, "Unknown architecture requested, aborting.\n");
		exit(-1);
	}
}

int main(int argc, char **argv)
{
//	assert(argc == 2);

#ifdef __MINGW32__
	WSADATA wsadata;
	WSAStartup(MAKEWORD(1,0), &wsadata);
#endif

	parse_args(argc, argv);

        if (list) {
                int ret = starpu_list_models(stdout);
                if (ret) {
                        fprintf(stderr, "The performance model directory is invalid\n");
                        return 1;
                }
        }
        else {
                int ret = starpu_load_history_debug(symbol, &model);
                if (ret == 1)
                        {
                                fprintf(stderr, "The performance model could not be loaded\n");
                                return 1;
                        }

                display_all_perf_models(&model);
        }

	return 0;
}
