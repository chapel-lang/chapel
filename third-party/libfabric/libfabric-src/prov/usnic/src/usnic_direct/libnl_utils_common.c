/*
 * Copyright (c) 2014-2016, Cisco Systems, Inc. All rights reserved.
 *
 * LICENSE_BEGIN
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * LICENSE_END
 *
 *
 */
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#include "libnl_utils.h"
#include "usnic_user_utils.h"

#if USNIC_LOG_LVL >= USNIC_LOG_LVL_ERR
#define usnic_nlmsg_dump(msg) nl_msg_dump(msg, stderr)
#else
#define usnic_nlmsg_dump(msg)
#endif

/*
 * Querying the routing tables via netlink is expensive, especially
 * when many processes are doing so at the same time on a single
 * server (e.g., in an MPI job).  As such, we cache netlink responses
 * to alleviate pressure on the netlink kernel interface.
 */
 struct usd_nl_cache_entry {
	time_t timestamp;

	uint32_t src_ipaddr_be;
	uint32_t dest_ipaddr_be;
	int ifindex;
	uint32_t nh_addr;
	int reachable;

	/* For now, this cache is a simple linked list.  Eventually,
	 * this cache should be a better data structure, such as a
	 * hash table. */
	struct usd_nl_cache_entry *prev;
	struct usd_nl_cache_entry *next;
};

/* Semi-arbitrarily set cache TTL to 2 minutes */
static time_t usd_nl_cache_timeout = 120;

static struct usd_nl_cache_entry *cache = NULL;


static struct nla_policy route_policy[RTA_MAX+1] = {
	[RTA_IIF]	= { .type = NLA_STRING,
			    .maxlen = IFNAMSIZ, },
	[RTA_OIF]	= { .type = NLA_U32 },
	[RTA_PRIORITY]	= { .type = NLA_U32 },
	[RTA_FLOW]	= { .type = NLA_U32 },
	[RTA_MP_ALGO]	= { .type = NLA_U32 },
	[RTA_CACHEINFO]	= { .minlen = sizeof(struct rta_cacheinfo) },
	[RTA_METRICS]	= { .type = NLA_NESTED },
	[RTA_MULTIPATH]	= { .type = NLA_NESTED },
};

static int usnic_is_nlreply_expected(struct usnic_nl_sk *unlsk,
					struct nlmsghdr *nlm_hdr)
{
	if (nlm_hdr->nlmsg_pid != nl_socket_get_local_port(unlsk->nlh)
		|| nlm_hdr->nlmsg_seq != unlsk->seq) {
		usnic_err("Not an expected reply msg pid: %u local pid: %u msg seq: %u expected seq: %u\n",
				nlm_hdr->nlmsg_pid,
				nl_socket_get_local_port(unlsk->nlh),
				nlm_hdr->nlmsg_seq, unlsk->seq);
		return 0;
	}

	return 1;
}

static int usnic_is_nlreply_err(struct nlmsghdr *nlm_hdr,
                                struct usnic_rt_cb_arg *arg)
{
	if (nlm_hdr->nlmsg_type == NLMSG_ERROR) {
		struct nlmsgerr *e = (struct nlmsgerr *)nlmsg_data(nlm_hdr);
		if (nlm_hdr->nlmsg_len >= (__u32)NLMSG_SIZE(sizeof(*e))) {
			usnic_strerror(e->error,
					"Received a netlink error message");
			/* Sometimes nl_send() succeeds, but the
			 * request fails because the kernel is
			 * temporarily out of resources.  In these
			 * cases, we should tell the caller that they
			 * should try again. */
			if (e->error == -ECONNREFUSED) {
				arg->retry = 1;
			}
		} else
			usnic_err(
				"Received a truncated netlink error message\n");
		return 1;
	}

	return 0;
}

static int usnic_nl_send_query(struct usnic_nl_sk *unlsk, struct nl_msg *msg,
				int protocol, int flag)
{
	int ret, retry;
	struct nlmsghdr *nlhdr;

	nlhdr = nlmsg_hdr(msg);
	while (1) {
		nlhdr->nlmsg_pid = nl_socket_get_local_port(unlsk->nlh);
		nlhdr->nlmsg_seq = ++unlsk->seq;
		nlmsg_set_proto(msg, protocol);
		nlhdr->nlmsg_flags = flag;

		/* Sometimes nl_send() can fail simply because the
		 * kernel is temporarily out of resources, and we
		 * should just try again.  libnl1 and libnl3 handle
		 * this case a little differently, so use the
		 * USD_NL_SEND() macro to hide the differences.  If
		 * retry comes back as true, then sleep a little and
		 * try again. */
		USD_NL_SEND(unlsk->nlh, msg, ret, retry);
		if (retry) {
			usleep(5);
			continue;
		}
		break;
	}

	return ret;
}

