/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>

#include <limits.h>
#include <shared.h>

#include "fabtest.h"

static int persistent = 1;
static int do_fork = 0;

//static struct timespec start, end;

static struct ft_series *series;
static int test_start_index, test_end_index = INT_MAX;
struct ft_info test_info;
struct fi_info *fabric_info;
struct ft_xcontrol ft_rx_ctrl, ft_tx_ctrl;
struct ft_mr_control ft_mr_ctrl;
struct ft_atomic_control ft_atom_ctrl;
struct ft_control ft_ctrl;

size_t recv_size, send_size;

enum {
	FT_SUCCESS,
	FT_SKIP,
	FT_ENODATA,
	FT_ENOSYS,
	FT_ERROR,
	FT_EIO,
	FT_MAX_RESULT
};

static int results[FT_MAX_RESULT];
static char *filename = NULL;
static char *domain_name = NULL;

static int ft_nullstr(char *str)
{
	return (!str || str[0] == '\0');
}

static char *ft_strptr(char *str)
{
	return ft_nullstr(str) ? NULL : str;
}

static char *ft_test_type_str(enum ft_test_type enum_str)
{
	switch (enum_str) {
	case FT_TEST_LATENCY:
		return "latency";
	case FT_TEST_BANDWIDTH:
		return "bandwidth";
	case FT_TEST_UNIT:
		return "unit";
	default:
		return "test_unspec";
	}
}

static char *ft_class_func_str(enum ft_class_function enum_str)
{
	switch (enum_str) {
	case FT_FUNC_SEND:
		return (test_info.test_class & FI_TAGGED) ? "tsend" : "send";
	case FT_FUNC_SENDV:
		return (test_info.test_class & FI_TAGGED) ? "tsendv" : "sendv";
	case FT_FUNC_SENDMSG:
		return (test_info.test_class & FI_TAGGED) ? "tsendmsg" : "sendmsg";
	case FT_FUNC_INJECT:
		return (test_info.test_class & FI_TAGGED) ? "tinject" : "inject";
	case FT_FUNC_INJECTDATA:
		return (test_info.test_class & FI_TAGGED) ? "tinjectdata" : "injectdata";
	case FT_FUNC_SENDDATA:
		return (test_info.test_class & FI_TAGGED) ? "tsenddata" : "senddata";
	case FT_FUNC_READ:
		return "read";
	case FT_FUNC_READV:
		return "readv";
	case FT_FUNC_READMSG:
		return "readmsg";
	case FT_FUNC_WRITE:
		return "write";
	case FT_FUNC_WRITEV:
		return "writev";
	case FT_FUNC_WRITEMSG:
		return "writemsg";
	case FT_FUNC_INJECT_WRITE:
		return "inject_write";
	case FT_FUNC_WRITEDATA:
		return "writedata";
	case FT_FUNC_INJECT_WRITEDATA:
		return "inject_writedata";
	case FT_FUNC_ATOMIC:
		return "atomic";
	case FT_FUNC_ATOMICV:
		return "atomicv";
	case FT_FUNC_ATOMICMSG:
		return "atomic_msg";
	case FT_FUNC_INJECT_ATOMIC:
		return "inject_atomic";
	case FT_FUNC_FETCH_ATOMIC:
		return "fetch_atomic";
	case FT_FUNC_FETCH_ATOMICV:
		return "fetch_atomicv";
	case FT_FUNC_FETCH_ATOMICMSG:
		return "fetch_atomicmsg";
	case FT_FUNC_COMPARE_ATOMIC:
		return "compare_atomic";
	case FT_FUNC_COMPARE_ATOMICV:
		return "compare_atomicv";
	case FT_FUNC_COMPARE_ATOMICMSG:
		return "compare_atomicmsg";
	default:
		return "func_unspec";
	}
}

static char *ft_wait_obj_str(enum fi_wait_obj enum_str)
{
	switch (enum_str) {
	case FI_WAIT_NONE:
		return "wait_none";
	case FI_WAIT_UNSPEC:
		return "wait_unspec";
	case FI_WAIT_SET:
		return "wait_set";
	case FI_WAIT_FD:
		return "wait_fd";
	case FI_WAIT_MUTEX_COND:
		return "wait_mutex_cond";
	default:
		return "";
	}
}

