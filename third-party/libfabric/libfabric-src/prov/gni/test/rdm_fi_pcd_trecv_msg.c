/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
 * Copyright (c) 2019 Triad National Security, LLC. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "gnix_vc.h"
#include "gnix_cm_nic.h"
#include "gnix_hashtable.h"
#include "gnix_rma.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

/* Both the send and recv paths use independent state machines within
 * each test to simulate the behavior you would expect in a client/server
 * or independent peer environment
 *
 * Below are the valid states, but state transitions may differ from test
 * to test. To simplify the reading of each test, it is recommended that
 * the state transitions in the test are written in order.
 *
 * Ex.
 * switch (send_state) {
 * case STATE_1:
 * case STATE_2:
 * case STATE_3:
 * ....
 * case STATE_DONE:
 * default:
 * }
 *
 * Normal switch semantics should be followed and no tag should be reused
 * within the same test. Duplicates will trigger an infinite loop.
 */

#define stringify(x) helper(x)
#define helper(x) #x

#define STATE_DEF(x) [x] = stringify(x)

/* sender states */
enum send_state {
	S_STATE_SEND_MSG_1 = 0,       /* send the first message */
	S_STATE_SEND_MSG_1_WAIT_CQ,   /* wait for the first completion */
	S_STATE_SEND_MSG_2,           /* send the second message */
	S_STATE_SEND_MSG_2_WAIT_CQ,   /* wait for the second completion */
	S_STATE_DONE,                 /* sender is done */
	S_STATE_INVALID,              /* invalid state used for test init */
};

__attribute__((unused))
static char *send_state_strings[S_STATE_INVALID + 1] = {
	STATE_DEF(S_STATE_SEND_MSG_1),
	STATE_DEF(S_STATE_SEND_MSG_1_WAIT_CQ),
	STATE_DEF(S_STATE_SEND_MSG_2),
	STATE_DEF(S_STATE_SEND_MSG_2_WAIT_CQ),
	STATE_DEF(S_STATE_DONE),
	STATE_DEF(S_STATE_INVALID),
};

/* receiver states */
enum recv_state {
	R_STATE_PEEK = 0,             /* peek a message */
	R_STATE_PEEK_WAIT_CQ,         /* wait for cqe from peek */
	/* wait for error cq */
	R_STATE_PEEK_WAIT_ERR_CQ,
	R_STATE_PEEK_CLAIM,           /* peek|claim a message */
	R_STATE_PEEK_CLAIM_WAIT_CQ,   /* wait for cqe from peek|claim */
	/* wait for error cq */
	R_STATE_PEEK_CLAIM_WAIT_ERR_CQ,
	R_STATE_PEEK_DISCARD,         /* peek|discard a message */
	R_STATE_PEEK_DISCARD_WAIT_CQ, /* wait for cqe from peek|discard */
	/* wait for error cq */
	R_STATE_PEEK_DISCARD_WAIT_ERR_CQ,
	R_STATE_CLAIM,                /* claim a message */
	R_STATE_CLAIM_WAIT_CQ,        /* wait for cqe from claim */
	R_STATE_CLAIM_DISCARD,        /* claim|discard a message */
	R_STATE_CLAIM_DISCARD_WAIT_CQ,
	R_STATE_RECV_MSG_1,           /* recv first message */
	R_STATE_RECV_MSG_1_WAIT_CQ,   /* wait for cqe from first recv message */
	R_STATE_RECV_MSG_2,           /* recv second message */
	R_STATE_RECV_MSG_2_WAIT_CQ,   /* wait for cqe from second recv message*/
	R_STATE_DONE,                 /* receiver is done */
	R_STATE_INVALID,              /* invalid state used for test init */
};

__attribute__((unused))
static char *recv_state_strings[R_STATE_INVALID + 1] = {
	STATE_DEF(R_STATE_PEEK),
	STATE_DEF(R_STATE_PEEK_WAIT_CQ),
	STATE_DEF(R_STATE_PEEK_WAIT_ERR_CQ),
	STATE_DEF(R_STATE_PEEK_CLAIM),
	STATE_DEF(R_STATE_PEEK_CLAIM_WAIT_CQ),
	STATE_DEF(R_STATE_PEEK_CLAIM_WAIT_ERR_CQ),
	STATE_DEF(R_STATE_PEEK_DISCARD),
	STATE_DEF(R_STATE_PEEK_DISCARD_WAIT_CQ),
	STATE_DEF(R_STATE_PEEK_DISCARD_WAIT_ERR_CQ),
	STATE_DEF(R_STATE_CLAIM),
	STATE_DEF(R_STATE_CLAIM_WAIT_CQ),
	STATE_DEF(R_STATE_CLAIM_DISCARD),
	STATE_DEF(R_STATE_RECV_MSG_1),
	STATE_DEF(R_STATE_RECV_MSG_1_WAIT_CQ),
	STATE_DEF(R_STATE_RECV_MSG_2),
	STATE_DEF(R_STATE_RECV_MSG_2_WAIT_CQ),
	STATE_DEF(R_STATE_DONE),
	STATE_DEF(R_STATE_INVALID),
};

/* poll the SCQ with a dummy CQE if true */
#define SHOULD_BLIND_POLL_SCQ(state) \
	((state) == S_STATE_DONE)

/* poll the RCQ with a dummy CQE if true */
#define SHOULD_BLIND_POLL_RCQ(state) \
	((state) != R_STATE_PEEK_WAIT_CQ && \
			state != R_STATE_PEEK_CLAIM_WAIT_CQ && \
			state != R_STATE_PEEK_DISCARD_WAIT_CQ && \
			state != R_STATE_CLAIM_WAIT_CQ && \
			state != R_STATE_CLAIM_DISCARD_WAIT_CQ && \
			state != R_STATE_RECV_MSG_1_WAIT_CQ && \
			state != R_STATE_RECV_MSG_2_WAIT_CQ && \
			state != R_STATE_PEEK_WAIT_ERR_CQ && \
			state != R_STATE_PEEK_CLAIM_WAIT_ERR_CQ && \
			state != R_STATE_PEEK_DISCARD_WAIT_ERR_CQ)

