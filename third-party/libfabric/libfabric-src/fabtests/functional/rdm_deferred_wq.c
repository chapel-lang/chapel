/*
 * Copyright (c) 2017 Intel Corporation.  All rights reserved.
 * Copyright (c) 2017, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under the BSD license
 * below:
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

#include <rdma/fi_errno.h>
#include <rdma/fi_trigger.h>

#include <shared.h>

struct fi_context2 trig_ctx1, trig_ctx2;
static char *welcome_text = "Hello from Client!";
static char *write_text = "This is a successful trigger";
struct fi_deferred_work work;
struct fid_cntr *test_cntr;
uint64_t n_trig, rx_exp;
static char *result_buf, *compare_buf;
static struct fid_mr *mr_result, *mr_compare;
int use_alias = 0;
struct fid_ep *trig_ep;
enum fi_op_type tested_op;

static void format_simple_msg(struct fi_msg *msg, struct iovec *iov, void *src,
			      size_t size, void *ctx)
{
	msg->context = ctx;
	msg->desc = mr_desc;
	msg->iov_count = 1;
	msg->addr = remote_fi_addr;
	msg->data = 0;

	iov->iov_base = src;
	iov->iov_len = size;
	msg->msg_iov = iov;
}

static void format_simple_msg_tagged(struct fi_msg_tagged *msg, struct iovec *iov,
			      void *src, size_t size, void *ctx,
			      uint64_t tag)
{
	msg->context = ctx;
	msg->desc = mr_desc;
	msg->iov_count = 1;
	msg->addr = remote_fi_addr;
	msg->data = 0;
	msg->tag = tag;

	iov->iov_base = src;
	iov->iov_len = size;
	msg->msg_iov = iov;
}

static void format_simple_msg_rma(struct fi_msg_rma *msg, struct iovec *iov,
			   struct fi_rma_iov *rma_iov, void *src,
			   size_t size, void *ctx)
{
	msg->context = ctx;
	msg->desc = mr_desc;
	msg->iov_count = 1;
	msg->addr = remote_fi_addr;
	msg->rma_iov_count = 1;

	iov->iov_base = src;
	iov->iov_len = size;
	msg->msg_iov = iov;

	rma_iov->addr = 0;
	rma_iov->key = FT_MR_KEY;
	rma_iov->len = size;
	msg->rma_iov = rma_iov;
}

static void format_simple_msg_atomic(struct fi_msg_atomic *msg, struct fi_ioc *iov,
			      struct fi_rma_ioc *rma_iov, void *src, size_t size,
			      void *ctx, enum fi_datatype datatype,
			      enum fi_op op)
{
	msg->context = ctx;
	msg->desc = mr_desc;
	msg->iov_count = 1;
	msg->rma_iov_count = 1;
	msg->addr = remote_fi_addr;
	msg->data = 0;
	msg->datatype = datatype;
	msg->op = op;
	msg->data = 0;

	iov->addr = src;
	iov->count = size;
	msg->msg_iov = iov;

	rma_iov->addr = 0;
	rma_iov->count = size;
	rma_iov->key = FT_MR_KEY;
	msg->rma_iov = rma_iov;
}

static void format_simple_msg_fetch(struct fi_msg_fetch *msg, struct fi_ioc *iov,
				    void *src, size_t size)
{
	msg->desc = fi_mr_desc(mr_result);

	iov->addr = src;
	iov->count = size;

	msg->iov_count = 1;
	msg->msg_iov = iov;
}

static void format_simple_msg_compare(struct fi_msg_compare *msg, struct fi_ioc *iov,
				      void *src, size_t size)
{
	msg->desc = fi_mr_desc(mr_compare);

	iov->addr = src;
	iov->count = size;

	msg->iov_count = 1;
	msg->msg_iov = iov;
}

static int check_data()
{
	int ret, i;
	char c = ~0;

	switch (tested_op) {
	case FI_OP_COMPARE_ATOMIC:
	case FI_OP_FETCH_ATOMIC:
		if (strncmp(result_buf, welcome_text, strlen(welcome_text))) {
			printf("Data mismatch in fetch buffer...");
			return 1;
		}
		/* fall through */
	case FI_OP_ATOMIC:
		for (i = 0; i < strlen(welcome_text); i++) {
			if (rx_buf[i] != c) {
				printf("Data mismatch found at byte %d...", i);
				return 1;
			}
		}
		break;
	case FI_OP_CNTR_SET:
	case FI_OP_CNTR_ADD:
		ret  = fi_cntr_wait(test_cntr, 10, -1);
		if (ret)
			return ret;
		break;
	case FI_OP_READ:
	case FI_OP_WRITE:
		if (strncmp(result_buf, write_text, strlen(write_text))) {
			printf("Data mismatch in read buffer...");
			return 1;
		}
		/* fall through */
	default:
		if (strncmp(rx_buf, write_text, strlen(write_text))) {
			printf("Data mismatch in rx_buf, found...");
			return 1;
		}
	}
	return 0;
}

