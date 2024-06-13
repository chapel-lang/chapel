/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2023 Hewlett Packard Enterprise Development LP
 */

/*
 * libfabric C test framework for multinode testing.
 *
 * This must be compiled with:
 *
 * - PLATFORM_CASSINI_HW=1 (or other hardware flag)
 *
 * Tests are run using srun: $ srun -Nn ./test_frmwk 'n' is the number of nodes
 * to use. Some tests may place requirements on 'n'.
 *
 * frmwk_init_libfabric() sets up
 * - generic fabric info for CXI driver
 * - one domain (fabric address)
 * - one endpoint
 * - one of each of the following
 *   - eq
 *   - tx cq
 *   - rx cq
 *   - send cntr
 *   - recv cntr
 *   - read cntr
 *   - write cntr
 *   - remote cntr
 *
 * frmwk_populate_av() uses a sockets-based Allgather operation to collect local
 * HSN addresses and distribute them over the entire set of nodes, and then
 * creates and binds the fi_av object for the endpoint. This 'populate' function
 * has been separated out from initialization, to allow the framework to use
 * other means of population (e.g. MPI). The following environment variables are
 * significant:
 * - PMI_SIZE		(WLM)  number of ranks in job (from WLM)
 * - PMI_RANK		(WLM)  rank of this process   (from WLM)
 * - PMI_SHARED_SECRET	(WLM)  unique job identifier  (from WLM)
 * - PMI_NUM_HSNS	(USER) optional, defaults to 1
 * - PMI_HOME		(USER) optional, preferred file system directory to use
 * - HOME		(USER) default file system directory to use
 *
 * frmwk_enable_libfabric() can be used after the fi_av object has been
 * initialized.
 *
 * frmwk_free_libfabric() terminates the libfabric instance and cleans up.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <time.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <ofi.h>
#include <cxip.h>

#include "multinode_frmwk.h"

/* If not compiled with DEBUG=1, this is a no-op */
#define	TRACE(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_TEST_CODE, fmt, ##__VA_ARGS__)

#define RETURN_ERROR(ret, txt) \
	if (ret != FI_SUCCESS) { \
		fprintf(stderr, "FAILED %s = %s\n", txt, fi_strerror(-ret)); \
		return ret; \
	}

#define	CLOSE_OBJ(obj)	do {if (obj) fi_close(&obj->fid); } while (0)

/* Taken from SLURM environment variables */
int frmwk_nics_per_rank;		/* PMI_NUM_HSNS (defaults to 1) */
int frmwk_numranks;			/* PMI_SIZE */
const char *frmwk_unique;		/* PMI_SHARED_SECRET */
int frmwk_rank;				/* PMI_RANK */
int frmwk_hwcoll_addrs_per_job;		/* FI_CXI_HWCOLL_ADDRS_PER_JOB */
int frmwk_hwcoll_min_nodes;		/* FI_CXI_HWCOLL_MIN_NODES */
const char *frmwk_jobid;		/* FI_CXI_COLL_JOB_ID */
const char *frmwk_jobstep;		/* FI_CXI_COLL_JOB_STEP_ID */
const char *frmwk_mcast_token;		/* FI_CXI_COLL_MCAST_TOKEN */
const char *frmwk_fabric_mgr_url;	/* FI_CXI_COLL_FABRIC_MGR_URL */
const char *frmwk_nodename;		/* SLURMD_NODENAME */
const char frmwk_node0[32];		/* SLURMD_NODELIST (first name) */
union nicaddr *frmwk_nics;		/* array of NIC addresses  */
int frmwk_numnics;			/* number of NIC addresses */

int _frmwk_init;

char *cxit_node;
char *cxit_service;
uint64_t cxit_flags;
struct fi_info *cxit_fi_hints;
struct fi_info *cxit_fi;

struct fid_fabric *cxit_fabric;
struct fid_domain *cxit_domain;
struct fi_cxi_dom_ops *cxit_dom_ops;