static void ft_print_comp_flag(uint64_t bind, uint64_t op)
{
	printf("(bind-%s, op-%s)", (bind & FI_SELECTIVE_COMPLETION) ?
		"FI_SELECTIVE_COMPLETION" : "NONE",
		op ? fi_tostr(&op, FI_TYPE_OP_FLAGS) : "NONE");
}

static void ft_print_comp(struct ft_info *test)
{
	if (ft_use_comp_cq(test->comp_type))
		printf("comp_queue -- tx ");
	if (ft_use_comp_cntr(test->comp_type))
		printf("comp_cntr -- tx ");

	ft_print_comp_flag(test->tx_cq_bind_flags, test->tx_op_flags);
	printf(", rx: ");
	ft_print_comp_flag(test->rx_cq_bind_flags, test->rx_op_flags);
	printf(", ");
}

static void ft_show_test_info(void)
{
	printf("[%s,", test_info.prov_name);
	printf(" %s,", ft_test_type_str(test_info.test_type));
	if (test_info.test_class & FI_ATOMIC) {
		printf(" %s ", ft_class_func_str(test_info.class_function));
		printf("(%s, ", fi_tostr(&test_info.op, FI_TYPE_ATOMIC_OP));
		printf("%s)--", fi_tostr(&test_info.datatype, FI_TYPE_ATOMIC_TYPE));
	} else {
		printf(" %s--", ft_class_func_str(test_info.class_function));
	}
	printf("%s,", fi_tostr(&test_info.msg_flags, FI_TYPE_OP_FLAGS));
	printf(" %s,", fi_tostr(&test_info.ep_type, FI_TYPE_EP_TYPE));
	printf(" %s,", fi_tostr(&test_info.av_type, FI_TYPE_AV_TYPE));
	printf(" eq_%s,", ft_wait_obj_str(test_info.eq_wait_obj));
	printf(" cq_%s,", ft_wait_obj_str(test_info.cq_wait_obj));
	printf(" cntr_%s, ", ft_wait_obj_str(test_info.cq_wait_obj));
	ft_print_comp(&test_info);
	printf(" %s,", fi_tostr(&test_info.progress, FI_TYPE_PROGRESS));
	printf(" %s (only hints),", fi_tostr(&test_info.threading, FI_TYPE_THREADING));
	printf(" [%s],", fi_tostr(&test_info.mr_mode, FI_TYPE_MR_MODE));
	printf(" [%s],", fi_tostr(&test_info.mode, FI_TYPE_MODE));
	printf(" [%s]]\n", fi_tostr(&test_info.caps, FI_TYPE_CAPS));
}

static int ft_check_info(struct fi_info *hints, struct fi_info *info)
{
	if (info->mode & ~hints->mode) {
		fprintf(stderr, "fi_getinfo unsupported mode returned:\n");
		fprintf(stderr, "hints mode: %s\n",
			fi_tostr(&hints->mode, FI_TYPE_MODE));
		fprintf(stderr, "info mode: %s\n",
			fi_tostr(&info->mode, FI_TYPE_MODE));
		return -FI_EINVAL;
	}
	if (hints->caps != (hints->caps & info->caps)) {
		fprintf(stderr, "fi_getinfo missing caps:\n");
		fprintf(stderr, "hints caps: %s\n",
			fi_tostr(&hints->caps, FI_TYPE_CAPS));
		fprintf(stderr, "info caps: %s\n",
			fi_tostr(&info->caps, FI_TYPE_CAPS));
		return -FI_EINVAL;
	}

	return 0;
}

static void ft_fw_convert_info(struct fi_info *info, struct ft_info *test_info)
{
	info->caps = test_info->caps;

	info->mode = test_info->mode;

	info->domain_attr->mr_mode = test_info->mr_mode;
	info->domain_attr->av_type = test_info->av_type;
	info->domain_attr->data_progress = test_info->progress;
	info->domain_attr->threading = test_info->threading;

	info->ep_attr->type = test_info->ep_type;
	info->ep_attr->protocol = test_info->protocol;
	info->ep_attr->protocol_version = test_info->protocol_version;

	if (!ft_nullstr(test_info->prov_name)) {
		info->fabric_attr->prov_name = strndup(test_info->prov_name,
					sizeof test_info->prov_name - 1);
	}
	if (!ft_nullstr(test_info->fabric_name)) {
		info->fabric_attr->name = strndup(test_info->fabric_name,
					sizeof test_info->fabric_name - 1);
	}

	info->tx_attr->op_flags = test_info->tx_op_flags;
	info->rx_attr->op_flags = test_info->rx_op_flags;

	if (is_data_func(test_info->class_function) ||
	    (is_msg_func(test_info->class_function) &&
	     test_info->msg_flags & FI_REMOTE_CQ_DATA))
		info->domain_attr->cq_data_size = 4;
}