static int usnic_nl_set_rcvsk_timer(NL_HANDLE *nlh)
{
	int err = 0;
	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	err = setsockopt(nl_socket_get_fd(nlh), SOL_SOCKET, SO_RCVTIMEO,
				(char *)&timeout, sizeof(timeout));
	if (err < 0)
		usnic_perr("Failed to set SO_RCVTIMEO for nl socket");

	return err;
}

static int usnic_nl_sk_alloc(struct usnic_nl_sk **p_sk, int protocol)
{
	struct usnic_nl_sk *unlsk;
	NL_HANDLE *nlh;
	int err;

	unlsk = calloc(1, sizeof(*unlsk));
	if (!unlsk) {
		usnic_err("Failed to allocate usnic_nl_sk struct\n");
		return ENOMEM;
	}

	nlh = NL_HANDLE_ALLOC();
	if (!nlh) {
		usnic_err("Failed to allocate nl handle\n");
		err = ENOMEM;
		goto err_free_unlsk;
	}

	err = nl_connect(nlh, protocol);
	if (err < 0) {
		usnic_err("Failed to connnect netlink route socket error: %s\n",
				NL_GETERROR(err));
                err = EINVAL;
		goto err_free_nlh;
	}

	NL_DISABLE_SEQ_CHECK(nlh);
	err = usnic_nl_set_rcvsk_timer(nlh);
	if (err < 0)
		goto err_close_nlh;

	unlsk->nlh = nlh;
	unlsk->seq = (uint32_t) time(NULL);
	*p_sk = unlsk;
	return 0;

err_close_nlh:
	nl_close(nlh);
err_free_nlh:
	NL_HANDLE_FREE(nlh);
err_free_unlsk:
	free(unlsk);
	return err;
}

static void usnic_nl_sk_free(struct usnic_nl_sk *unlsk)
{
	nl_close(unlsk->nlh);
	NL_HANDLE_FREE(unlsk->nlh);
	free(unlsk);
}

static int usnic_rt_raw_parse_cb(struct nl_msg *msg, void *arg)
{
	struct usnic_rt_cb_arg *lookup_arg = (struct usnic_rt_cb_arg *)arg;
	struct usnic_nl_sk *unlsk = lookup_arg->unlsk;
	struct nlmsghdr *nlm_hdr = nlmsg_hdr(msg);
	struct rtmsg *rtm;
	struct nlattr *tb[RTA_MAX + 1];
	int found = 0;
	int err;

	INC_CB_MSGCNT(lookup_arg);

	if (!usnic_is_nlreply_expected(unlsk, nlm_hdr)) {
		usnic_nlmsg_dump(msg);
		return NL_SKIP;
	}

	if (usnic_is_nlreply_err(nlm_hdr, lookup_arg)) {
		usnic_nlmsg_dump(msg);
		return NL_SKIP;
	}

	if (nlm_hdr->nlmsg_type != RTM_NEWROUTE) {
		char buf[128];
		nl_nlmsgtype2str(nlm_hdr->nlmsg_type, buf, sizeof(buf));
		usnic_err("Received an invalid route request reply message type: %s\n",
				buf);
		usnic_nlmsg_dump(msg);
		return NL_SKIP;
	}

	rtm = nlmsg_data(nlm_hdr);
	if (rtm->rtm_family != AF_INET) {
		usnic_err("RTM message contains invalid AF family: %u\n",
				rtm->rtm_family);
		usnic_nlmsg_dump(msg);
		return NL_SKIP;
	}

	err = nlmsg_parse(nlm_hdr, sizeof(struct rtmsg), tb, RTA_MAX,
			  route_policy);
	if (err < 0) {
		usnic_err("nlmsg parse error %s\n", NL_GETERROR(err));
		usnic_nlmsg_dump(msg);
		return NL_SKIP;
	}

	if (tb[RTA_OIF]) {
		if (nla_get_u32(tb[RTA_OIF]) == (uint32_t)lookup_arg->oif)
			found = 1;
		else
			usnic_err("Retrieved route has a different outgoing interface %d (expected %d)\n",
					nla_get_u32(tb[RTA_OIF]),
					lookup_arg->oif);
	}

	if (found && tb[RTA_GATEWAY])
		lookup_arg->nh_addr = nla_get_u32(tb[RTA_GATEWAY]);

	lookup_arg->found = found;
	return NL_STOP;
}