struct mem_region {
	uint8_t *mem;
	struct fid_mr *mr;
};

struct fid_ep *cxit_ep;
struct fi_eq_attr cxit_eq_attr = {
	.size = 32,
	.flags = FI_WRITE,
	.wait_obj = FI_WAIT_NONE
};
uint64_t cxit_eq_bind_flags = 0;

struct fid_eq *cxit_eq;

struct fi_cq_attr cxit_rx_cq_attr = {
	.format = FI_CQ_FORMAT_TAGGED

};
uint64_t cxit_rx_cq_bind_flags = FI_RECV;
struct fid_cq *cxit_rx_cq;

struct fi_cq_attr cxit_tx_cq_attr = {
	.format = FI_CQ_FORMAT_TAGGED,
	.size = 16384
};
uint64_t cxit_tx_cq_bind_flags = FI_TRANSMIT;
struct fid_cq *cxit_tx_cq;

fi_addr_t cxit_ep_fi_addr;

struct fi_cntr_attr cxit_cntr_attr = {};
struct fid_cntr *cxit_send_cntr;
struct fid_cntr *cxit_recv_cntr;
struct fid_cntr *cxit_read_cntr;
struct fid_cntr *cxit_write_cntr;
struct fid_cntr *cxit_rem_cntr;

struct fi_av_attr cxit_av_attr = {
	.type = FI_AV_TABLE,
	.rx_ctx_bits = 0
};
struct fid_av *cxit_av;

int cxit_n_ifs;
struct fid_av_set *cxit_av_set;
struct fid_mc *cxit_mc;
fi_addr_t cxit_mc_addr;

/* HMEM memory functional overlays */
int mr_create_ext(size_t len, uint64_t access, uint8_t seed, uint64_t key,
		  struct fid_cntr *cntr, struct mem_region *mr)
{
	int ret;

	if (len) {
		mr->mem = calloc(1, len);
		ret = (mr->mem != NULL) ? FI_SUCCESS : FI_ENOMEM;
		RETURN_ERROR(ret, __func__);
	} else {
		mr->mem = 0;
	}

	for (size_t i = 0; i < len; i++)
		mr->mem[i] = i + seed;

	ret = fi_mr_reg(cxit_domain, mr->mem, len, access, 0, key, 0,
			&mr->mr, NULL);
	RETURN_ERROR(ret, "fi_mr_reg");

	ret = fi_mr_bind(mr->mr, &cxit_ep->fid, 0);
	RETURN_ERROR(ret, "fi_mr_bind ep");

	if (cntr) {
		ret = fi_mr_bind(mr->mr, &cntr->fid, FI_REMOTE_WRITE);
		RETURN_ERROR(ret, "fi_mr_bind cntr");
	}

	ret = fi_mr_enable(mr->mr);
	RETURN_ERROR(ret, "fi_mr_enable");

	return 0;
}

static ssize_t copy_from_hmem_iov(void *dest, size_t size,
				  enum fi_hmem_iface iface, uint64_t device,
				  const struct iovec *hmem_iov,
				  size_t hmem_iov_count,
				  uint64_t hmem_iov_offset)
{
	size_t cpy_size = MIN(size, hmem_iov->iov_len);

	assert(iface == FI_HMEM_SYSTEM);
	assert(hmem_iov_count == 1);
	assert(hmem_iov_offset == 0);

	memcpy(dest, hmem_iov->iov_base, cpy_size);

	return cpy_size;
}

static ssize_t copy_to_hmem_iov(enum fi_hmem_iface iface, uint64_t device,
				const struct iovec *hmem_iov,
				size_t hmem_iov_count,
				uint64_t hmem_iov_offset, const void *src,
				size_t size)
{
	size_t cpy_size = MIN(size, hmem_iov->iov_len);

	assert(iface == FI_HMEM_SYSTEM);
	assert(hmem_iov_count == 1);
	assert(hmem_iov_offset == 0);

	memcpy(hmem_iov->iov_base, src, cpy_size);

	return cpy_size;
}

