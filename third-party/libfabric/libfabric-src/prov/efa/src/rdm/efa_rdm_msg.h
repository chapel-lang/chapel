/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

static inline
void efa_rdm_msg_construct(struct fi_msg *msg, const struct iovec *iov, void **desc,
		       size_t count, fi_addr_t addr, void *context, uint64_t data)
{
	msg->msg_iov = iov;
	msg->desc = desc;
	msg->iov_count = count;
	msg->addr = addr;
	msg->context = context;
	msg->data = data;
}

/**
 * functions to allocate rxe for two sided operations
 */
struct efa_rdm_ope *efa_rdm_msg_alloc_rxe_zcpy(struct efa_rdm_ep *ep,
					    const struct fi_msg *msg,
					    uint32_t op, uint64_t flags,
					    uint64_t tag, uint64_t ignore);

struct efa_rdm_ope *
efa_rdm_msg_alloc_rxe_for_msgrtm(struct efa_rdm_ep *ep,
				 struct efa_rdm_pke **pkt_entry_ptr);

struct efa_rdm_ope *
efa_rdm_msg_alloc_rxe_for_tagrtm(struct efa_rdm_ep *ep,
				 struct efa_rdm_pke **pkt_entry_ptr);

struct efa_rdm_ope *efa_rdm_msg_split_rxe(struct efa_rdm_ep *ep,
					    struct efa_rdm_ope *posted_entry,
					    struct efa_rdm_ope *consumer_entry,
					    struct efa_rdm_pke *pkt_entry);
ssize_t efa_rdm_msg_post_rtm(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe);
/*
 * The following 2 OP structures are defined in efa_rdm_msg.c and is
 * used by #efa_rdm_ep_open()
 */
extern struct fi_ops_msg efa_rdm_msg_ops;

extern struct fi_ops_tagged efa_rdm_msg_tagged_ops;