static struct usd_nl_cache_entry *
usd_nl_cache_lookup(uint32_t src_ipaddr_be, uint32_t dest_ipaddr_be, int ifindex)
{
	time_t now;
	struct usd_nl_cache_entry *nlce;
	struct usd_nl_cache_entry *stale;

	now = time(NULL);
	for (nlce = cache; NULL != nlce; ) {
		/* While we're traversing the cache, we might as well
		 * remove stale entries */
		if (now > nlce->timestamp + usd_nl_cache_timeout) {
			stale = nlce;
			nlce = nlce->next;

			if (stale->prev) {
				stale->prev->next = stale->next;
			}
			if (stale->next) {
				stale->next->prev = stale->prev;
			}
			if (cache == stale) {
				cache = nlce;
			}
			free(stale);

			continue;
		}

		if (nlce->src_ipaddr_be == src_ipaddr_be &&
			nlce->dest_ipaddr_be == dest_ipaddr_be &&
			nlce->ifindex == ifindex) {
			return nlce;
		}

		nlce = nlce->next;
	}

	return NULL;
}

static void
usd_nl_cache_save(int32_t src_ipaddr_be, uint32_t dest_ipaddr_be, int ifindex,
		uint32_t nh_addr, int reachable)
{
	struct usd_nl_cache_entry *nlce;

	nlce = calloc(1, sizeof(*nlce));
	if (NULL == nlce) {
		return;
	}

	nlce->timestamp = time(NULL);
	nlce->src_ipaddr_be = src_ipaddr_be;
	nlce->dest_ipaddr_be = dest_ipaddr_be;
	nlce->ifindex = ifindex;
	nlce->nh_addr = nh_addr;
	nlce->reachable = reachable;

	nlce->next = cache;
	if (cache) {
		cache->prev = nlce;
	}
	cache = nlce;
}


int usnic_nl_rt_lookup(uint32_t src_addr, uint32_t dst_addr, int oif,
			uint32_t *nh_addr)
{
	struct usnic_nl_sk	*unlsk;
	struct nl_msg		*nlm;
	struct rtmsg		rmsg;
	struct usnic_rt_cb_arg	arg;
	int			err;

	/* See if we have this NL result cached */
	struct usd_nl_cache_entry *nlce;
	nlce = usd_nl_cache_lookup(src_addr, dst_addr, oif);
	if (nlce) {
		if (nlce->reachable) {
			*nh_addr = nlce->nh_addr;
			return 0;
		} else {
			return EHOSTUNREACH;
		}
	}

retry:
	unlsk = NULL;
	err = usnic_nl_sk_alloc(&unlsk, NETLINK_ROUTE);
	if (err)
		return err;

	memset(&rmsg, 0, sizeof(rmsg));
	rmsg.rtm_family = AF_INET;
	rmsg.rtm_dst_len = sizeof(dst_addr) * CHAR_BIT;
	rmsg.rtm_src_len = sizeof(src_addr) * CHAR_BIT;

	nlm = nlmsg_alloc_simple(RTM_GETROUTE, 0);
	if (!nlm) {
		usnic_err("Failed to alloc nl message, %s\n",
				NL_GETERROR(err));
		err = ENOMEM;
		goto out;
	}
	nlmsg_append(nlm, &rmsg, sizeof(rmsg), NLMSG_ALIGNTO);
	nla_put_u32(nlm, RTA_DST, dst_addr);
	nla_put_u32(nlm, RTA_SRC, src_addr);

	err = usnic_nl_send_query(unlsk, nlm, NETLINK_ROUTE, NLM_F_REQUEST);
	nlmsg_free(nlm);
	if (err < 0) {
		usnic_err("Failed to send RTM_GETROUTE query message, error %s\n",
				NL_GETERROR(err));
                err = EINVAL;
		goto out;
	}

	memset(&arg, 0, sizeof(arg));
	arg.oif		= oif;
	arg.unlsk	= unlsk;
	err = nl_socket_modify_cb(unlsk->nlh, NL_CB_MSG_IN, NL_CB_CUSTOM,
					usnic_rt_raw_parse_cb, &arg);
	if (err != 0) {
		usnic_err("Failed to setup callback function, error %s\n",
				NL_GETERROR(err));
                err = EINVAL;
		goto out;
	}

	/* Sometimes the recvmsg can fail because something is
	 * temporarily out of resources.  In this case, delay a little
	 * and try again. */
	do {
		err = 0;
		NL_RECVMSGS(unlsk->nlh, arg, EAGAIN, err, out);
		if (err == EAGAIN) {
			usleep(5);
		}
	} while (err == EAGAIN);

	/* If we got a reply back that indicated that the kernel was
	 * too busy to handle this request, delay a little and try
	 * again. */
        if (arg.retry) {
            usleep(5);
            goto retry;
        }

	if (arg.found) {
		*nh_addr = arg.nh_addr;
		err = 0;
	} else {
		err = EHOSTUNREACH;
	}

	/* Save this result in the cache */
	usd_nl_cache_save(src_addr, dst_addr, oif,
			arg.nh_addr, arg.found);

out:
	usnic_nl_sk_free(unlsk);
	return err;
}