struct fi_hmem_override_ops cxit_hmem_ops = {
	.copy_from_hmem_iov = copy_from_hmem_iov,
	.copy_to_hmem_iov = copy_to_hmem_iov,
};

/* A minimal generic context for use with asynchronous operations */
struct mycontext {
	int rx_err;
	int rx_prov_err;
	int tx_err;
	int tx_prov_err;
};

/* display message on stdout from rank 0 */
int frmwk_log0(const char *fmt, ...)
{
	va_list args;
	int len = 0;

	if (_frmwk_init && frmwk_rank != 0)
		return 0;

	va_start(args, fmt);
	len = vfprintf(stdout, fmt, args);
	va_end(args);
	fflush(stdout);
	return len;
}

/* display message with rank designation */
int frmwk_log(const char *fmt, ...)
{
	va_list args;
	int len = 0;

	if (_frmwk_init)
		len += fprintf(stdout, "[%2d] ", frmwk_rank);
	va_start(args, fmt);
	len += vfprintf(stdout, fmt, args);
	va_end(args);
	fflush(stdout);
	return len;
}

/* Implement a simple sockets-based allgather for testing.
 *
 * This selects one node across all of the nodes to serve as a local root, and
 * then uses sockets to transfer information.
 */
#define	FAIL(cond, msg, label) \
	if (cond) { \
		fprintf(stderr, "FAIL socket %s=%d\n", msg, cond); \
		goto label; \
	}

/* Sockets can chop up large reads */
static ssize_t _fullread(int fd, char *ptr, ssize_t size)
{
	ssize_t rem = size;
	ssize_t len;

	while (rem > 0) {
		len = read(fd, ptr, rem);
		if (len < 0)
			return len;
		ptr += len;
		rem -= len;
	}
	return size;
}

/* Sockets can chop up large writes */
static ssize_t _fullwrite(int fd, char *ptr, ssize_t size)
{
	ssize_t rem = size;
	ssize_t len;

	while (rem > 0) {
		len = write(fd, ptr, rem);
		if (len < 0)
			return len;
		ptr += len;
		rem -= len;
	}
	return size;
}

/* frmwk_node0 (first in list) serves as root */
int _accept(int portno, size_t size, void *data, void *rslt)
{
	int listenfd = 0;
	int *connfd, conncnt, connidx;
	struct sockaddr_in serv_addr = { 0 };
	char *rsltp;
	size_t siz;
	ssize_t len;
	int error, ret;

	// any early exit reports failure
	error = -1;

	// create the socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	FAIL(listenfd < 0, "socket", lablisten);

	// release the socket immediately after termination
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
			 &(int){1}, sizeof(int));
	FAIL(ret < 0, "reuseaddr", lablisten);

	// bind the socket to accept any incoming connections
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	ret = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	FAIL(ret < 0, "bind", lablisten);

	// limit the number of connections
	conncnt = frmwk_numranks - 1;
	ret = listen(listenfd, conncnt);
	FAIL(ret < 0, "listen", lablisten);

	// create the connection array
	connfd = calloc(conncnt, sizeof(*connfd));
	FAIL(!connfd, "connfd", lablisten);

	// initialize to invalid file descriptors
	for (connidx = 0; connidx < conncnt; connidx++)
		connfd[connidx] = -1;

	// add our contribution to the result
	rsltp = rslt;
	memcpy(rsltp, data, size);
	rsltp += size;

	// accept connections and start the root protocol
	for (connidx = 0; connidx < conncnt; connidx++) {
		int fd;

		fd = accept(listenfd, (struct sockaddr *)NULL, NULL);
		FAIL(fd < 0, "accept", labclose);

		// record this for later send
		connfd[connidx] = fd;

		// read from the connection
		siz = size;
		len = _fullread(fd, rsltp, siz);
		FAIL(len < siz, "read", labclose);

		// advance the result pointer
		rsltp += siz;
	}

	// all contributions complete, send the result
	for (connidx = 0; connidx < conncnt; connidx++)
	{
		int fd;

		fd = connfd[connidx];
		siz = frmwk_numranks * size;
		len = _fullwrite(fd, rslt, siz);
		FAIL(len < siz, "write", labclose);
	}

	// report success
	error = 0;