#define RAISE_UNREACHABLE_STATE cr_assert_eq(0, 1, "unreachable state")
#define ASSERT_SEND_RECV_DONE                      \
	do {                                           \
		if (!SEND_RECV_DONE)                       \
			dbg_printf("failed on test size %d\n", len);  \
		if (s_state != S_STATE_DONE)               \
			dbg_printf("failed to finish send side, " \
					"state=%s\n",                  \
					send_state_strings[s_state]);        \
		if (r_state != R_STATE_DONE)               \
			dbg_printf("failed to finish recv side, " \
					"state=%s\n",                  \
					recv_state_strings[r_state]);  \
		cr_assert_eq(s_state, S_STATE_DONE);       \
		cr_assert_eq(r_state, R_STATE_DONE);       \
	} while (0)

#define PROGRESS_CQS(cqs)                             \
	do {                                              \
		struct fi_cq_tagged_entry trash;              \
		int __events = 0;                             \
		if (SHOULD_BLIND_POLL_SCQ(s_state)) {         \
			__events = fi_cq_read(cqs[0], &trash, 1); \
			if (__events == 1 && rate_limit())        \
				dbg_printf("found unexpected events: scq\n"); \
		}                                             \
		if (SHOULD_BLIND_POLL_RCQ(r_state)) {         \
			__events = fi_cq_read(cqs[1], &trash, 1); \
			if (__events == 1 && rate_limit())        \
				dbg_printf("found unexpected events: rcq\n"); \
		}                                             \
	} while (0)

#define __STATE_TRANSITION(state, next_state, labels) \
	do {                                      \
		if ((state) != (next_state))          \
			dbg_printf(stringify(state) " transition: %s to %s\n", \
					labels[state], labels[next_state]);       \
		(state) = (next_state);               \
	} while (0)

#define SEND_STATE_TRANSITION(next_state) \
		__STATE_TRANSITION(s_state, next_state, send_state_strings)

#define RECV_STATE_TRANSITION(next_state) \
		__STATE_TRANSITION(r_state, next_state, recv_state_strings)

#define __COND_STATE_TRANSITION(cond, state, true_state, false_state, labels) \
	__STATE_TRANSITION(state, ((cond) ? (true_state) : (false_state)), labels)

#define COND_RECV_STATE_TRANSITION(actual, expected, true_state, false_state) \
	__COND_STATE_TRANSITION(((actual) == (expected)), \
			r_state, true_state, false_state, recv_state_strings)

#define COND_SEND_STATE_TRANSITION(actual, expected, true_state, false_state) \
	__COND_STATE_TRANSITION(((actual) == (expected)), \
			s_state, true_state, false_state, send_state_strings)

#define INIT_TEST_STATE(send_state, recv_state) \
	do {                                        \
		SEND_STATE_TRANSITION(send_state);      \
		RECV_STATE_TRANSITION(recv_state);      \
	} while (0)

#define SEND_RECV_DONE (r_state == R_STATE_DONE && s_state == S_STATE_DONE)

#define TEST_TIME_LIMIT_EXPIRED (elapsed >= max_test_time)

#if 1
#define dbg_printf(...) do { } while (0)
#else
#define dbg_printf(...) \
	do { \
		fprintf(stderr, __VA_ARGS__); \
		fflush(stderr); \
	} while (0)
#endif

struct timeval begin, end;
struct timeval loop_start, loop_end;

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fid_ep *ep[2];
static struct fid_av *av;
static struct fi_info *hints;
static struct fi_info *fi;
static void *ep_name[2];
static size_t gni_addr[2];
static struct fid_cq *msg_cq[2];
static struct fi_cq_attr cq_attr;

#define BUF_SZ (64*1024)
static char *target, *target_base;
static char *source, *source_base;
static struct fid_mr *rem_mr, *loc_mr;
static uint64_t mr_key;
static const int max_test_time = 10;

/* test variables */
static int elapsed;
enum send_state s_state;
enum recv_state r_state;
struct fi_cq_tagged_entry d_peek_cqe;
struct fi_cq_err_entry cqe_error;
struct fi_msg_tagged peek_msg;
struct iovec peek_iov;
char *peek_buffer;


static int elapsed_seconds(struct timeval *s, struct timeval *e)
{
	/* rough estimate... I don't care that this is accurate */
	int seconds = e->tv_sec - s->tv_sec;

	if (!seconds)
		return seconds;

	if (e->tv_usec <= s->tv_usec)
		seconds -= 1;

	return seconds;
}

static int rate_limit(void)
{
	static struct timeval _rl_begin, _rl_end;
	static int init, squelched, events;
	int ret = 0;

	if (!init)
		gettimeofday(&_rl_begin, NULL);

	gettimeofday(&_rl_end, NULL);

	if (events < ((elapsed_seconds(&_rl_begin, &_rl_end) + 1) * 20)) {
		ret = 1;
		events++;
	}

	if (ret && squelched) {
		dbg_printf("squelched %d similar events\n", squelched);
		squelched = 0;
	} else if (!ret) {
		squelched++;
	}

	return ret;
}

static inline void init_test_variables(void)
{
	/* re-initialize all variables necessary here */
	elapsed = 0;
	r_state = R_STATE_INVALID;
	s_state = S_STATE_INVALID;

	memset(&d_peek_cqe, 0, sizeof(struct fi_cq_tagged_entry));
	memset(&peek_msg, 0, sizeof(struct fi_msg_tagged));
	memset(&peek_iov, 0, sizeof(struct iovec));
}