static int send_wait_check()
{
	int ret;

	if (opts.dst_addr) {
		ret = fi_write(ep, tx_buf, strlen(welcome_text), mr_desc,
				remote_fi_addr, 0, FT_MR_KEY, &tx_ctx);
 		if (ret) {
 			FT_PRINTERR("fi_write", ret);
 			return ret;
		}
	}

	ret = fi_cntr_wait(test_cntr, n_trig, -1);
	if (ret)
		return ret;

	ret = fi_cntr_wait(rxcntr, rx_exp, -1);
	if (ret)
		return ret;

	return check_data();
}

static int cntr_trigger()
{
	int ret;
	struct fi_op_cntr op_cntr;

	op_cntr.cntr = test_cntr;
	op_cntr.value = 5;
	work.op_type = FI_OP_CNTR_SET;

	work.op.cntr = &op_cntr;

	ret = fi_control(&domain->fid, FI_QUEUE_WORK, &work);
	if (ret)
		return ret;

	work.op_type = FI_OP_CNTR_ADD;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int rma_trigger()
{
	int ret;
	struct fi_msg_rma msg;
	struct fi_rma_iov rma_iov;
	struct iovec iov;
	struct fi_op_rma rma;

	format_simple_msg_rma(&msg, &iov, &rma_iov,
			      tx_buf + strlen(welcome_text),
			      strlen(write_text), &work.context);
	rma.ep = trig_ep;
	rma.msg = msg;
	rma.flags = 0;

	work.op.rma = &rma;
	work.op_type = FI_OP_WRITE;
	ret = fi_control(&domain->fid, FI_QUEUE_WORK, &work);
	if (ret)
		return ret;

	format_simple_msg_rma(&msg, &iov, &rma_iov,
			      result_buf, strlen(write_text), &work.context);
	work.op_type = FI_OP_READ;
	work.triggering_cntr = test_cntr;
	work.threshold = 1;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int atomic_trigger()
{
	int ret;
	size_t count;
	struct fi_op_atomic atomic;
	struct fi_msg_atomic msg;
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;

	ret = check_base_atomic_op(ep, FI_BOR, FI_UINT8, &count);
	if (ret)
		return ret;

	format_simple_msg_atomic(&msg, &iov, &rma_iov,
				 tx_buf + strlen(welcome_text),
				 strlen(welcome_text), &work.context, FI_UINT8, FI_BOR);
	atomic.ep = trig_ep;
	atomic.msg = msg;
	atomic.flags = 0;
	work.op_type = FI_OP_ATOMIC;

	work.op.atomic = &atomic;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int fetch_atomic_trigger()
{
	int ret;
	size_t count;
	struct fi_op_fetch_atomic fetch_atomic;
	struct fi_msg_atomic msg;
	struct fi_msg_fetch fetch;
	struct fi_ioc iov;
	struct fi_ioc fetch_iov;
	struct fi_rma_ioc rma_iov;

	ret = check_fetch_atomic_op(ep, FI_BOR, FI_UINT8, &count);
	if (ret)
		return ret;

	format_simple_msg_atomic(&msg, &iov, &rma_iov,
				 tx_buf + strlen(welcome_text),
				 strlen(welcome_text), &work.context, FI_UINT8, FI_BOR);
	format_simple_msg_fetch(&fetch, &fetch_iov, result_buf, strlen(welcome_text));

	fetch_atomic.ep = trig_ep;
	fetch_atomic.msg = msg;
	fetch_atomic.fetch = fetch;
	fetch_atomic.flags = 0;
	work.op_type = FI_OP_FETCH_ATOMIC;

	work.op.fetch_atomic = &fetch_atomic;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int compare_atomic_trigger()
{
	int ret;
	size_t count;
	struct fi_op_compare_atomic compare_atomic;
	struct fi_msg_atomic msg;
	struct fi_msg_fetch fetch;
	struct fi_msg_compare compare;
	struct fi_ioc iov;
	struct fi_ioc fetch_iov;
	struct fi_ioc compare_iov;
	struct fi_rma_ioc rma_iov;

	ret = check_compare_atomic_op(ep, FI_CSWAP_LE, FI_UINT8, &count);
	if (ret)
		return ret;

	format_simple_msg_atomic(&msg, &iov, &rma_iov,
				 tx_buf + strlen(welcome_text),
				 strlen(welcome_text), &work.context, FI_UINT8, FI_CSWAP_LE);
	format_simple_msg_fetch(&fetch, &fetch_iov, result_buf, strlen(welcome_text));
	format_simple_msg_compare(&compare, &compare_iov, compare_buf, strlen(welcome_text));

	compare_atomic.ep = trig_ep;
	compare_atomic.msg = msg;
	compare_atomic.fetch = fetch;
	compare_atomic.compare = compare;
	compare_atomic.flags = 0;
	work.op_type = FI_OP_COMPARE_ATOMIC;

	work.op.compare_atomic = &compare_atomic;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int tsend_trigger()
{
	int ret;
	struct fi_op_tagged tagged;
	struct fi_msg_tagged msg;
	struct iovec msg_iov;

	format_simple_msg_tagged(&msg, &msg_iov, rx_buf,
				 strlen(write_text), &work.context, 0xCAFE);
	tagged.ep = trig_ep;
	tagged.msg = msg;
	tagged.flags = 0;
	work.op.tagged = &tagged;

	work.op_type = FI_OP_TRECV;
	ret = fi_control(&domain->fid, FI_QUEUE_WORK, &work);
	if (ret)
		return ret;

	work.context = trig_ctx2;

	format_simple_msg_tagged(&msg, &msg_iov, tx_buf + strlen(welcome_text),
				 strlen(write_text), &work.context, 0xCAFE);
	work.op_type = FI_OP_TSEND;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int send_trigger()
{
	int ret;
	struct fi_op_msg op_msg;
	struct fi_msg msg;
	struct iovec msg_iov;

	format_simple_msg(&msg, &msg_iov, rx_buf,
			  strlen(write_text), &work.context);

	op_msg.ep = trig_ep;
	op_msg.msg = msg;
	op_msg.flags = 0;
	work.op.msg = &op_msg;

	work.op_type = FI_OP_RECV;
	ret = fi_control(&domain->fid, FI_QUEUE_WORK, &work);
	if (ret)
		return ret;

	work.context = trig_ctx2;

	format_simple_msg(&msg, &msg_iov, tx_buf + strlen(welcome_text),
			  strlen(write_text), &work.context);
	work.op_type = FI_OP_SEND;
	return fi_control(&domain->fid, FI_QUEUE_WORK, &work);
}

static int trigger()
{
	int ret;

	//both sides start with both counters at 2, client will initiate triggering write
	//which will trigger txcntr on client and rxcntr on server
	//rx_exp = number of rx completions we should expect on that side
	//n_trig = total number of triggers on work queue to expect completed
	if (opts.dst_addr) {
		work.triggering_cntr = txcntr;
		rx_exp = 2;
	} else {
		work.triggering_cntr = rxcntr;
		rx_exp = 3;
	}
	work.threshold = 3;

	if (tested_op != FI_OP_CNTR_ADD && tested_op != FI_OP_CNTR_SET)
		work.completion_cntr = test_cntr;

	switch (tested_op) {
	case FI_OP_RECV:
	case FI_OP_SEND:
		ret = send_trigger();
		n_trig++;
		break;
	case FI_OP_TRECV:
	case FI_OP_TSEND:
		ret = tsend_trigger();
		n_trig++;
		break;
	case FI_OP_READ:
	case FI_OP_WRITE:
		ret = rma_trigger();
		n_trig++;
		rx_exp += 2;
		break;
	case FI_OP_ATOMIC:
		ret = atomic_trigger();
		rx_exp++;
		break;
	case FI_OP_FETCH_ATOMIC:
		ret = fetch_atomic_trigger();
		rx_exp++;
		break;
	case FI_OP_COMPARE_ATOMIC:
		ret = compare_atomic_trigger();
		rx_exp++;
		break;
	default:
		work.op_type = tested_op;
		ret = cntr_trigger();
		n_trig++;
	}
	n_trig++;

	if (ret) {
		if (ret == -FI_ENOSYS)
			fprintf(stdout, "Operation not supported by provider\n");
		else
			FT_PRINTERR("fi_control", ret);
		return ret;
	}

	ret = send_wait_check();

	return ret;
}

static void init_buf_vals()
{
	switch (tested_op) {
	case FI_OP_ATOMIC:
	case FI_OP_FETCH_ATOMIC:
	case FI_OP_COMPARE_ATOMIC:
		memset(compare_buf, ~0, strlen(welcome_text) * 2);
		sprintf(tx_buf, "%s", welcome_text);
		memset(&tx_buf[strlen(welcome_text)], ~0, strlen(welcome_text));
		if (opts.dst_addr)
			sprintf(rx_buf, "%s", welcome_text);
		break;
	default:
		sprintf(tx_buf, "%s%s", welcome_text, write_text);
	}
}

static int run_test()
{
	int ret;

	work.context = trig_ctx1;

	printf("Testing triggered %s", fi_tostr(&tested_op, FI_TYPE_OP_TYPE));
	if (tested_op != FI_OP_ATOMIC && tested_op != FI_OP_FETCH_ATOMIC &&
	    tested_op != FI_OP_COMPARE_ATOMIC) {
		tested_op++;
		printf("/%s", fi_tostr(&tested_op, FI_TYPE_OP_TYPE));
	}
	printf(" with counters..."); 

	init_buf_vals();

	//eat up first receive to make sure the triggered op doesn't go there instead
	ret = ft_tx(ep, remote_fi_addr, strlen(welcome_text), &tx_ctx);
	if (ret)
		return ret;
	ret = ft_get_rx_comp(rx_seq);
	if (ret)
		return ret;

	ret = trigger();
	if (ret) {
		if (ret != -FI_ENOSYS) {
	       		printf("FAIL\n");
       			if (ret != 1)
       				FT_PRINTERR("cntr_trigger", ret);
       		}
	} else {
		printf("PASS\n");
	}

	return ret;
}

static int alloc_mr_res()
{
	int ret;

	result_buf = calloc(buf_size, sizeof(*result_buf));
	compare_buf = calloc(buf_size, sizeof(*compare_buf));
	if (!result_buf || !compare_buf) {
		fprintf(stdout, "calloc\n");
		return -FI_ENOMEM;
	}

	ret = fi_mr_reg(domain, buf, buf_size, FT_RMA_MR_ACCESS | FT_MSG_MR_ACCESS,
			0, FT_MR_KEY, 0, &mr, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}

	ret = fi_mr_reg(domain, result_buf, buf_size, FT_RMA_MR_ACCESS,
			0, FT_MR_KEY + 1, 0, &mr_result, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}
	ret = fi_mr_reg(domain, compare_buf, buf_size, FT_RMA_MR_ACCESS,
			0, FT_MR_KEY + 2, 0, &mr_compare, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}
	return 0;
}

static void free_mr_res()
{
	FT_CLOSE_FID(mr_result);
	FT_CLOSE_FID(mr_compare);
	free(result_buf);
	free(compare_buf);
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options = FT_OPT_SIZE | FT_OPT_RX_CNTR | FT_OPT_TX_CNTR |
		       FT_OPT_SKIP_REG_MR;
	opts.mr_mode = FI_MR_LOCAL | FI_MR_VIRT_ADDR | FI_MR_ALLOCATED;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	tested_op = FI_OP_CNTR_SET;

	while ((op = getopt(argc, argv, "aT:h" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'a':
			use_alias = 1;
			break;
		case 'T':
			if (!strncasecmp("msg", optarg, 3))
				tested_op = FI_OP_RECV;
			else if (!strncasecmp("tagged", optarg, 6))
				tested_op = FI_OP_TRECV;
			else if (!strncasecmp("rma", optarg, 3))
				tested_op = FI_OP_READ;
			else if (!strncasecmp("atomic", optarg, 6))
				tested_op = FI_OP_ATOMIC;
			else if (!strncasecmp("f_atomic", optarg, 8))
				tested_op = FI_OP_FETCH_ATOMIC;
			else if (!strncasecmp("c_atomic", optarg, 8))
				tested_op = FI_OP_COMPARE_ATOMIC;
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "A simple RDM client-sever triggered RMA example with alias ep.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type = FI_EP_RDM;
	hints->caps = FI_MSG | FI_RMA | FI_RMA_EVENT | FI_TRIGGER;

	if (tested_op == FI_OP_TSEND)
		hints->caps |= FI_TAGGED;
	else if (tested_op == FI_OP_ATOMIC ||
		 tested_op == FI_OP_FETCH_ATOMIC ||
		 tested_op == FI_OP_COMPARE_ATOMIC)
		hints->caps |= FI_ATOMIC;

	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	if (use_alias) {
		ret = ft_init_alias_ep(FI_TRANSMIT | FI_TRIGGER);
		if (ret)
			return ret;
		trig_ep = alias_ep;
	} else {
		trig_ep = ep;
	}

	ret = ft_cntr_open(&test_cntr);
	if (ret) {
		FT_PRINTERR("fi_cntr_open", ret);
		return ret;
	}

	ret = alloc_mr_res();
	if (ret)
		return ret;

	ret = run_test();
	if (ret)
		return ret;

	free_mr_res();

	ret = fi_close(&test_cntr->fid);
	if (ret)
		FT_PRINTERR("fi_cntr_close", ret);

	ft_free_res();
	return ft_exit_code(ret);
}