labclose:
	for (connidx = 0; connidx < conncnt; connidx++)
		close(connfd[connidx]);
	free(connfd);
lablisten:
	close(listenfd);
	return error;
}

/* nodes other than frmwk_node0 serve as leaves */
int _connect(int portno, size_t size, void *data, void *rslt)
{
	int connfd = 0;
	struct sockaddr_in serv_addr = { 0 };
	struct hostent *he;
	struct in_addr **addr_list;
	size_t siz;
	ssize_t len;
	int error, ret;

	// any early exit returns error
	error = -1;

	// create the socket
	connfd = socket(AF_INET, SOCK_STREAM, 0);
	FAIL(connfd < 0, "socket", labclose);

	// release the socket immediately after termination
	ret = setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR,
			 &(int){1}, sizeof(int));
	FAIL(ret < 0, "reuseaddr", labclose);

	// get network address of frmwk_node0 and connect socket
	he = gethostbyname(frmwk_node0);
	FAIL(!he, "gethostbyname", labclose);

	addr_list = (struct in_addr **)he->h_addr_list;
	FAIL(!addr_list, "gethostbyname empty", labclose);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr = *addr_list[0];
	do {
		usleep(1000);
		ret = connect(connfd, (struct sockaddr *)&serv_addr,
			      sizeof(serv_addr));
	} while (ret < 0);

	// write our data
	siz = size;
	len = _fullwrite(connfd, data, siz);
	FAIL(len < siz, "write", labclose);

	// wait for full data response
	siz = frmwk_numranks * size;
	len = _fullread(connfd, rslt, siz);
	FAIL(len < siz, "read", labclose);

	// report success
	error = 0;

labclose:
	close(connfd);
	return error;
}

int frmwk_allgather(size_t size, void *data, void *rslt)
{
	int portno = 5000;

	return (!strcmp(frmwk_node0, frmwk_nodename)) ?
		_accept(portno, size, data, rslt) :
		_connect(portno, size, data, rslt);
}

int frmwk_barrier(void)
{
	ssize_t size = sizeof(char);
	char data = 0;
	char *rslt;
	int ret;

	rslt = calloc(frmwk_numranks, sizeof(char));
	ret = frmwk_allgather(size, &data, rslt);
	free(rslt);

	return ret;
}

/**
 * @brief Check for minimum number of ranks
 *
 * @param minranks required minimum number of ranks
 * @return int error code, 0 on success
 */
int frmwk_check_env(int minranks)
{
	if (!_frmwk_init) {
		fprintf(stderr, "Framework not initialized\n");
		return -1;
	}
	if (frmwk_numranks < minranks) {
		/* only one rank makes noise */
		if (!frmwk_rank)
			fprintf(stderr, "Requires >= %d ranks\n", minranks);
		return -1;
	}
	return 0;
}

/**
 * @brief Shut down the libfabric test framework.
 */
void frmwk_free_libfabric(void)
{
	/* must close EP before closing anything bound to it */
	CLOSE_OBJ(cxit_ep);
	CLOSE_OBJ(cxit_av);
	CLOSE_OBJ(cxit_rem_cntr);
	CLOSE_OBJ(cxit_write_cntr);
	CLOSE_OBJ(cxit_read_cntr);
	CLOSE_OBJ(cxit_recv_cntr);
	CLOSE_OBJ(cxit_send_cntr);
	CLOSE_OBJ(cxit_eq);
	CLOSE_OBJ(cxit_tx_cq);
	CLOSE_OBJ(cxit_rx_cq);
	CLOSE_OBJ(cxit_domain);
	CLOSE_OBJ(cxit_fabric);
	fi_freeinfo(cxit_fi);
	fi_freeinfo(cxit_fi_hints);
}