static void rdm_fi_pdc_setup(void)
{
	int ret = 0;
	struct fi_av_attr attr;
	size_t addrlen = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	memset(&attr, 0, sizeof(attr));
	attr.type = FI_AV_MAP;
	attr.count = 16;

	ret = fi_av_open(dom, &attr, &av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom, fi, &ep[0], NULL);
	cr_assert(!ret, "fi_endpoint");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(dom, &cq_attr, &msg_cq[0], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_cq_open(dom, &cq_attr, &msg_cq[1], 0);
	cr_assert(!ret, "fi_cq_open");

	ret = fi_ep_bind(ep[0], &msg_cq[0]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_getname(&ep[0]->fid, NULL, &addrlen);
	cr_assert(addrlen > 0);

	ep_name[0] = malloc(addrlen);
	cr_assert(ep_name[0] != NULL);

	ret = fi_getname(&ep[0]->fid, ep_name[0], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_endpoint(dom, fi, &ep[1], NULL);
	cr_assert(!ret, "fi_endpoint");

	ret = fi_ep_bind(ep[1], &msg_cq[1]->fid, FI_SEND | FI_RECV);
	cr_assert(!ret, "fi_ep_bind");

	ep_name[1] = malloc(addrlen);
	cr_assert(ep_name[1] != NULL);

	ret = fi_getname(&ep[1]->fid, ep_name[1], &addrlen);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_av_insert(av, ep_name[0], 1, &gni_addr[0], 0,
				NULL);
	cr_assert(ret == 1);

	ret = fi_av_insert(av, ep_name[1], 1, &gni_addr[1], 0,
				NULL);
	cr_assert(ret == 1);

	ret = fi_ep_bind(ep[0], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_ep_bind(ep[1], &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep[0]);
	cr_assert(!ret, "fi_ep_enable");

	ret = fi_enable(ep[1]);
	cr_assert(!ret, "fi_ep_enable");

	target_base = malloc(GNIT_ALIGN_LEN(BUF_SZ*2));
	assert(target_base);
	target = GNIT_ALIGN_BUFFER(char *, target_base);

	source_base = malloc(GNIT_ALIGN_LEN(BUF_SZ*2));
	assert(source_base);
	source = GNIT_ALIGN_BUFFER(char *, source_base);

	peek_buffer = calloc(BUF_SZ, sizeof(char));
	cr_assert(peek_buffer);

	ret = fi_mr_reg(dom,
			  target,
			  BUF_SZ*2,
			  FI_REMOTE_WRITE,
			  0,
			  ((USING_SCALABLE(fi)) ? 1 : 0),
			  0,
			  &rem_mr,
			  &target);
	cr_assert_eq(ret, 0);

	ret = fi_mr_reg(dom,
			  source,
			  BUF_SZ*2,
			  FI_REMOTE_WRITE,
			  0,
			  ((USING_SCALABLE(fi)) ? 2 : 0),
			  0,
			  &loc_mr,
			  &source);
	cr_assert_eq(ret, 0);

	if (USING_SCALABLE(fi)) {
		MR_ENABLE(rem_mr, target, BUF_SZ*2);
		MR_ENABLE(loc_mr, source, BUF_SZ*2);
	}

	mr_key = fi_mr_key(rem_mr);

	init_test_variables();
}

static void rdm_fi_pdc_teardown(void)
{
	int ret = 0;

	fi_close(&loc_mr->fid);
	fi_close(&rem_mr->fid);

	free(target_base);
	free(source_base);

	ret = fi_close(&ep[0]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&ep[1]->fid);
	cr_assert(!ret, "failure in closing ep.");

	ret = fi_close(&msg_cq[0]->fid);
	cr_assert(!ret, "failure in send cq.");

	ret = fi_close(&msg_cq[1]->fid);
	cr_assert(!ret, "failure in recv cq.");

	ret = fi_close(&av->fid);
	cr_assert(!ret, "failure in closing av.");

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "failure in closing domain.");

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
	free(ep_name[0]);
	free(ep_name[1]);
	free(peek_buffer);
}

static void rdm_fi_pdc_init_data_range(
		char *buf,
		int start,
		int len,
		char seed)
{
	int i;

	for (i = start; i < start + len; i++)
		buf[i] = seed;
}

static void rdm_fi_pdc_init_data(
		char *buf,
		int len,
		char seed)
{
	rdm_fi_pdc_init_data_range(buf, 0, len, seed);
}

static int rdm_fi_pdc_check_data_range(
		char *src,
		char *dst,
		int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (src[i] != dst[i]) {
			printf("data mismatch, elem: %d, exp: %x, act: %x\n",
				   i, src[i], dst[i]);
			return 0;
		}
	}

	return 1;
}

static int rdm_fi_pdc_check_data_pattern(
		char *buf,
		char pattern,
		int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (buf[i] != pattern) {
			printf("data mismatch, elem: %d, exp: %x, act: %x\n",
				   i, pattern, buf[i]);
			return 0;
		}
	}

	return 1;
}

static int rdm_fi_pdc_check_data(
		char *buf1,
		char *buf2,
		int len)
{
	return rdm_fi_pdc_check_data_range(buf1, buf2, len);
}

static void rdm_fi_pdc_xfer_for_each_size(
		void (*xfer)(int len),
		int slen,
		int elen)
{
	int i;

	for (i = slen; i <= elen; i *= 2) {
		dbg_printf("running test on size %d\n", i);
		xfer(i);
	}
}

/*******************************************************************************
 * Test MSG functions
 ******************************************************************************/

TestSuite(rdm_fi_pdc,
		.init = rdm_fi_pdc_setup,
		.fini = rdm_fi_pdc_teardown,
		.disabled = false);

static void build_message(
		struct fi_msg_tagged *msg,
		struct iovec *iov,
		void *t,
		int len,
		void **rem_mr,
		size_t gni_addr,
		void *source,
		uint64_t tag,
		uint64_t ignore)
{
	iov->iov_base = t;
	iov->iov_len = len;

	msg->msg_iov = iov;
	msg->desc = rem_mr;
	msg->iov_count = 1;
	msg->addr = gni_addr;
	msg->context = source;
	msg->data = (uint64_t) source;
	msg->tag = tag;
	msg->ignore = ignore;
}

static void build_peek_message(
		struct fi_msg_tagged *peek,
		struct fi_msg_tagged *msg)
{
	/* copy contents */
	*peek = *msg;

	peek->msg_iov = &peek_iov;

	peek_iov.iov_base = peek_buffer;
	peek_iov.iov_len = msg->msg_iov[0].iov_len;
}

#define TSEND_FLAGS (FI_MSG | FI_SEND | FI_TAGGED)
#define TRECV_FLAGS (FI_MSG | FI_RECV | FI_TAGGED)

static void validate_cqe_contents(
		struct fi_cq_tagged_entry *entry,
		uint64_t flags,
		void *buf,
		size_t len,
		uint64_t tag,
		void *context)
{
	cr_assert_eq(entry->op_context, context);
	cr_assert_eq(entry->flags, flags);
	cr_assert_eq(entry->data, 0);

	if (flags & FI_RECV) {
		if (!(flags & FI_DISCARD)) {
			cr_assert_eq(entry->len, len);
		}
		else {
			cr_assert_eq(entry->len, 0);
			cr_assert_eq(entry->buf, NULL);
		}
		cr_assert_eq(entry->tag, tag);
		if (entry->buf != NULL)
			cr_assert_eq(entry->buf, buf);
	} else {
		cr_assert_eq(entry->len, 0);
		cr_assert_eq(entry->tag, 0);
		cr_assert_eq(entry->buf, 0);
	}
}

static void validate_cqe_with_message(
		struct fi_cq_tagged_entry *entry,
		struct fi_msg_tagged *msg,
		uint64_t flags)
{
	validate_cqe_contents(entry, flags, msg->msg_iov[0].iov_base,
			msg->msg_iov[0].iov_len, msg->tag, msg->context);
}


static inline void map_src_cqes_to_src_context(
		struct fi_cq_tagged_entry *source_cqes,
		struct fi_cq_tagged_entry **mapping,
		void **context)
{
	int i, j;

	/* map src cqes to src parameters */
	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
			if (source_cqes[i].op_context == context[j])
				mapping[j] = &source_cqes[i];
}

static inline void start_test_timer(void)
{
	gettimeofday(&loop_start, NULL);
}

static inline void update_test_timer(void)
{
	gettimeofday(&loop_end, NULL);
	elapsed = elapsed_seconds(&loop_start, &loop_end);
}

/*
ssize_t (*recvmsg)(struct fid_ep *ep, const struct fi_msg *msg,
		uint64_t flags);
 */
Test(rdm_fi_pdc, peek_no_event)
{
	/*
	 * This test should do nothing but peek into EP to ensure that
	 * no messages are there. This should be a simple test
	 */

	int ret;
	struct fi_msg_tagged msg;
	struct iovec iov;

	build_message(&msg, &iov, target, 128, (void *) &rem_mr, gni_addr[0],
			source, 128, 0);

	ret = fi_trecvmsg(ep[1], &msg, FI_PEEK);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
	cr_assert_eq(ret, 1);

	cr_assert_eq(cqe_error.buf, msg.msg_iov[0].iov_base);
	cr_assert_eq(cqe_error.len, msg.msg_iov[0].iov_len);
	cr_assert_eq(cqe_error.err, FI_ENOMSG);
	cr_assert_eq(cqe_error.olen, msg.msg_iov[0].iov_len);
	cr_assert_eq(cqe_error.op_context, msg.context);
	cr_assert_eq(cqe_error.prov_errno, FI_ENOMSG);
	cr_assert_eq(cqe_error.tag, msg.tag);
}

static void pdc_peek_event_present_buffer_provided(int len)
{
	/* PEEK then RECV with no buffer during peek test
	 *
	 * For each message size,
	 *   1. send a message
	 *   2. peek| the receiver to find the message
	 *   3. receive the message
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents.
	 *
	 *   This is the special case where the application provides a buffer
	 *   during the peek for which some of the data can be written.
	 *
	 *      An  application  may supply a buffer as part of the
	 *      peek operation. If given, the provider may return a copy
	 *      of the message data.
	 *
	 *	 Ideally, both cases should be tested, where the provider
	 *	 returns a NULL pointer indicating that no data was available
	 *	 yet even though the peek succeeded, and the case where some
	 *	 of the data is copied back.
	 */
	int ret;
	struct fi_msg_tagged msg;
	struct iovec iov;
	struct fi_cq_tagged_entry s_cqe;
	struct fi_cq_tagged_entry d_cqe;

	rdm_fi_pdc_init_data(source, len, 0xab);
	rdm_fi_pdc_init_data(target, len, 0);

	build_message(&msg, &iov, target, len, (void *) &rem_mr, gni_addr[0],
			source, len, 0);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK);

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], source, len, loc_mr,
					gni_addr[1], len, target);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK);

			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_WAIT_CQ,
					R_STATE_PEEK);
			break;
		case R_STATE_PEEK_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK,
						R_STATE_PEEK_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_1,
						R_STATE_PEEK_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_1:
			ret = fi_trecvmsg(ep[1], &msg, 0);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_RECV_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* validate the expected results */
	validate_cqe_contents(&s_cqe, TSEND_FLAGS, source, len, len, target);
	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK);

	/* if CQE provided a buffer back, the data was copied.
	 * Check the data */
	if (d_peek_cqe.buf) {
		cr_assert(rdm_fi_pdc_check_data_pattern(peek_buffer, 0xab, len),
				"Data mismatch");
	}

	validate_cqe_with_message(&d_cqe, &msg, TRECV_FLAGS);
	cr_assert(rdm_fi_pdc_check_data(source, target, len), "Data mismatch");
}

