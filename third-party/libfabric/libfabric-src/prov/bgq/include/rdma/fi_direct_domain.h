/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#ifndef _FI_PROV_BGQ_DOMAIN_H_
#define _FI_PROV_BGQ_DOMAIN_H_

#define FABRIC_DIRECT_DOMAIN 1

#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#include "rdma/bgq/fi_bgq_spi.h"

#include "rdma/bgq/fi_bgq_l2atomic.h"
#include "rdma/bgq/fi_bgq_progress.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fi_bgq_ep;	/* forward declaration */

struct fi_bgq_bat_entry {
	uintptr_t	vaddr;
	uint64_t	paddr;
};

#define FI_BGQ_DOMAIN_MAX_TX_CTX ((BGQ_MU_NUM_INJ_FIFO_GROUPS-1)*BGQ_MU_NUM_INJ_FIFOS_PER_GROUP)	/* defensively set to the number of mu injection fifos per node */
#define FI_BGQ_DOMAIN_MAX_RX_CTX ((BGQ_MU_NUM_REC_FIFO_GROUPS-1)*BGQ_MU_NUM_REC_FIFOS_PER_GROUP)	/* defensively set to the number of mu reception fifos per node */
struct fi_bgq_domain {
	struct fid_domain	domain_fid;
	struct fi_bgq_fabric	*fabric;

	enum fi_threading	threading;
	enum fi_resource_mgmt	resource_mgmt;
	enum fi_mr_mode		mr_mode;
	enum fi_progress	data_progress;

	struct {
		struct l2atomic_lock	lock;
	} mu;

	struct {
		uint32_t	max_fifos;
		uint32_t	max;
		uint32_t	count;
		MUSPI_RecFifo_t		*rfifo[BGQ_MU_NUM_REC_FIFOS_PER_GROUP*(BGQ_MU_NUM_REC_FIFO_GROUPS-1)];	/* do not mess with 17th core rec fifos */
		struct fi_bgq_ep	*ctx[FI_BGQ_DOMAIN_MAX_RX_CTX];
	} rx;

	struct {
		uint32_t	count;
		uint8_t		rget_subgroup_base;
	} tx;

	uint64_t		num_mr_keys;
	struct fi_bgq_bat_entry	*bat;	/* only for FI_MR_SCALABLE */

	BG_CoordinateMapping_t	my_coords;
	struct l2atomic_lock	lock;
	void			*rfifo_mem;
	MUSPI_RecFifoSubGroup_t	rfifo_subgroup[BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE];
	MUSPI_InjFifoSubGroup_t	ififo_subgroup[BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE];

	struct {
		MUSPI_GIBarrier_t	barrier;
		uint32_t		is_leader;
		uint32_t		leader_tcoord;
	} gi;

	struct {
		uint64_t		value;
		uint64_t		paddr;
	} zero;
	struct {
		uint64_t		value;
		uint64_t		paddr;
	} one;

	uint64_t		max_ep;


	struct {
		struct fi_bgq_progress	thread[64];
		uint64_t		max_threads;
		uint64_t		num_threads_active;
		void			*memptr;

	} progress;


	uint64_t		subgroups_per_process;
	struct l2atomic_counter	ref_cnt;
};

struct fi_bgq_av {
	struct fid_av		av_fid;
	struct fi_bgq_domain	*domain;
	enum fi_av_type		type;
	void			*map_addr;
	struct l2atomic_counter	ref_cnt;
};

struct fi_bgq_mr {
	struct fid_mr		mr_fid;
	struct fi_bgq_domain	*domain;
	const void		*buf;
	size_t			len;
	size_t			offset;
	uint64_t		access;
	uint64_t		flags;
	uint64_t		cntr_bflags;
	struct fi_bgq_cntr	*cntr;
	struct fi_bgq_ep	*ep;
};

static inline void
fi_bgq_domain_bat_read(struct fi_bgq_bat_entry * bat, uint64_t key, uintptr_t *vaddr, uint64_t *paddr)
{
	assert(bat);
	*vaddr = bat[key].vaddr;
	*paddr = bat[key].paddr;
}

static inline void *
fi_bgq_domain_bat_read_vaddr(struct fi_bgq_bat_entry * bat, uint64_t key)
{
	assert(bat);
	return (void*)bat[key].vaddr;
}

static inline uint64_t
fi_bgq_domain_bat_read_paddr(struct fi_bgq_bat_entry * bat, uint64_t key)
{
	assert(bat);
	return bat[key].paddr;
}

static inline void
fi_bgq_domain_bat_write(struct fi_bgq_domain *bgq_domain, uint64_t requested_key, const void *buf, size_t len)
{
	assert(requested_key < bgq_domain->num_mr_keys);

	bgq_domain->bat[requested_key].vaddr = (uintptr_t)buf;
	if (buf == NULL) {
		bgq_domain->bat[requested_key].paddr = 0;
	} else {
		Kernel_MemoryRegion_t cnk_mr;
		uint32_t cnk_rc __attribute__ ((unused));
		cnk_rc = Kernel_CreateMemoryRegion(&cnk_mr, (void *)buf, len);
		assert(cnk_rc == 0);

		bgq_domain->bat[requested_key].paddr =
			(uint64_t)cnk_mr.BasePa + ((uint64_t)buf - (uint64_t)cnk_mr.BaseVa);
	}

	{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
		volatile uint64_t *mu_register =
			(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
				0x030 - PHYMAP_PRIVILEGEDOFFSET);
		*mu_register = 0;
	}
	ppc_msync();
}