static void ft_fw_update_info(struct ft_info *test_info, struct fi_info *info)
{
	if (info->ep_attr) {
		test_info->protocol = info->ep_attr->protocol;
		test_info->protocol_version = info->ep_attr->protocol_version;
	}

	if (info->fabric_attr) {
		if (info->fabric_attr->prov_name) {
			strncpy(test_info->prov_name, info->fabric_attr->prov_name,
				sizeof test_info->prov_name - 1);
		}
		if (info->fabric_attr->name) {
			strncpy(test_info->fabric_name, info->fabric_attr->name,
				sizeof test_info->fabric_name - 1);
		}
	}

	if (info->domain_attr) {
		test_info->progress = info->domain_attr->data_progress;
		test_info->threading = info->domain_attr->threading;
		test_info->mr_mode = info->domain_attr->mr_mode;
	}

	test_info->mode = info->mode;
}

static int ft_fw_result_index(int fi_errno)
{
	switch (fi_errno) {
	case 0:
		return FT_SUCCESS;
	case -FI_ENODATA:
		return FT_ENODATA;
	case -FI_ENOSYS:
		return FT_ENOSYS;
	case -FI_EIO:
		return FT_EIO;
	case -FT_SKIP:
		return FT_SKIP;
	default:
		return FT_ERROR;
	}
}

static int ft_recv_test_info(void)
{
	int ret;

	ret = ft_sock_recv(sock, &test_info, sizeof test_info);
	if (ret)
		return ret;

	test_info.node[sizeof(test_info.node) - 1] = '\0';
	test_info.service[sizeof(test_info.service) - 1] = '\0';
	test_info.prov_name[sizeof(test_info.prov_name) - 1] = '\0';
	test_info.fabric_name[sizeof(test_info.fabric_name) - 1] = '\0';
	return 0;
}

static int ft_send_result(int err, struct fi_info *info)
{
	int ret;
	ret = ft_sock_send(sock, &err, sizeof err);
	if (ret) {
		FT_PRINTERR("ft_sock_send", ret);
		return ret;
	}
	if (err) {
		printf("Ending test %d, result: %s\n", test_info.test_index,
			fi_strerror(-err));
		return err;
	}

	return 0;
}

static int ft_recv_result(struct fi_info *info)
{
	int ret, err = 0;
	ret = ft_sock_recv(sock, &err, sizeof err);
	if (ret) {
		FT_PRINTERR("ft_sock_recv", ret);
		return ret;
	}
	if (err) {
		printf("Ending test %d, result: %s\n", test_info.test_index,
			fi_strerror(-err));
	}

	return err;
}

static int ft_server_setup(struct fi_info *hints, struct fi_info *info)
{
	int ret = 0;

	hints = fi_allocinfo();
	if (!hints) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ft_fw_convert_info(hints, &test_info);
	if (domain_name)
		hints->domain_attr->name = domain_name;

	ret = fi_getinfo(FT_FIVERSION, ft_strptr(test_info.node),
			 ft_strptr(test_info.service), FI_SOURCE, hints, &info);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		goto err;
	}

	fabric_info = info;

	ret = ft_check_info(hints, fabric_info);
	if (ret)
		goto err;

	ret = ft_open_res();
	if (ret)
		goto err;

	ft_fw_update_info(&test_info, fabric_info);

	return 0;
err:
	ft_send_result(ret, info);
	return ret;
}