Test(rdm_fi_pdc, peek_event_present_buff_provided)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_event_present_buffer_provided,
			1, BUF_SZ);
}

static void pdc_peek_event_present_no_buff_provided(int len)
{
	/* PEEK then RECV with no buffer during peek test
	 *
	 * For each message size,
	 *   1. send a message
	 *   2. peek| the receiver to find the message
	 *   3. receive the message
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents.
	 *
	 * The CQE for the peek should have a NULL buffer field
	 */

	int ret;
	struct fi_msg_tagged msg;
	struct iovec iov;
	struct fi_cq_tagged_entry s_cqe;
	struct fi_cq_tagged_entry d_cqe;

	rdm_fi_pdc_init_data(source, len, 0xab);
	rdm_fi_pdc_init_data(target, len, 0);

	build_message(&msg, &iov, target, len, (void *) &rem_mr,
			gni_addr[0], source, len, 0);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK);

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg);

	/* send in a null buffer to indicate that we don't want data back */
	peek_iov.iov_base = NULL;

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], source, len, loc_mr,
					gni_addr[1], len, target);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_WAIT_CQ,
					R_STATE_PEEK);
			break;
		case R_STATE_PEEK_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK,
						R_STATE_PEEK_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_1,
						R_STATE_PEEK_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_1:
			ret = fi_trecvmsg(ep[1], &msg, 0);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_RECV_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* verify test execution correctness */
	validate_cqe_contents(&s_cqe, TSEND_FLAGS, source, len, len, target);
	validate_cqe_with_message(&d_cqe, &msg, TRECV_FLAGS);

	/* a pointer should never be returned */
	cr_assert_eq(d_peek_cqe.buf, NULL);
	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK);
	cr_assert(rdm_fi_pdc_check_data(source, target, len),
			"Data mismatch");
}