/**
 * @brief Initialize the libfabric test framework.
 *
 * The ep_obj->src_addr has a PID value of 511 (PID_ANY) until the EP is
 * enabled, at which point the actual PID is assigned. Nothing works if the PIDs
 * are mismatched between ranks.
 *
 * @return int error code, 0 on success
 */
int frmwk_init_libfabric(void)
{
	int ret;

	if (!_frmwk_init) {
		fprintf(stderr, "Framework not initialized\n");
		return -1;
	}

	cxit_fi_hints = fi_allocinfo();
	ret = (cxit_fi_hints != NULL) ? FI_SUCCESS : FI_ENOMEM;

	cxit_fi_hints->fabric_attr->prov_name = strdup("cxi");
	cxit_fi_hints->domain_attr->mr_mode = FI_MR_ENDPOINT;
	cxit_fi_hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;

	ret = fi_getinfo(FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION),
			 cxit_node, cxit_service, cxit_flags, cxit_fi_hints,
			 &cxit_fi);
	RETURN_ERROR(ret, "fi_getinfo");

	ret = fi_fabric(cxit_fi->fabric_attr, &cxit_fabric, NULL);
	RETURN_ERROR(ret, "fi_fabric");

	ret = fi_domain(cxit_fabric, cxit_fi, &cxit_domain, NULL);
	RETURN_ERROR(ret, "fi_domain");

	ret = fi_open_ops(&cxit_domain->fid, FI_CXI_DOM_OPS_1, 0,
			  (void **)&cxit_dom_ops, NULL);
	RETURN_ERROR(ret, "fi_open_ops 1");

	ret = fi_open_ops(&cxit_domain->fid, FI_CXI_DOM_OPS_2, 0,
			  (void **)&cxit_dom_ops, NULL);
	RETURN_ERROR(ret, "fi_open_ops 2");

	ret = fi_open_ops(&cxit_domain->fid, FI_CXI_DOM_OPS_3, 0,
			  (void **)&cxit_dom_ops, NULL);
	RETURN_ERROR(ret, "fi_open_ops 3");

	ret = fi_set_ops(&cxit_domain->fid, FI_SET_OPS_HMEM_OVERRIDE, 0,
			 &cxit_hmem_ops, NULL);
	RETURN_ERROR(ret, "fi_set_ops");

	ret = fi_endpoint(cxit_domain, cxit_fi, &cxit_ep, NULL);
	RETURN_ERROR(ret, "fi_endpoint");

	ret = fi_cq_open(cxit_domain, &cxit_rx_cq_attr, &cxit_rx_cq, NULL);
	RETURN_ERROR(ret, "fi_cq_open RX");

	ret = fi_ep_bind(cxit_ep, &cxit_rx_cq->fid, cxit_rx_cq_bind_flags);
	RETURN_ERROR(ret, "fi_ep_bind RX_CQ");

	ret = fi_cq_open(cxit_domain, &cxit_tx_cq_attr, &cxit_tx_cq, NULL);
	RETURN_ERROR(ret, "fi_cq_open TX");
	ret = fi_ep_bind(cxit_ep, &cxit_tx_cq->fid, cxit_tx_cq_bind_flags);
	RETURN_ERROR(ret, "fi_ep_bind TX_CQ");

	ret = fi_eq_open(cxit_fabric, &cxit_eq_attr, &cxit_eq, NULL);
	RETURN_ERROR(ret, "fi_eq_open");
	ret = fi_ep_bind(cxit_ep, &cxit_eq->fid, cxit_eq_bind_flags);
	RETURN_ERROR(ret, "fi_ep_bind EQ");

	ret = fi_cntr_open(cxit_domain, NULL, &cxit_send_cntr, NULL);
	RETURN_ERROR(ret, "fi_cntr_open SEND");
	ret = fi_ep_bind(cxit_ep, &cxit_send_cntr->fid, FI_SEND);
	RETURN_ERROR(ret, "fi_ep_bind SEND CNTR");

	ret = fi_cntr_open(cxit_domain, NULL, &cxit_recv_cntr, NULL);
	RETURN_ERROR(ret, "fi_cntr_open RECV");
	ret = fi_ep_bind(cxit_ep, &cxit_recv_cntr->fid, FI_RECV);
	RETURN_ERROR(ret, "fi_ep_bind RECV CNTR");

	ret = fi_cntr_open(cxit_domain, NULL, &cxit_read_cntr, NULL);
	RETURN_ERROR(ret, "fi_cntr_open READ");
	ret = fi_ep_bind(cxit_ep, &cxit_read_cntr->fid, FI_READ);
	RETURN_ERROR(ret, "fi_ep_bind READ CNTR");

	ret = fi_cntr_open(cxit_domain, NULL, &cxit_write_cntr, NULL);
	RETURN_ERROR(ret, "fi_cntr_open WRITE");
	ret = fi_ep_bind(cxit_ep, &cxit_write_cntr->fid, FI_WRITE);
	RETURN_ERROR(ret, "fi_ep_bind WRITE CNTR");

	ret = fi_cntr_open(cxit_domain, NULL, &cxit_rem_cntr, NULL);
	RETURN_ERROR(ret, "fi_cntr_open REMOTE");

	cxit_av_attr.count = 1024;
	ret = fi_av_open(cxit_domain, &cxit_av_attr, &cxit_av, NULL);
	RETURN_ERROR(ret, "fi_av_open");

	ret = fi_ep_bind(cxit_ep, &cxit_av->fid, 0);
	RETURN_ERROR(ret, "fi_ep_bind AV");

	ret = fi_enable(cxit_ep);
	RETURN_ERROR(ret, "fi_enable");

	return 0;
}

