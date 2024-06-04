/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * (c) Copyright 2021-2023 Hewlett Packard Enterprise Development LP
 */

#ifndef FRMWK_HEADER
#define FRMWK_HEADER

union nicaddr {
	uint64_t value;
	struct {
		uint64_t nic:20;	// 20-bit CXI NIC address
		uint64_t net:28;	// 28-bit network route
		uint64_t hsn:2;		// up to 4 CXI chips per node
		uint64_t rank:14;	// up to 16k ranks
	} __attribute__((__packed__));
};
#define	NICSIZE	(sizeof(union nicaddr))

/* These are initialized by frmwk_init() */
extern int frmwk_nics_per_rank;		/* PMI_NUM_HSNS (defaults to 1) */
extern int frmwk_numranks;		/* PMI_SIZE */
extern const char *frmwk_unique;	/* PMI_SHARED_SECRET */
extern int frmwk_rank;			/* PMI_RANK */
extern int frmwk_hwcoll_addrs_per_job;	/* FI_CXI_HWCOLL_ADDRS_PER_JOB */
extern int frmwk_hwcoll_min_nodes;	/* FI_CXI_HWCOLL_MIN_NODES */
extern const char *frmwk_jobid;		/* FI_CXI_COLL_JOB_ID */
extern const char *frmwk_jobstep;	/* FI_CXI_COLL_JOB_STEP_ID */
extern const char *frmwk_mcast_token;	/* FI_CXI_COLL_MCAST_TOKEN */
extern const char *frmwk_fabric_mgr_url;/* FI_CXI_COLL_FABRIC_MGR_URL */
extern const char *frmwk_nodename;	/* SLURMD_NODENAME */
extern const char frmwk_node0[32];	/* SLURMD_NODELIST (first name) */
extern union nicaddr *frmwk_nics;	/* array of NIC addresses  */
extern int frmwk_numnics;		/* number of NIC addresses */

extern char *cxit_node;
extern char *cxit_service;
extern uint64_t cxit_flags;
extern struct fi_info *cxit_fi_hints;
extern struct fi_info *cxit_fi;

extern struct fid_fabric *cxit_fabric;
extern struct fid_domain *cxit_domain;
extern struct fi_cxi_dom_ops *dom_ops;

extern struct fid_ep *cxit_ep;
extern struct fi_eq_attr cxit_eq_attr;
extern uint64_t cxit_eq_bind_flags;
extern struct fid_eq *cxit_eq;

extern struct fi_cq_attr cxit_rx_cq_attr;
extern uint64_t cxit_rx_cq_bind_flags;
extern struct fid_cq *cxit_rx_cq;

extern struct fi_cq_attr cxit_tx_cq_attr;
extern uint64_t cxit_tx_cq_bind_flags;
extern struct fid_cq *cxit_tx_cq;

extern fi_addr_t cxit_ep_fi_addr;

extern struct fi_cntr_attr cxit_cntr_attr;
extern struct fid_cntr *cxit_send_cntr;
extern struct fid_cntr *cxit_recv_cntr;
extern struct fid_cntr *cxit_read_cntr;
extern struct fid_cntr *cxit_write_cntr;
extern struct fid_cntr *cxit_rem_cntr;

extern struct fi_av_attr cxit_av_attr;
extern struct fid_av *cxit_av;

extern int cxit_n_ifs;
extern struct fid_av_set *cxit_av_set;
extern struct fid_mc *cxit_mc;
extern fi_addr_t cxit_mc_addr;

int frmwk_allgather(size_t size, void *data, void *rslt);
int frmwk_barrier(void);
int frmwk_gather_nics(void);
int frmwk_nic_addr(int rank, int hsn);

void frmwk_init(bool quiet);
void frmwk_term(void);
int frmwk_init_libfabric(void);
void frmwk_free_libfabric(void);
int frmwk_check_env(int minranks);
int frmwk_populate_av(fi_addr_t **fiaddr, size_t *size);
int frmwk_errmsg(int ret, const char *fmt, ...)
	__attribute__((format(__printf__, 2, 3)));
int frmwk_log0(const char *fmt, ...)
	__attribute__((format(__printf__, 1, 2)));
int frmwk_log(const char *fmt, ...)
	__attribute__((format(__printf__, 1, 2)));

#endif /* FRMWK_HEADER */