Test(rdm_fi_pdc, peek_event_present_no_buff_provided)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_event_present_no_buff_provided,
				1, BUF_SZ);
}

static void pdc_peek_claim_same_tag(int len)
{
	/* PEEK|CLAIM then CLAIM using the same tags test
	 *
	 * For each message size,
	 *   1. send two messages with identical tags but different
	 *      buffer parameters
	 *   2. peek|claim the receiver to find and claim the first message
	 *   3. receive the second message
	 *   4. claim the first message
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents.
	 */
	int ret, i;
	struct fi_msg_tagged msg[2];
	struct iovec iov[2];
	struct fi_cq_tagged_entry s_cqe[2];
	struct fi_cq_tagged_entry d_cqe[2];
	struct fi_cq_tagged_entry *src_cqe[2] = {NULL, NULL};
	char *src_buf[2] = {source, source + len};
	char *dst_buf[2] = {target, target + len};
	void *src_context[2] = {target, target + len};

	/* initialize the initial data range on the source buffer to have
	 * different data values for one message than for the other
	 */
	rdm_fi_pdc_init_data_range(source, 0, len, 0xa5);
	rdm_fi_pdc_init_data_range(source, len, len, 0x5a);
	rdm_fi_pdc_init_data(target, len*2, 0);

	/* set up messages */
	for (i = 0; i < 2; i++) {
		build_message(&msg[i], &iov[i], dst_buf[i], len, (void *)
				&rem_mr, gni_addr[0], src_buf[i], len, 0);
	}

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg[0]);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK_CLAIM);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], src_buf[0], len, loc_mr,
					gni_addr[1], len, dst_buf[0]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_2);
			break;
		case S_STATE_SEND_MSG_2:
			ret = fi_tsend(ep[0], src_buf[1], len, loc_mr,
					gni_addr[1], len, dst_buf[1]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[0], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_SEND_MSG_2_WAIT_CQ,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[1], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_2_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK_CLAIM:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK | FI_CLAIM);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_CLAIM_WAIT_CQ,
					R_STATE_PEEK_CLAIM);
			break;
		case R_STATE_PEEK_CLAIM_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK_CLAIM,
						R_STATE_PEEK_CLAIM_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_2,
						R_STATE_PEEK_CLAIM_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_2:
			ret = fi_trecvmsg(ep[1], &msg[1], 0);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_RECV_MSG_2_WAIT_CQ,
					R_STATE_RECV_MSG_2);
			break;
		case R_STATE_RECV_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[1], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_CLAIM,
					R_STATE_RECV_MSG_2_WAIT_CQ);
			break;
		case R_STATE_CLAIM:
			ret = fi_trecvmsg(ep[1], &msg[0], FI_CLAIM);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_CLAIM_WAIT_CQ,
					R_STATE_CLAIM);
			break;
		case R_STATE_CLAIM_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[0], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_CLAIM_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* map src cqes to src parameters */
	map_src_cqes_to_src_context(s_cqe, src_cqe, src_context);

	/* verify test execution correctness */
	validate_cqe_contents(src_cqe[0], TSEND_FLAGS,
			src_buf[0], len, len, dst_buf[0]);
	validate_cqe_contents(src_cqe[1], TSEND_FLAGS,
			src_buf[1], len, len, dst_buf[1]);
	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK | FI_CLAIM);
	validate_cqe_with_message(&d_cqe[1], &msg[1], TRECV_FLAGS);
	validate_cqe_with_message(&d_cqe[0], &msg[0], TRECV_FLAGS | FI_CLAIM);

	/* if CQE provided a buffer back, the data was copied.
	 * Check the data */
	if (d_peek_cqe.buf) {
		cr_assert(rdm_fi_pdc_check_data_pattern(peek_buffer, 0xa5, len),
				"Data mismatch");
	}


	cr_assert(rdm_fi_pdc_check_data(src_buf[0], dst_buf[0], len),
			"Data mismatch");

	cr_assert(rdm_fi_pdc_check_data(src_buf[1], dst_buf[1], len),
			"Data mismatch");
}

Test(rdm_fi_pdc, peek_claim_same_tag)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_claim_same_tag, 1, BUF_SZ);
}