static int ft_server_child()
{
	struct fi_info *hints = NULL;
	struct fi_info *info  = NULL;
	int ret, result;

	printf("Starting test %d:\n", test_info.test_index);

	ret = ft_server_setup(hints, info);
	if (ret)
		return ret;

	ret = ft_send_result(0, info);
	if (ret)
		return ret;

	ret = ft_sock_send(sock, &test_info, sizeof test_info);
	if (ret) {
		FT_PRINTERR("ft_sock_send", ret);
		return ret;
	}

	ret = ft_recv_result(info);
	if (ret)
		return ret;

	ret = ft_init_test();
	if (ret)
		return ret;

	result = ft_run_test();

	ret = ft_sock_send(sock, &result, sizeof result);
	if (result) {
		FT_PRINTERR("ft_run_test", result);
	}

	fi_freeinfo(hints);
	ft_cleanup();

	if (ret) {
		FT_PRINTERR("ft_sock_send", ret);
		return ret;
	}

	printf("Ending test %d, result: %s\n", test_info.test_index,
		fi_strerror(-result));

	return result;
}

static int ft_fw_server(void)
{
	int ret;
	pid_t pid;

	do {
		ret = ft_recv_test_info();
		if (ret) {
			if (ret == -FI_ENOTCONN)
				ret = 0;
			break;
		}

		if (do_fork) {
			pid = fork();
			if (!pid) {
				ret = ft_server_child();
				_exit(-ret);
			} else {
				waitpid(pid, &ret, 0);
				ret = WEXITSTATUS(ret);
			}
		} else {
			ret = ft_server_child();
		}

		results[ft_fw_result_index(ret)]++;

	} while (!ret || ret == -FI_EIO || ret == -FI_ENODATA || ret == -FT_SKIP);

	return ret;
}
static int ft_client_setup(struct fi_info *hints, struct fi_info *info)
{
	int ret;
	ret = ft_recv_test_info();
	if (ret)
		goto err;

	hints = fi_allocinfo();
	if (!hints) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = ft_getsrcaddr(opts.src_addr, opts.src_port, hints);
	if (ret)
		goto err;

	ft_fw_convert_info(hints, &test_info);
	if (domain_name)
		hints->domain_attr->name = domain_name;

	ft_show_test_info();

	ret = fi_getinfo(FT_FIVERSION, ft_strptr(test_info.node),
			 ft_strptr(test_info.service), 0, hints, &info);
	if (ret)
		goto err;

	fabric_info = info;

	ret = ft_check_info(hints, fabric_info);
	if (ret)
		goto err;

	ft_fw_update_info(&test_info, fabric_info);

	ret = ft_open_res();

	return 0;

err:
	ft_send_result(ret, info);
	return ret;
}
static int ft_client_child(void)
{
	struct fi_info *hints = NULL;
	struct fi_info  *info = NULL;
	int ret, result, sresult = 0;
	result = -FI_ENODATA;

	ret = ft_sock_send(sock, &test_info, sizeof test_info);
	if (ret)
		goto err;

	printf("Starting test %d / %d:\n", test_info.test_index,
		series->test_count);

	ret = ft_recv_result(info);
	if (ret)
		return ret;

	ret = ft_client_setup(hints, info);
	if (ret)
		return ret;

	ret = ft_send_result(0, info);
	if (ret)
		return ret;

	result = ft_init_test();
	if (result)
		return result;

	result = ft_run_test();
	ret = ft_sock_recv(sock, &sresult, sizeof sresult);
	if (result && result != -FI_EIO) {
		FT_PRINTERR("ft_run_test", result);
		fprintf(stderr, "Node: %s\nService: %s \n",
			test_info.node, test_info.service);
		fprintf(stderr, "%s\n", fi_tostr(hints, FI_TYPE_INFO));
		ret = -FI_EOTHER;
	} else if (ret) {
		FT_PRINTERR("ft_sock_recv", ret);
		result = ret;
		ret = -FI_EOTHER;
	} else if (sresult) {
		result = sresult;
		if (sresult != -FI_EIO)
			ret = -FI_EOTHER;
	}

	printf("Ending test %d / %d, result: %s\n", test_info.test_index,
		series->test_count, fi_strerror(-result));

	fi_freeinfo(hints);
	ft_cleanup();

	return result;

err:
	ft_send_result(ret, info);
	return result;
}

static int ft_fw_client(void)
{
	int result;
	pid_t pid;

	for (fts_start(series, test_start_index);
	     !fts_end(series, test_end_index);
	     fts_next(series)) {

		fts_cur_info(series, &test_info);
		if (!fts_info_is_valid()) {
			printf("Skipping test %d (invalid):\n", test_info.test_index);
			ft_show_test_info();
			results[FT_SKIP]++;
			continue;
		}

		if (do_fork) {
			pid = fork();
			if (!pid) {
				result = ft_client_child();
				_exit(-result);
			} else {
				waitpid(pid, &result, 0);
				result = WEXITSTATUS(result);
			}
		} else {
			result = ft_client_child();
		}

		results[ft_fw_result_index(result)]++;
	}
	return 0;
}

