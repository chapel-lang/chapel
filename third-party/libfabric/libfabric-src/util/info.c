/*
 * Copyright (c) 2013-2020 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#include <ofi_osd.h>

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>

static struct fi_info *hints;
static char *node, *port;
static int ver = 0;
static int list_providers = 0;
static int verbose = 0, env = 0;
static char *envstr;

/* options and matching help strings need to be kept in sync */

static const struct option longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"node", required_argument, NULL, 'n'},
	{"port", required_argument, NULL, 'P'},
	{"caps", required_argument, NULL, 'c'},
	{"mode", required_argument, NULL, 'm'},
	{"ep_type", required_argument, NULL, 't'},
	{"domain", required_argument, NULL, 'd'},
	{"fabric", required_argument, NULL, 'f'},
	{"addr_format", required_argument, NULL, 'a'},
	{"provider", required_argument, NULL, 'p'},
	{"env", no_argument, NULL, 'e'},
	{"getenv", required_argument, NULL, 'g'},
	{"list", no_argument, NULL, 'l'},
	{"verbose", no_argument, NULL, 'v'},
	{"version", no_argument, &ver, 1},
	{0,0,0,0}
};

static const char *help_strings[][2] = {
	{"", "\t\tdisplay this help and exit"},
	{"NAME", "\t\tnode name or address"},
	{"PNUM", "\t\tport number"},
	{"CAP1|CAP2..", "\tone or more capabilities: FI_MSG|FI_RMA..."},
	{"MOD1|MOD2..", "\tone or more modes, default all modes"},
	{"EPTYPE", "\t\tspecify single endpoint type: FI_EP_MSG, FI_EP_DGRAM..."},
	{"DOMAIN", "\t\tspecify the domain name"},
	{"FABRIC", "\t\tspecify the fabric name"},
	{"FMT", "\t\tspecify accepted address format: FI_FORMAT_UNSPEC, FI_SOCKADDR..."},
	{"PROV", "\t\tspecify provider explicitly"},
	{"", "\t\tprint libfabric environment variables"},
	{"", "\t\tprint libfabric environment variables with substr"},
	{"", "\t\tlist available libfabric providers"},
	{"", "\t\tverbose output"},
	{"", "\t\tprint version info and exit"},
	{"", ""}
};

static void usage(void)
{
	int i = 0;
	const struct option *ptr = longopts;

	for (; ptr->name != NULL; ++i, ptr = &longopts[i])
		if (ptr->has_arg == required_argument)
			printf("  -%c, --%s=%s%s\n", ptr->val, ptr->name,
				help_strings[i][0], help_strings[i][1]);
		else if (ptr->flag != NULL)
			printf("  --%s\t%s\n", ptr->name,
				help_strings[i][1]);
		else
			printf("  -%c, --%s\t%s\n", ptr->val, ptr->name,
				help_strings[i][1]);
}