static void pdc_peek_claim_unique_tag(int len)
{
	/* PEEK|CLAIM then CLAIM using unique tags test
	 *
	 * For each message size,
	 *   1. send two messages with unique tags and parameters
	 *   2. peek|claim the receiver to find and claim the first message
	 *   3. receive the second message
	 *   4. claim the first message
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents
	 */

	int ret, i;
	struct fi_msg_tagged msg[2];
	struct iovec iov[2];
	struct fi_cq_tagged_entry s_cqe[2];
	struct fi_cq_tagged_entry d_cqe[2];
	struct fi_cq_tagged_entry *src_cqe[2] = {NULL, NULL};
	char *src_buf[2] = {source, source + len};
	char *dst_buf[2] = {target, target + len};
	void *src_context[2] = {target, target + len};

	/* initialize the initial data range on the source buffer to have
	 * different data vaules for one message than for the other
	 */
	rdm_fi_pdc_init_data_range(source, 0, len, 0xa5);
	rdm_fi_pdc_init_data_range(source, len, len, 0x5a);
	rdm_fi_pdc_init_data(target, len*2, 0);

	/* set up messages */
	for (i = 0; i < 2; i++) {
		build_message(&msg[i], &iov[i], dst_buf[i], len,
				(void *) &rem_mr, gni_addr[0],
				src_buf[i], len + i, 0);
	}

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg[0]);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK_CLAIM);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], src_buf[0], len, loc_mr,
					gni_addr[1], len, dst_buf[0]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_2);
			break;
		case S_STATE_SEND_MSG_2:
			ret = fi_tsend(ep[0], src_buf[1], len, loc_mr,
					gni_addr[1], len + 1, dst_buf[1]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[0], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_SEND_MSG_2_WAIT_CQ,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[1], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_2_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK_CLAIM:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK | FI_CLAIM);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_CLAIM_WAIT_CQ,
					R_STATE_PEEK_CLAIM);
			break;
		case R_STATE_PEEK_CLAIM_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK_CLAIM,
						R_STATE_PEEK_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_2,
						R_STATE_PEEK_CLAIM_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_2:
			ret = fi_trecvmsg(ep[1], &msg[1], 0);
			cr_assert_eq(ret, FI_SUCCESS);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_RECV_MSG_2_WAIT_CQ,
					R_STATE_RECV_MSG_2);
			break;
		case R_STATE_RECV_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[1], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_CLAIM,
					R_STATE_RECV_MSG_2_WAIT_CQ);
			break;
		case R_STATE_CLAIM:
			ret = fi_trecvmsg(ep[1], &msg[0], FI_CLAIM);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_CLAIM_WAIT_CQ,
					R_STATE_CLAIM);
			break;
		case R_STATE_CLAIM_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[0], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_CLAIM_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* map src cqes to src parameters */
	map_src_cqes_to_src_context(s_cqe, src_cqe, src_context);

	/* verify test execution correctness */
	for (i = 0; i < 2; i++)
		validate_cqe_contents(src_cqe[i], TSEND_FLAGS,
				src_buf[i], len, len + i, dst_buf[i]);

	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK | FI_CLAIM);
	validate_cqe_with_message(&d_cqe[1], &msg[1], TRECV_FLAGS);
	validate_cqe_with_message(&d_cqe[0], &msg[0], TRECV_FLAGS | FI_CLAIM);

	/* if CQE provided a buffer back, the data was copied.
	 * Check the data */
	if (d_peek_cqe.buf) {
		cr_assert(rdm_fi_pdc_check_data_pattern(peek_buffer, 0xa5, len),
				"Data mismatch");
	}


	cr_assert(rdm_fi_pdc_check_data(src_buf[0], dst_buf[0], len),
			"Data mismatch");
	cr_assert(rdm_fi_pdc_check_data(src_buf[1], dst_buf[1], len),
			"Data mismatch");
}

Test(rdm_fi_pdc, peek_claim_unique_tag)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_claim_unique_tag, 1, BUF_SZ);
}

static void pdc_peek_discard(int len)
{
	/* PEEK|DISCARD then PEEK test
	 *
	 * For each message size,
	 *   1. send one messages
	 *   2. peek|discard the receiver to find and discard message
	 *   3. peek the message and fail to find it
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents
	 */

	int ret;
	struct fi_msg_tagged msg;
	struct iovec iov;
	struct fi_cq_tagged_entry s_cqe;

	rdm_fi_pdc_init_data(source, len, 0xab);
	rdm_fi_pdc_init_data(target, len, 0);

	build_message(&msg, &iov, target, len, (void *) &rem_mr,
			gni_addr[0], source, len, 0);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK_DISCARD);

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], source, len, loc_mr,
					gni_addr[1], len, target);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK_DISCARD:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK | FI_DISCARD);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_DISCARD_WAIT_CQ,
					R_STATE_PEEK_DISCARD);
			break;
		case R_STATE_PEEK_DISCARD_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK_DISCARD,
						R_STATE_PEEK_DISCARD_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK,
						R_STATE_PEEK_DISCARD_WAIT_CQ);
			}
			break;
		case R_STATE_PEEK:
			ret = fi_trecvmsg(ep[1], &msg, FI_PEEK);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_PEEK_WAIT_ERR_CQ);
			break;
		case R_STATE_PEEK_WAIT_ERR_CQ:
			ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_PEEK_WAIT_ERR_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* verify test execution correctness */
	validate_cqe_contents(&s_cqe, TSEND_FLAGS, source, len, len, target);
	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK | FI_DISCARD);

	cr_assert(rdm_fi_pdc_check_data_pattern(target, 0, len),
			"Data matched");
}

Test(rdm_fi_pdc, peek_discard)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_discard, 1, BUF_SZ);
}