/**
 * @brief One way of populating the address vector.
 *
 * This uses frmwk_allgather() to perform the allgather of addresses across all
 * nodes in the job. To work properly, the libfabric endpoint must already be
 * enabled.
 *
 * This also serves as a barrier that ensures that all ranks have reached this
 * call, i.e. all ranks have enabled their respective endpoint. If an endpoint
 * is not enabled when another endpoint sends a packet, the sender will receive
 * an ACK, but the target will drop the packet.
 *
 * This routine can be replaced with anything that provides an accurate AV
 * across all nodes in the job, e.g. MPI, symmetric AVs distributed by some
 * other out-of-band means to all nodes, or logical (rank) addressing of the
 * Cassini chips.
 *
 * @param fiaddr : returns array of fi_addr_t in rank order
 * @param size   : returns size of fiaddr array
 * @return int error code, 0 on success.
 */
int frmwk_populate_av(fi_addr_t **fiaddrp, size_t *sizep)
{
	struct cxip_addr *alladdrs = NULL;
	fi_addr_t *fiaddrs = NULL;
	int i, ret;

	if (!fiaddrp || !sizep)
		return -FI_EINVAL;

	ret = -FI_EFAULT;
	ret = frmwk_gather_nics();
	if (ret < 0)
		goto fail;

	ret = -FI_ENOMEM;
	alladdrs = calloc(frmwk_numnics, sizeof(*alladdrs));
	fiaddrs = calloc(frmwk_numnics, sizeof(*fiaddrs));
	if (!fiaddrs || !alladdrs)
		goto fail;

	for (i = 0; i < frmwk_numnics; i++)
		alladdrs[i].nic = frmwk_nics[i].nic;
	ret = fi_av_insert(cxit_av, alladdrs, frmwk_numnics,
			   fiaddrs, 0, NULL);
	if (ret != frmwk_numnics)
		goto fail;

	*sizep = frmwk_numnics;
	*fiaddrp = fiaddrs;
	return FI_SUCCESS;

fail:
	free(fiaddrs);
	free(alladdrs);
	return ret;
}