static inline void
fi_bgq_domain_bat_clear(struct fi_bgq_domain *bgq_domain, uint64_t key)
{
	assert(key < bgq_domain->num_mr_keys);

	bgq_domain->bat[key].vaddr = (uintptr_t)0;
	bgq_domain->bat[key].paddr = (uint64_t)0;

	{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
		volatile uint64_t *mu_register =
			(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
				0x030 - PHYMAP_PRIVILEGEDOFFSET);
		*mu_register = 0;
	}
	ppc_msync();
}

static inline uint32_t
fi_bgq_domain_get_tx_max(struct fi_bgq_domain *bgq_domain) {

	/*
	 * The maximum number of tx contexts depends on how many mu injection
	 * fifos are available and how many rx contexts have been allocated -
	 * each tx context requires 2 mu injection fifos, and each allocated
	 * rx context consumes an additional mu injection fifo.
	 */

	const uint32_t ppn = Kernel_ProcessCount();
	return ((FI_BGQ_DOMAIN_MAX_TX_CTX / ppn) - bgq_domain->rx.count) / 2;
}

static inline uint32_t
fi_bgq_domain_get_rx_max(struct fi_bgq_domain *bgq_domain) {

	/*
	 * The maximum number of rx contexts depends on how many mu reception
	 * fifos are available and how many tx contexts have been allocated -
	 * each rx context requires 1 mu reception fifo and 1 mu injection fifo
	 */

	const uint32_t ppn = Kernel_ProcessCount();

	return MIN((FI_BGQ_DOMAIN_MAX_RX_CTX / ppn),((FI_BGQ_DOMAIN_MAX_TX_CTX / ppn) - (bgq_domain->tx.count * 2)));
}


#ifdef FABRIC_DIRECT
static inline int
fi_domain(struct fid_fabric *fabric, struct fi_info *info,
	   struct fid_domain **domain, void *context)
{
	return fabric->ops->domain(fabric, info, domain, context);
}

static inline int
fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	   struct fid_cq **cq, void *context)
{
	return domain->ops->cq_open(domain, attr, cq, context);
}

static inline int
fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
	      struct fid_cntr **cntr, void *context)
{
	return domain->ops->cntr_open(domain, attr, cntr, context);
}

static inline int
fi_mr_reg(struct fid_domain *domain, const void *buf, size_t len,
	  uint64_t access, uint64_t offset, uint64_t requested_key,
	  uint64_t flags, struct fid_mr **mr, void *context)
{
	return domain->mr->reg(&domain->fid, buf, len, access, offset,
			       requested_key, flags, mr, context);
}

static inline
int fi_mr_bind(struct fid_mr *mr, struct fid *bfid, uint64_t flags)
{
	return mr->fid.ops->bind(&mr->fid, bfid, flags);
}

static inline
void *fi_mr_desc(struct fid_mr *mr)
{
	return mr->mem_desc;
}

static inline
uint64_t fi_mr_key(struct fid_mr *mr)
{
	return mr->key;
}

static inline int
fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
	   struct fid_av **av, void *context)
{
	return domain->ops->av_open(domain, attr, av, context);
}

static inline int
fi_av_insert(struct fid_av *av, const void *addr, size_t count,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insert(av, addr, count, fi_addr, flags, context);
}

static inline int
fi_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsvc(av, node, service, fi_addr, flags, context);
}

static inline int
fi_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsym(av, node, nodecnt, service, svccnt, fi_addr, flags, context);
}

static inline int
fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
	return av->ops->lookup(av, fi_addr, addr, addrlen);
}

static inline int
fi_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return av->ops->remove(av, fi_addr, count, flags);
}

static inline fi_addr_t
fi_rx_addr(fi_addr_t fi_addr, int rx_index, int rx_ctx_bits)
{
	/*
	 * The rx information for bgq is the rec fifo id, this is stored
	 * across the bits in the 'rx_lsb' and 'rx_msb'
	 * fields in the uid, these fields should be concatenated to determine
	 * the rx base fifo id, then the rx_index should be added to this to identify
	 * the correct rec fifo id for this rx and then restored in the 'rx_lsb'
	 * and 'rx_msb' bits to support scalable endpoints.
	 *
	 */

	union fi_bgq_addr bgq_addr = {.fi=fi_addr};

	uint32_t rec_fifo_id = 0;
	rec_fifo_id = (rec_fifo_id | (((bgq_addr.uid.fi & 0xF0000000u) >> 23) | ((bgq_addr.uid.fi & 0x0000003Eu) >> 1))) + rx_index;
	bgq_addr.uid.fi = (bgq_addr.uid.fi & 0x0FFFFFC1u) |            /* clear rx_msb and rx_lsb */
                ((rec_fifo_id << 23) & 0xF0000000u) |    /* set rx_msb */
                ((rec_fifo_id << 1) & 0x0000003Eu);	/* set rx_lsb */

	return bgq_addr.fi;
}

static inline int fi_wait_open(struct fid_fabric *fabric,
			       struct fi_wait_attr *attr,
			       struct fid_wait **waitset)
{
	return -FI_ENOSYS;		/* TODO - implement this */
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_BGQ_DOMAIN_H_ */