static void pdc_peek_discard_unique_tags(int len)
{
	/* PEEK|DISCARD then PEEK using unique tags test
	 *
	 * For each message size,
	 *   1. send two messages with unique tags and parameters
	 *   2. peek|discard the receiver to find and discard the first message
	 *   3. receive the second message
	 *   4. peek the first message and fail to find it
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the
	 * message contents
	 */
	int ret, i;
	struct fi_msg_tagged msg[2];
	struct iovec iov[2];
	struct fi_cq_tagged_entry s_cqe[2];
	struct fi_cq_tagged_entry d_cqe[2];
	struct fi_cq_tagged_entry *src_cqe[2] = {NULL, NULL};
	char *src_buf[2] = {source, source + len};
	char *dst_buf[2] = {target, target + len};
	void *src_context[2] = {target, target + len};

	/* initialize the initial data range on the source buffer to have
	 * different data vaules for one message than for the other
	 */
	rdm_fi_pdc_init_data_range(source, 0, len, 0xa5);
	rdm_fi_pdc_init_data_range(source, len, len, 0x5a);
	rdm_fi_pdc_init_data(target, len*2, 0);

	/* set up messages */
	for (i = 0; i < 2; i++) {
		build_message(&msg[i], &iov[i], dst_buf[i], len,
				(void *) &rem_mr, gni_addr[0],
				src_buf[i], len + i, 0);
	}

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg[0]);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK_DISCARD);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], src_buf[0], len, loc_mr,
					gni_addr[1], len, dst_buf[0]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_2);
			break;
		case S_STATE_SEND_MSG_2:
			ret = fi_tsend(ep[0], src_buf[1], len, loc_mr,
					gni_addr[1], len + 1, dst_buf[1]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[0], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_SEND_MSG_2_WAIT_CQ,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[1], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_2_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK_DISCARD:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK | FI_DISCARD);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_DISCARD_WAIT_CQ,
					R_STATE_PEEK_DISCARD);
			break;
		case R_STATE_PEEK_DISCARD_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK_DISCARD,
						R_STATE_PEEK_DISCARD_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_2,
						R_STATE_PEEK_DISCARD_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_2:
			ret = fi_trecvmsg(ep[1], &msg[1], 0);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_RECV_MSG_2_WAIT_CQ,
					R_STATE_RECV_MSG_2);
			break;
		case R_STATE_RECV_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[1], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_PEEK,
					R_STATE_RECV_MSG_2_WAIT_CQ);
			break;
		case R_STATE_PEEK:
			ret = fi_trecvmsg(ep[1], &msg[0], FI_PEEK);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_PEEK_WAIT_ERR_CQ);
			break;
		case R_STATE_PEEK_WAIT_ERR_CQ:
			ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_PEEK_WAIT_ERR_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* map src cqes to src parameters */
	map_src_cqes_to_src_context(s_cqe, src_cqe, src_context);

	/* verify test execution correctness */
	for (i = 0; i < 2; i++)
		validate_cqe_contents(src_cqe[i], TSEND_FLAGS,
				src_buf[i], len, len + i, dst_buf[i]);

	validate_cqe_with_message(&d_cqe[1], &msg[1], TRECV_FLAGS);
	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK | FI_DISCARD);

	cr_assert(rdm_fi_pdc_check_data_pattern(dst_buf[0], 0, len),
			"Data mismatch");
	cr_assert(rdm_fi_pdc_check_data(src_buf[1], dst_buf[1], len),
			"Data mismatch");
}

Test(rdm_fi_pdc, peek_discard_unique_tags)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_discard_unique_tags,
			1, BUF_SZ);
}

static void pdc_peek_claim_then_claim_discard(int len)
{
	/* PEEK|CLAIM then CLAIM|DISCARD test
	 *
	 * For each message size,
	 *   1. send two messages with unique tags and parameters
	 *   2. peek|claim the receiver to find and claim the first message
	 *   3. receive the second message
	 *   4. claim|discard the first message
	 *
	 *   From the fi_tagged man page:
	 *      Claimed messages can only be retrieved using a subsequent,
	 *      paired receive operation with the FI_CLAIM  flag  set.
	 *
	 *      (FI_DISCARD) flag may also be used in conjunction
	 *      with FI_CLAIM in order to retrieve and discard a message
	 *      previously claimed using an FI_PEEK + FI_CLAIM request.
	 *
	 * Test validation is done at the end of the test by verifying the
	 * contents of the CQEs after asserting that the send and recv
	 * sides have finished. Buffer contents should match the message
	 * contents
	 */
	int ret, i;
	struct fi_msg_tagged msg[2];
	struct iovec iov[2];
	struct fi_cq_tagged_entry s_cqe[2];
	struct fi_cq_tagged_entry d_cqe[2];
	struct fi_cq_tagged_entry *src_cqe[2] = {NULL, NULL};
	char *src_buf[2] = {source, source + len};
	char *dst_buf[2] = {target, target + len};
	void *src_context[2] = {target, target + len};

	/* initialize the initial data range on the source buffer to have
	 * different data values for one message than for the other
	 */
	rdm_fi_pdc_init_data_range(source, 0, len, 0xa5);
	rdm_fi_pdc_init_data_range(source, len, len, 0x5a);
	rdm_fi_pdc_init_data(target, len*2, 0);

	/* set up messages */
	for (i = 0; i < 2; i++) {
		build_message(&msg[i], &iov[i], dst_buf[i], len,
				(void *) &rem_mr, gni_addr[0],
				src_buf[i], len + i, 0);
	}

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg[0]);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK_CLAIM);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], src_buf[0], len, loc_mr,
					gni_addr[1], len, dst_buf[0]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_2);
			break;
		case S_STATE_SEND_MSG_2:
			ret = fi_tsend(ep[0], src_buf[1], len, loc_mr,
					gni_addr[1], len + 1, dst_buf[1]);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[0], 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_SEND_MSG_2_WAIT_CQ,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe[1], 1);
			COND_SEND_STATE_TRANSITION(ret, 1, S_STATE_DONE,
					S_STATE_SEND_MSG_2_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK_CLAIM:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK | FI_CLAIM);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_CLAIM_WAIT_CQ,
					R_STATE_PEEK_CLAIM);
			break;
		case R_STATE_PEEK_CLAIM_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK_CLAIM,
						R_STATE_PEEK_CLAIM_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_2,
						R_STATE_PEEK_CLAIM_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_2:
			ret = fi_trecvmsg(ep[1], &msg[1], 0);
			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_RECV_MSG_2_WAIT_CQ,
					R_STATE_RECV_MSG_2);
			break;
		case R_STATE_RECV_MSG_2_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[1], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_CLAIM_DISCARD,
					R_STATE_RECV_MSG_2_WAIT_CQ);
			break;
		case R_STATE_CLAIM_DISCARD:
			ret = fi_trecvmsg(ep[1], &msg[0],
					FI_CLAIM | FI_DISCARD);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_CLAIM_DISCARD_WAIT_CQ);
			break;
		case R_STATE_CLAIM_DISCARD_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe[0], 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_CLAIM_DISCARD_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	/* map src cqes to src parameters */
	map_src_cqes_to_src_context(s_cqe, src_cqe, src_context);

	/* verify test execution correctness */
	for (i = 0; i < 2; i++)
		validate_cqe_contents(src_cqe[i], TSEND_FLAGS,
				src_buf[i], len, len + i, dst_buf[i]);

	validate_cqe_with_message(&d_peek_cqe, &peek_msg,
			TRECV_FLAGS | FI_PEEK | FI_CLAIM);

	/* if CQE provided a buffer back, the data was copied.
	 * Check the data */
	if (d_peek_cqe.buf) {
		cr_assert(rdm_fi_pdc_check_data_pattern(peek_buffer, 0xa5, len),
				"Data mismatch");
	}

	validate_cqe_with_message(&d_cqe[0], &msg[0],
			TRECV_FLAGS | FI_CLAIM | FI_DISCARD);
	validate_cqe_with_message(&d_cqe[1], &msg[1], TRECV_FLAGS);

	cr_assert(rdm_fi_pdc_check_data_pattern(dst_buf[0], 0, len),
			"Data mismatch");
	cr_assert(rdm_fi_pdc_check_data(src_buf[1], dst_buf[1], len),
			"Data mismatch");
}