/**
 * @brief Display an error message to stderr and return error code.
 *
 * This prints to stderr only if ret != 0. It includes rank of the failing
 * process and the size of the job. These values are meaningful only after
 * frmwk_populate_av() has successfully completed.
 *
 * @param ret : error code
 * @param fmt : fprintf format
 * @param ... : fprintf parameters
 * @return int value of ret
 */
int frmwk_errmsg(int ret, const char *fmt, ...)
{
	va_list args;
	char host[256];
	char *str;
	int len;

	if (!ret)
		return 0;

	if (gethostname(host, sizeof(host)))
		strcpy(host, "unknown");

	va_start(args, fmt);
	len = vasprintf(&str, fmt, args);
	va_end(args);
	if (len < 0)
		str = "(no errmsg)";
	fprintf(stderr, "%s rank %2d of %2d FAILED %d: %s",
		host, frmwk_rank, frmwk_numranks, ret, str);
	if (len >= 0)
		free(str);

	return ret;
}

/* Read /sys files to get the HSN nic addresses */
static void get_local_nic(int hsn, union nicaddr *nic)
{
	char fname[256];
	char text[256];
	char *ptr;
	FILE *fid;
	int i, n;

	/* default */
	strcpy(text, "FF:FF:FF:FF:FF:FF\n");
	/* read from file, if possible */
	snprintf(fname, sizeof(fname), "/sys/class/net/hsn%d/address", hsn);
	if ((fid = fopen(fname, "r"))) {
		n = fread(text, 1, sizeof(text), fid);
		fclose(fid);
		text[n] = 0;
	}
	TRACE("HSN address: %s", text);

	/* parse "XX:XX:XX:XX:XX:XX\n" into 48-bit integer value */
	nic->value = 0L;
	ptr = text;
	for (i = 0; i < 6; i++) {
		nic->value <<= 8;
		nic->value |= strtol(ptr, &ptr, 16);
		ptr++;
	}
	nic->hsn = hsn;
	nic->rank = frmwk_rank;
	TRACE("rank=%2d hsn=%d nic=%05x\n", nic->rank, nic->hsn, nic->nic);
}

/* Sort comparator */
static int _compare(const void *v1, const void *v2)
{
	uint64_t *a1 = (uint64_t *)v1;
	uint64_t *a2 = (uint64_t *)v2;

	if (*a1 < *a2)
		return -1;
	if (*a1 > *a2)
		return 1;
	return 0;
}

/* Allgather on NIC addresses across collective */
int frmwk_gather_nics(void)
{
	union nicaddr *mynics = NULL;
	int i, ret, localsize;

	if (frmwk_nics)
		return 0;

	localsize = frmwk_nics_per_rank * NICSIZE;
	mynics = calloc(1, localsize);
	frmwk_nics = calloc(frmwk_numranks, localsize);
	if (!mynics || !frmwk_nics)
		goto fail;

	for (i = 0; i < frmwk_nics_per_rank; i++)
		get_local_nic(i, &mynics[i]);

	ret = frmwk_allgather(localsize, mynics, frmwk_nics);
	if (ret)
		goto fail;

	frmwk_numnics = frmwk_numranks * frmwk_nics_per_rank;
	qsort(frmwk_nics, frmwk_numnics, NICSIZE, _compare);
	TRACE("---\n");
	for (i = 0; i < frmwk_numnics; i++)
		TRACE("rank=%2d hsn=%d nic=%05x\n",
		      frmwk_nics[i].rank,
		      frmwk_nics[i].hsn,
		      frmwk_nics[i].nic);
	return 0;

fail:
	frmwk_numnics = 0;
	free(frmwk_nics);
	free(mynics);
	return -1;
}

/* User call for the address of rank, hsn */
int frmwk_nic_addr(int rank, int hsn)
{
	if (!frmwk_nics ||
	    rank < 0 || rank >= frmwk_numranks ||
	    hsn < 0 || hsn >= frmwk_nics_per_rank)
		return -1;
	return (long)frmwk_nics[rank * frmwk_nics_per_rank + hsn].nic;
}