#define ORCASE(SYM)                                                            \
	do {                                                                   \
		if (strcasecmp(#SYM, inputstr) == 0) {                         \
			*value = SYM;                                          \
			return 0;                                              \
		}                                                              \
	} while (0)

static int str2cap(char *inputstr, uint64_t *value)
{
	ORCASE(FI_MSG);
	ORCASE(FI_RMA);
	ORCASE(FI_TAGGED);
	ORCASE(FI_ATOMIC);
	ORCASE(FI_MULTICAST);

	ORCASE(FI_READ);
	ORCASE(FI_WRITE);
	ORCASE(FI_RECV);
	ORCASE(FI_SEND);
	ORCASE(FI_REMOTE_READ);
	ORCASE(FI_REMOTE_WRITE);

	ORCASE(FI_MULTI_RECV);
	ORCASE(FI_REMOTE_CQ_DATA);
	ORCASE(FI_MORE);
	ORCASE(FI_PEEK);
	ORCASE(FI_TRIGGER);
	ORCASE(FI_FENCE);

	ORCASE(FI_SOURCE_ERR);
	ORCASE(FI_LOCAL_COMM);
	ORCASE(FI_REMOTE_COMM);
	ORCASE(FI_SHARED_AV);
	ORCASE(FI_RMA_EVENT);
	ORCASE(FI_SOURCE);
	ORCASE(FI_NAMED_RX_CTX);
	ORCASE(FI_DIRECTED_RECV);
	ORCASE(FI_HMEM);

	fprintf(stderr, "error: Unrecognized capability: %s\n", inputstr);

	return -EINVAL;
}

static int str2mode(char *inputstr, uint64_t *value)
{
	ORCASE(FI_CONTEXT);
	ORCASE(FI_MSG_PREFIX);
	ORCASE(FI_ASYNC_IOV);
	ORCASE(FI_RX_CQ_DATA);
	ORCASE(FI_LOCAL_MR);
	ORCASE(FI_NOTIFY_FLAGS_ONLY);
	ORCASE(FI_RESTRICTED_COMP);
	ORCASE(FI_CONTEXT2);

	fprintf(stderr, "error: Unrecognized mode: %s\n", inputstr);

	return -EINVAL;
}

static int str2ep_type(char *inputstr, enum fi_ep_type *value)
{
	ORCASE(FI_EP_UNSPEC);
	ORCASE(FI_EP_MSG);
	ORCASE(FI_EP_DGRAM);
	ORCASE(FI_EP_RDM);
	ORCASE(FI_EP_SOCK_STREAM);
	ORCASE(FI_EP_SOCK_DGRAM);

	fprintf(stderr, "error: Unrecognized endpoint type: %s\n", inputstr);

	return -EINVAL;
}

static int str2addr_format(char *inputstr, uint32_t *value)
{
	ORCASE(FI_FORMAT_UNSPEC);
	ORCASE(FI_SOCKADDR);
	ORCASE(FI_SOCKADDR_IN);
	ORCASE(FI_SOCKADDR_IN6);
	ORCASE(FI_SOCKADDR_IB);
	ORCASE(FI_ADDR_PSMX);
	ORCASE(FI_ADDR_GNI);
	ORCASE(FI_ADDR_BGQ);
	ORCASE(FI_ADDR_MLX);
	ORCASE(FI_ADDR_STR);
	ORCASE(FI_ADDR_PSMX2);
	ORCASE(FI_ADDR_EFA);

	fprintf(stderr, "error: Unrecognized address format: %s\n", inputstr);

	return -EINVAL;
}

static int tokparse(char *caps,
		    int (*str2flag)(char *, uint64_t *),
		    uint64_t *flags)
{
	uint64_t value;
	char *tok;
	int ret;

	for (tok = strtok(caps, "|"); tok != NULL; tok = strtok(NULL, "|")) {
		ret = str2flag(tok, &value);
		if (ret)
			return ret;

		*flags |= value;
	}

	return 0;
}

static const char *param_type(enum fi_param_type type)
{
	switch (type) {
	case FI_PARAM_STRING:
		return "String";
	case FI_PARAM_INT:
		return "Integer";
	case FI_PARAM_SIZE_T:
		return "size_t";
	case FI_PARAM_BOOL:
		return "Boolean (0/1, on/off, true/false, yes/no)";
	default:
		return "Unknown";
	}
}

static int print_vars(void)
{
	int ret, count, i;
	struct fi_param *params;
	char delim;

	ret = fi_getparams(&params, &count);
	if (ret)
		return ret;

	for (i = 0; i < count; ++i) {
		if (envstr && !strcasestr(params[i].name, envstr))
			continue;

		printf("# %s: %s\n", params[i].name, param_type(params[i].type));
		printf("# %s\n", params[i].help_string);

		if (params[i].value) {
			delim = strchr(params[i].value, ' ') ? '"' : '\0';
			printf("%s=%c%s%c\n", params[i].name, delim,
			       params[i].value, delim);
		}

		printf("\n");
	}

	fi_freeparams(params);
	return ret;
}

static int print_providers(struct fi_info *info)
{
	struct fi_info *cur;
	for (cur = info; cur; cur = cur->next) {
		printf("%s:\n", cur->fabric_attr->prov_name);
		printf("    version: %d.%d\n",
			FI_MAJOR(cur->fabric_attr->prov_version),
			FI_MINOR(cur->fabric_attr->prov_version));
	}
	return EXIT_SUCCESS;
}

static int print_short_info(struct fi_info *info)
{
	struct fi_info *cur;
	for (cur = info; cur; cur = cur->next) {
		printf("provider: %s\n", cur->fabric_attr->prov_name);
		printf("    fabric: %s\n", cur->fabric_attr->name),
		printf("    domain: %s\n", cur->domain_attr->name),
		printf("    version: %d.%d\n", FI_MAJOR(cur->fabric_attr->prov_version),
			FI_MINOR(cur->fabric_attr->prov_version));
		if (!list_providers) {
			printf("    type: %s\n", fi_tostr(&cur->ep_attr->type, FI_TYPE_EP_TYPE));
			printf("    protocol: %s\n", fi_tostr(&cur->ep_attr->protocol, FI_TYPE_PROTOCOL));
		}
	}
	return EXIT_SUCCESS;
}

static int print_long_info(struct fi_info *info)
{
	struct fi_info *cur;
	for (cur = info; cur; cur = cur->next) {
		printf("---\n");
		printf("%s", fi_tostr(cur, FI_TYPE_INFO));
	}
	return EXIT_SUCCESS;
}

static int run(struct fi_info *hints, char *node, char *port)
{
	struct fi_info *info;
	int ret;
	uint64_t flags;

	flags = list_providers ? FI_PROV_ATTR_ONLY : 0;
	ret = fi_getinfo(FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION),
			node, port, flags, hints, &info);
	if (ret) {
		fprintf(stderr, "fi_getinfo: %d\n", ret);
		return ret;
	}

	if (env)
		ret = print_vars();
	else if (verbose)
		ret = print_long_info(info);
	else if (list_providers)
		ret = print_providers(info);
	else
		ret = print_short_info(info);

	fi_freeinfo(info);
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret, option_index;
	int use_hints = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	hints->mode = ~0;
	hints->domain_attr->mode = ~0;
	hints->domain_attr->mr_mode = ~(FI_MR_BASIC | FI_MR_SCALABLE);

	while ((op = getopt_long(argc, argv, "n:P:c:m:t:a:p:d:f:eg:lhv", longopts,
				 &option_index)) != -1) {
		switch (op) {
		case 0:
			/* there is no short variant only for --version */
			if (ver) {
				printf("%s: %s\n", argv[0], PACKAGE_VERSION);
				printf("libfabric: %s\n", fi_tostr("1", FI_TYPE_VERSION));
				printf("libfabric api: %d.%d\n",
				       FI_MAJOR_VERSION, FI_MINOR_VERSION);
				return EXIT_SUCCESS;
			}
			goto print_help;
		case 'n':
			node = optarg;
			break;
		case 'P':
			port = optarg;
			break;
		case 'c':
			ret = tokparse(optarg, str2cap, &hints->caps);
			if (ret)
				goto out;

			use_hints = 1;
			break;
		case 'm':
			hints->mode = 0;
			ret = tokparse(optarg, str2mode, &hints->mode);
			if (ret)
				goto out;

			use_hints = 1;
			break;
		case 't':
			ret = str2ep_type(optarg, &hints->ep_attr->type);
			if (ret)
				goto out;

			use_hints = 1;
			break;
		case 'a':
			ret = str2addr_format(optarg, &hints->addr_format);
			if (ret)
				goto out;

			use_hints = 1;
			break;
		case 'p':
			free(hints->fabric_attr->prov_name);
			hints->fabric_attr->prov_name = strdup(optarg);
			use_hints = 1;
			break;
		case 'd':
			hints->domain_attr->name = strdup(optarg);
			use_hints = 1;
			break;
		case 'f':
			hints->fabric_attr->name = strdup(optarg);
			use_hints = 1;
			break;
		case 'g':
			envstr = optarg;
			/* fall through */
		case 'e':
			env = 1;
			break;
		case 'l':
			list_providers = 1;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
		default:
print_help:
			printf("Usage: %s\n", argv[0]);
			usage();
			return EXIT_FAILURE;
		}
	}

	ret = run(use_hints ? hints : NULL, node, port);

out:
	fi_freeinfo(hints);
	return -ret;
}