Test(rdm_fi_pdc, peek_claim_then_claim_discard)
{
	rdm_fi_pdc_xfer_for_each_size(pdc_peek_claim_then_claim_discard,
			1, BUF_SZ);
}

static void pdc_peek_event_present_small_buffer_provided(int len)
{
	/* Like pdc_peek_event_present_buffer_provided except uses an
	 * undersized receive buffer with the FI_PEEK request. */
	int ret, i;
	struct fi_msg_tagged msg;
	struct iovec iov;
	struct fi_cq_tagged_entry s_cqe;
	struct fi_cq_tagged_entry d_cqe;
	size_t cum_recv_len = 0;

	rdm_fi_pdc_init_data(source, len, 0xab);
	rdm_fi_pdc_init_data(target, len, 0);

	build_message(&msg, &iov, target, len, (void *) &rem_mr, gni_addr[0],
			source, len, 0);

	INIT_TEST_STATE(S_STATE_SEND_MSG_1, R_STATE_PEEK);

	/* we need to set up a peek buffer to ensure the contents of the peek
	 * are copied correctly. In the event of a discard, the data can be
	 * fetched with a peek, but the target buffer should remain untouched
	 */
	build_peek_message(&peek_msg, &msg);

	start_test_timer();
	do {
		PROGRESS_CQS(msg_cq);

		switch (s_state) {
		case S_STATE_SEND_MSG_1:
			ret = fi_tsend(ep[0], source, len, loc_mr,
					gni_addr[1], len, target);
			cr_assert_eq(ret, FI_SUCCESS);
			SEND_STATE_TRANSITION(S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_SEND_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[0], &s_cqe, 1);
			COND_SEND_STATE_TRANSITION(ret, 1,
					S_STATE_DONE,
					S_STATE_SEND_MSG_1_WAIT_CQ);
			break;
		case S_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		switch (r_state) {
		case R_STATE_PEEK:
			ret = fi_trecvmsg(ep[1], &peek_msg, FI_PEEK);

			COND_RECV_STATE_TRANSITION(ret, FI_SUCCESS,
					R_STATE_PEEK_WAIT_CQ,
					R_STATE_PEEK);
			break;
		case R_STATE_PEEK_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_peek_cqe, 1);
			if (ret == -FI_EAVAIL) {
				ret = fi_cq_readerr(msg_cq[1], &cqe_error, 0);
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_PEEK,
						R_STATE_PEEK_WAIT_CQ);
			} else {
				COND_RECV_STATE_TRANSITION(ret, 1,
						R_STATE_RECV_MSG_1,
						R_STATE_PEEK_WAIT_CQ);
			}
			break;
		case R_STATE_RECV_MSG_1:
			ret = fi_trecvmsg(ep[1], &msg, 0);
			cr_assert_eq(ret, FI_SUCCESS);
			RECV_STATE_TRANSITION(R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_RECV_MSG_1_WAIT_CQ:
			ret = fi_cq_read(msg_cq[1], &d_cqe, 1);
			COND_RECV_STATE_TRANSITION(ret, 1,
					R_STATE_DONE,
					R_STATE_RECV_MSG_1_WAIT_CQ);
			break;
		case R_STATE_DONE:
			break;
		default:
			RAISE_UNREACHABLE_STATE;
			break;
		}

		update_test_timer();
	} while (!TEST_TIME_LIMIT_EXPIRED && !SEND_RECV_DONE);

	ASSERT_SEND_RECV_DONE;

	for (i = 0; i < peek_msg.iov_count; i++) {
		cum_recv_len += peek_msg.msg_iov[i].iov_len;
	}

	/* validate the expected results */
	validate_cqe_contents(&s_cqe, TSEND_FLAGS, source, cum_recv_len, len, target);
	validate_cqe_contents(&d_peek_cqe, TRECV_FLAGS | FI_PEEK,
			peek_msg.msg_iov[0].iov_base, cum_recv_len, peek_msg.tag,
			peek_msg.context);

	/* if CQE provided a buffer back, the data was copied.
	 * Check the data */
	if (d_peek_cqe.buf) {
		cr_assert(rdm_fi_pdc_check_data_pattern(peek_buffer, 0xab,
				peek_iov.iov_len),
				"Data mismatch");
	}

	validate_cqe_with_message(&d_cqe, &msg, TRECV_FLAGS);

	cr_assert(rdm_fi_pdc_check_data(source, target, len), "Data mismatch");
}

Test(rdm_fi_pdc, peek_event_present_small_buff_provided)
{
	rdm_fi_pdc_xfer_for_each_size(
			pdc_peek_event_present_small_buffer_provided,
			1, BUF_SZ);
}