/* Get environment variable as string representation of int */
static int getenv_int(const char *name)
{
	char *env;
	int value;

	value = -1;
	env = getenv(name);
	if (env)
		sscanf(env, "%d", &value);
	return value;
}

/* Initialize the framework */
void frmwk_init(bool quiet)
{
	char *s, *d;
	int ret = -1;

	/* Values are provided by the WLM
	 * Expected format "prefix[n1-n2,m1-m2,...],..."
	 * newton9-node-01,newton9-node-02,...
	 * newton9-node-[01-04,11-14],newton9-extra-[101-104]...
	 * We only want the first, e.g. newton9-node-01
	 */
	s = getenv("SLURM_NODELIST");
	d = (char *)frmwk_node0;
	while (s && *s && *s != ',') {
		if (*s == '[') {
			s++;
			while (*s != '-' && *s != ']')
				*d++ = *s++;
			break;
		}
		*d++ = *s++;
	}
	*d = 0;
	frmwk_nodename = getenv("SLURMD_NODENAME");
	frmwk_numranks = getenv_int("PMI_SIZE");
	frmwk_rank = getenv_int("PMI_RANK");
	frmwk_jobid = getenv("FI_CXI_COLL_JOB_ID");
	frmwk_jobstep = getenv("FI_CXI_COLL_JOB_STEP_ID");
	frmwk_mcast_token = getenv("FI_CXI_COLL_MCAST_TOKEN");
	frmwk_fabric_mgr_url = getenv("FI_CXI_COLL_FABRIC_MGR_URL");
	frmwk_hwcoll_min_nodes = getenv_int("FI_CXI_HWCOLL_MIN_NODES");
	frmwk_hwcoll_addrs_per_job = getenv_int(
					"FI_CXI_HWCOLL_ADDRS_PER_JOB");
	if (!frmwk_nodename ||
	    frmwk_numranks < 1 ||
	    frmwk_rank < 0 ||
	    !frmwk_jobid ||
	    !frmwk_jobstep ||
	    frmwk_hwcoll_min_nodes < 1 ||
	    frmwk_hwcoll_addrs_per_job < 1) {
		if (quiet)
			goto fail;
		fprintf(stderr, "frmwk_nodename=%s\n", frmwk_nodename);
		fprintf(stderr, "frmwk_numranks=%d\n", frmwk_numranks);
		fprintf(stderr, "frmwk_rank=%d\n", frmwk_rank);
		fprintf(stderr, "frmwk_jobid=%s\n", frmwk_jobid);
		fprintf(stderr, "frmwk_jobstep=%s\n", frmwk_jobstep);
		fprintf(stderr, "frmwk_mcast_token=%s\n", frmwk_mcast_token);
		fprintf(stderr, "frmwk_fabric_mgr_url=%s\n",
			frmwk_fabric_mgr_url);
		fprintf(stderr, "frmwk_hwcoll_min_nodes=%d\n",
			frmwk_hwcoll_min_nodes);
		fprintf(stderr, "frmwk_hwcoll_addrs_per_job=%d\n",
			frmwk_hwcoll_addrs_per_job);
		fprintf(stderr, "Must be run under compatible WLM\n");
		goto fail;
	}

	/* Optional for multiple HSNs, defaults to hsn0 */
	frmwk_nics_per_rank = getenv_int("PMI_NUM_HSNS");
	if (frmwk_nics_per_rank < 1)
		frmwk_nics_per_rank = 1;

	ret = 0;
fail:
	_frmwk_init = (!ret);
}

void frmwk_term(void)
{
	free(frmwk_nics);
	frmwk_nics = NULL;
	frmwk_nics_per_rank = 0;
	frmwk_numranks = 0;
	frmwk_rank = 0;
	_frmwk_init = 0;
}