static void ft_fw_show_results(void)
{
	printf("Success: %d\n", results[FT_SUCCESS]);
	printf("Skipped: %d\n", results[FT_SKIP]);
	printf("ENODATA: %d\n", results[FT_ENODATA]);
	printf("ENOSYS : %d\n", results[FT_ENOSYS]);
	printf("EIO    : %d\n", results[FT_EIO]);
	printf("ERROR  : %d\n", results[FT_ERROR]);
}

static void ft_fw_usage(char *program)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  %s [OPTIONS] \t\t\tstart server\n", program);
	fprintf(stderr, "  %s [OPTIONS] -u config_file <server_node> \tconnect to server\n", program);
	fprintf(stderr, "\nOptions:\n");
	FT_PRINT_OPTS_USAGE("-q <service_port>", "Management port for test");
	FT_PRINT_OPTS_USAGE("-h", "display this help output");
	fprintf(stderr, "\nServer only options:\n");
	FT_PRINT_OPTS_USAGE("-x", "exit after test run");
	fprintf(stderr, "\nClient only options:\n");
	FT_PRINT_OPTS_USAGE("-u <test_config_file>", "test configuration file ");
	FT_PRINT_OPTS_USAGE("-y <start_test_index>", "");
	FT_PRINT_OPTS_USAGE("-z <end_test_index>", "");
	FT_PRINT_OPTS_USAGE("-s <address>", "source address");
	FT_PRINT_OPTS_USAGE("-B <src_port>", "non default source port number");
	FT_PRINT_OPTS_USAGE("-P <dst_port>", "non default destination port number "
		"(config file service parameter will override this)");
	FT_PRINT_OPTS_USAGE("-d <domain>", "domain name");
}

void ft_free()
{
	if (filename)
		free(filename);
}

int main(int argc, char **argv)
{
	char *service = "2710";
	opts = INIT_OPTS;
	int ret, op;

	while ((op = getopt(argc, argv, "u:q:xy:z:hfd:" ADDR_OPTS)) != -1) {
		switch (op) {
		case 'u':
			filename = strdup(optarg);
			break;
		case 'q':
			service = optarg;
			break;
		case 'x':
			persistent = 0;
			break;
		case 'y':
			test_start_index = atoi(optarg);
			break;
		case 'z':
			test_end_index = atoi(optarg);
			break;
		case 'f':
			do_fork = 1;
			break;
		case 'd':
			domain_name = strdup(optarg);
			break;
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			break;
		case '?':
		case 'h':
			ft_fw_usage(argv[0]);
			ft_free();
			exit(1);
		}
	}

	if (optind < argc - 1) {
		ft_fw_usage(argv[0]);
		ft_free();
		exit(1);
	}

	opts.dst_addr = (optind == argc - 1) ? argv[optind] : NULL;
	if (opts.dst_addr) {
		if (!opts.dst_port)
			opts.dst_port = default_port;
		if (!filename) {
			ft_fw_usage(argv[0]);
			ft_free();
			exit(1);
		}
		series = fts_load(filename);
		if (!series) {
			ft_free();
			exit(1);
		}

		ret = ft_sock_connect(opts.dst_addr, service);
		if (ret)
			goto out;

		ret = ft_fw_client();
		if (ret)
			FT_PRINTERR("ft_fw_client", ret);
		ft_sock_shutdown(sock);
	} else {
		ret = ft_sock_listen(opts.src_addr, service);
		if (ret)
			goto out;

		do {
			ret = ft_sock_accept();
			if (ret)
				goto out;

			ret = ft_fw_server();
			if (ret)
				FT_PRINTERR("ft_fw_server", ret);
			ft_sock_shutdown(sock);
		} while (persistent);
	}

	ft_fw_show_results();
out:
	if (opts.dst_addr)
		fts_close(series);
	ft_free();

	if (results[FT_EIO])
		ret = -FI_EIO;
	else if (results[FT_ENOSYS])
		ret = -FI_ENOSYS;
	else if (results[FT_ENODATA])
		ret = -FI_ENODATA;

	return ft_exit_code(ret);
}
