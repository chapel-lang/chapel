/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <sys/types.h>		/* shm_open and signal handling */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "psm_user.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#include "cmarw.h"
#include "psmi_wrappers.h"

#ifdef PSM_CUDA
#include "am_cuda_memhandle_cache.h"
#endif

int psmi_shm_mq_rv_thresh = PSMI_MQ_RV_THRESH_NO_KASSIST;

static const amsh_qinfo_t amsh_qcounts = {
	.qreqFifoShort = 1024,
	.qreqFifoLong = 256,
	.qrepFifoShort = 1024,
	.qrepFifoLong = 256
};

static const amsh_qinfo_t amsh_qelemsz = {
	.qreqFifoShort = sizeof(am_pkt_short_t),
	.qreqFifoLong = AMLONG_SZ,
	.qrepFifoShort = sizeof(am_pkt_short_t),
	.qrepFifoLong = AMLONG_SZ
};

ustatic struct {
	void *addr;
	size_t len;
	struct sigaction SIGSEGV_old_act;
	struct sigaction SIGBUS_old_act;
} action_stash;

static psm2_error_t amsh_poll(ptl_t *ptl, int replyonly);
static void process_packet(ptl_t *ptl, am_pkt_short_t *pkt, int isreq);
static void amsh_conn_handler(void *toki, psm2_amarg_t *args, int narg,
			      void *buf, size_t len);

/* Kassist helper functions */
#if _HFI_DEBUGGING
static const char *psmi_kassist_getmode(int mode);
#endif
static int psmi_get_kassist_mode();
int psmi_epaddr_pid(psm2_epaddr_t epaddr);

static inline void
am_ctl_qhdr_init(volatile am_ctl_qhdr_t *q, int elem_cnt, int elem_sz)
{
	pthread_spin_init(&q->lock, PTHREAD_PROCESS_SHARED);
	q->head = 0;
	q->tail = 0;
	q->elem_cnt = elem_cnt;
	q->elem_sz = elem_sz;
}

static void
am_ctl_bulkpkt_init(am_pkt_bulk_t *base_ptr, size_t elemsz, int nelems)
{
	int i;
	am_pkt_bulk_t *bulkpkt;
	uintptr_t bulkptr = (uintptr_t) base_ptr;

	for (i = 0; i < nelems; i++, bulkptr += elemsz) {
		bulkpkt = (am_pkt_bulk_t *) bulkptr;
		bulkpkt->idx = i;
	}
}

#define _PA(type) PSMI_ALIGNUP(amsh_qcounts.q ## type * amsh_qelemsz.q ## type, \
			       PSMI_PAGESIZE)
static inline uintptr_t am_ctl_sizeof_block()
{
	return PSMI_ALIGNUP(
			PSMI_ALIGNUP(AMSH_BLOCK_HEADER_SIZE, PSMI_PAGESIZE) +
			/* reqctrl block */
			PSMI_ALIGNUP(sizeof(am_ctl_blockhdr_t), PSMI_PAGESIZE) +
			_PA(reqFifoShort) + _PA(reqFifoLong) +
			/*reqctrl block */
			PSMI_ALIGNUP(sizeof(am_ctl_blockhdr_t), PSMI_PAGESIZE) +
			/* align to page size */
			_PA(repFifoShort) + _PA(repFifoLong), PSMI_PAGESIZE);
}

#undef _PA

static uint32_t create_extra_ep_data()
{
	uint32_t ret = getpid();

#ifdef PSM_CUDA
	/* PID is at maximum 22 bits */
	ret |= my_gpu_device << 22;
#endif

	return ret;
}

static void read_extra_ep_data(uint32_t data, uint32_t *pid, uint32_t *gpu)
{
	uint32_t pid_mask = (1 << 22) - 1;

	*pid = data & pid_mask;
	*gpu = (data & ~pid_mask) >> 22;
}

static void am_update_directory(struct am_ctl_nodeinfo *);

static
void amsh_atexit()
{
	static ips_atomic_t atexit_once = { 0 };
	psm2_ep_t ep;
	struct ptl_am *ptl;

	/* bail out if previous value is non-zero */
	if (ips_atomic_cmpxchg(&atexit_once, 0, 1) != 0)
		return;

	ep = psmi_opened_endpoint;
	while (ep) {
		ptl = (struct ptl_am *)(ep->ptl_amsh.ptl);
		if (ptl->self_nodeinfo &&
		    ptl->amsh_keyname != NULL) {
			_HFI_PRDBG("unlinking shm file %s\n",
				  ptl->amsh_keyname);
			shm_unlink(ptl->amsh_keyname);
		}
		ep = ep->user_ep_next;
	}

	return;
}

ustatic
void amsh_mmap_fault(int signo, siginfo_t *siginfo, void *context)
{
	if ((unsigned long int) siginfo->si_addr >= (unsigned long int) action_stash.addr &&
	    (unsigned long int) siginfo->si_addr <  (unsigned long int) action_stash.addr + (unsigned long int) action_stash.len) {

		static char shm_errmsg[256];

		snprintf(shm_errmsg, sizeof(shm_errmsg),
			 "%s: Unable to allocate shared memory for intra-node messaging.\n"
			 "%s: Delete stale shared memory files in /dev/shm.\n",
			 psmi_gethostname(), psmi_gethostname());
		amsh_atexit();
		if (psmi_write(2, shm_errmsg, strlen(shm_errmsg) + 1) == -1)
			psmi_exit(2);
		else
			psmi_exit(1); /* XXX revisit this... there's probably a better way to exit */
	} else {
		if (signo == SIGSEGV) {
			if (action_stash.SIGSEGV_old_act.sa_sigaction == (void*) SIG_DFL) {
				psmi_sigaction(SIGSEGV, &action_stash.SIGSEGV_old_act, NULL);
				raise(SIGSEGV);
				struct sigaction act;
				act.sa_sigaction = amsh_mmap_fault;
				act.sa_flags = SA_SIGINFO;
				psmi_sigaction(SIGSEGV, &act, NULL);
			} else if (action_stash.SIGSEGV_old_act.sa_sigaction == (void*) SIG_IGN) {
				return;
			} else {
				action_stash.SIGSEGV_old_act.sa_sigaction(signo, siginfo, context);
			}
		} else if (signo == SIGBUS) {
			if (action_stash.SIGBUS_old_act.sa_sigaction == (void*) SIG_DFL) {
				psmi_sigaction(SIGBUS, &action_stash.SIGBUS_old_act, NULL);
				raise(SIGBUS);
				struct sigaction act;
				act.sa_sigaction = amsh_mmap_fault;
				act.sa_flags = SA_SIGINFO;
				psmi_sigaction(SIGBUS, &act, NULL);
			} else if (action_stash.SIGBUS_old_act.sa_sigaction == (void*) SIG_IGN) {
				return;
			} else {
				action_stash.SIGBUS_old_act.sa_sigaction(signo, siginfo, context);
			}
		} else {
			psmi_exit(signo);
		}
	}
}

/**
 * Create endpoint shared-memory object, containing ep's info
 * and message queues.
 */
psm2_error_t psmi_shm_create(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_ep_t ep = ptl->ep;
	char shmbuf[256];
	void *mapptr;
	size_t segsz;
	psm2_error_t err = PSM2_OK;
	int shmfd = -1;
	char *amsh_keyname = NULL;
	int iterator;
	/* Get which kassist mode to use. */
	ptl->psmi_kassist_mode = psmi_get_kassist_mode();

	if (_HFI_PRDBG_ON) {
		_HFI_PRDBG_ALWAYS
			("kassist_mode %d %s use_kassist %d\n",
			ptl->psmi_kassist_mode,
			psmi_kassist_getmode(ptl->psmi_kassist_mode),
			(ptl->psmi_kassist_mode != PSMI_KASSIST_OFF));
	}

	segsz = am_ctl_sizeof_block();
	for (iterator = 0; iterator <= INT_MAX; iterator++) {
		snprintf(shmbuf,
			 sizeof(shmbuf),
			 "/psm3_shm.%ld%016lx%d",
			 (long int) getuid(),
			 ep->epid,
			 iterator);
		amsh_keyname = psmi_strdup(NULL, shmbuf);
		if (amsh_keyname == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		shmfd =
		    shm_open(amsh_keyname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (shmfd < 0) {
			psmi_free(amsh_keyname);
			amsh_keyname = NULL;
			if (errno == EACCES && iterator < INT_MAX)
				continue;
			else {
				err = psmi_handle_error(NULL,
							PSM2_SHMEM_SEGMENT_ERR,
							"Error creating shared "
							"memory object %s in "
							"shm_open: %s",
							amsh_keyname, strerror(errno));
				goto fail;
			}
		} else {
			struct stat st;
			if (fstat(shmfd, &st) == -1) {
				err = psmi_handle_error(NULL,
							PSM2_SHMEM_SEGMENT_ERR,
							"Error validating "
							"shared memory object %s "
							"with fstat: %s",
							amsh_keyname, strerror(errno));
				goto fail;
			}
			if (getuid() == st.st_uid) {
				err = PSM2_OK;
				break;
			} else {
				err = PSM2_SHMEM_SEGMENT_ERR;
				close(shmfd);
			}
		}
	}
	if (err) {
		if (amsh_keyname) psmi_free(amsh_keyname);
		err = psmi_handle_error(NULL,
					PSM2_SHMEM_SEGMENT_ERR,
					"Error creating shared memory object "
					"in shm_open: namespace exhausted.");
		goto fail;
	}

	/* Now register the atexit handler for cleanup, whether master or slave */
	atexit(amsh_atexit);

	_HFI_PRDBG("Opened shmfile %s\n", amsh_keyname);

	if (ftruncate(shmfd, segsz) != 0) {
		err = psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
					"Error setting size of shared memory object to %u bytes in "
					"ftruncate: %s\n",
					(uint32_t) segsz,
					strerror(errno));
		goto fail;
	}

	mapptr = mmap(NULL, segsz,
		      PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if (mapptr == MAP_FAILED) {
		err = psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
					"Error mmapping shared memory: %s",
					strerror(errno));
		psmi_free(amsh_keyname);
		goto fail;
	}

	memset((void *) mapptr, 0, segsz); /* touch all of my pages */

	/* Our own ep's info for ptl_am resides at the start of the
	   shm object.  Other processes need some of this info to
	   understand the rest of the queue structure and other details. */
	ptl->self_nodeinfo = (struct am_ctl_nodeinfo *) mapptr;
	ptl->amsh_keyname = amsh_keyname;
	ptl->self_nodeinfo->amsh_shmbase = (uintptr_t) mapptr;

fail:
	if (shmfd >= 0) close(shmfd);
	return err;
}

psm2_error_t psmi_epdir_extend(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	struct am_ctl_nodeinfo *new = NULL;

	new = (struct am_ctl_nodeinfo *)
		psmi_memalign(ptl->ep, PER_PEER_ENDPOINT, 64,
			      (ptl->am_ep_size + AMSH_DIRBLOCK_SIZE) *
			      sizeof(struct am_ctl_nodeinfo));
	if (new == NULL)
		return PSM2_NO_MEMORY;

	memcpy(new, ptl->am_ep,
	       ptl->am_ep_size * sizeof(struct am_ctl_nodeinfo));
	memset(new + ptl->am_ep_size, 0,
	       AMSH_DIRBLOCK_SIZE * sizeof(struct am_ctl_nodeinfo));

	psmi_free(ptl->am_ep);
	ptl->am_ep = new;
	ptl->am_ep_size += AMSH_DIRBLOCK_SIZE;

	return PSM2_OK;
}

/**
 * Unmap shm regions upon proper disconnect with other processes
 */
psm2_error_t psmi_do_unmap(uintptr_t shmbase)
{
	psm2_error_t err = PSM2_OK;
	if (munmap((void *)shmbase, am_ctl_sizeof_block())) {
		err =
		    psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
				      "Error with munmap of shared segment: %s",
				      strerror(errno));
	}
	return err;
}

/**
 * Map a remote process' shared memory object.
 *
 * If the remote process has a shared memory object available, add it to our own
 * directory and return the shmidx.  If the shared memory object does not exist,
 * return -1, and the connect poll function will try to map again later.
 *
 * If force_remap is true, then clear the entry that matches the epid.
 */
psm2_error_t psmi_shm_map_remote(ptl_t *ptl_gen, psm2_epid_t epid, uint16_t *shmidx_o, int force_remap)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	int i;
	int use_kassist;
	uint16_t shmidx;
	char shmbuf[256];
	void *dest_mapptr;
	size_t segsz;
	psm2_error_t err = PSM2_OK;
	int dest_shmfd;
	struct am_ctl_nodeinfo *dest_nodeinfo;
	int iterator;

	shmidx = *shmidx_o = -1;

	for (i = 0; i <= ptl->max_ep_idx; i++) {
		if (ptl->am_ep[i].epid == epid) {
			if (force_remap) {
				ptl->am_ep[i].epaddr = NULL;
				ptl->am_ep[i].epid = 0;
				break;
			}
			*shmidx_o = shmidx = i;
			return err;
		}
	}


	use_kassist = (ptl->psmi_kassist_mode != PSMI_KASSIST_OFF);

	segsz = am_ctl_sizeof_block();
	for (iterator = 0; iterator <= INT_MAX; iterator++) {
		snprintf(shmbuf,
			 sizeof(shmbuf),
			 "/psm3_shm.%ld%016lx%d",
			 (long int) getuid(),
			 epid,
			 iterator);
		dest_shmfd = shm_open(shmbuf, O_RDWR, S_IRWXU);
		if (dest_shmfd < 0) {
			if (errno == EACCES && iterator < INT_MAX)
				continue;
			else {
				err = psmi_handle_error(NULL,
							PSM2_SHMEM_SEGMENT_ERR,
							"Error opening remote "
							"shared memory object %s "
							"in shm_open: %s",
							shmbuf, strerror(errno));
				goto fail;
			}
		} else {
			struct stat st;
			if (fstat(dest_shmfd, &st) == -1) {
				err = psmi_handle_error(NULL,
							PSM2_SHMEM_SEGMENT_ERR,
							"Error validating "
							"shared memory object %s "
							"with fstat: %s",
							shmbuf, strerror(errno));
				close(dest_shmfd);
				goto fail;
			}
			if (getuid() == st.st_uid) {
				err = PSM2_OK;
				break;
			} else {
				err = PSM2_SHMEM_SEGMENT_ERR;
				close(dest_shmfd);
			}
		}
	}
	if (err) {
		err = psmi_handle_error(NULL,
					PSM2_SHMEM_SEGMENT_ERR,
					"Error opening remote shared "
					"memory object in shm_open: "
					"namespace exhausted.");
		goto fail;
	}

	dest_mapptr = mmap(NULL, segsz,
		      PROT_READ | PROT_WRITE, MAP_SHARED, dest_shmfd, 0);
	if (dest_mapptr == MAP_FAILED) {
		err = psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
					"Error mmapping remote shared memory: %s",
					strerror(errno));
		close(dest_shmfd);
		goto fail;
	}
	close(dest_shmfd);
	dest_nodeinfo = (struct am_ctl_nodeinfo *)dest_mapptr;

	/* We core dump right after here if we don't check the mmap */
	action_stash.addr = dest_mapptr;
	action_stash.len = segsz;

	struct sigaction act = { .sa_sigaction = amsh_mmap_fault, .sa_flags = SA_SIGINFO };

	sigaction(SIGSEGV, &act, &action_stash.SIGSEGV_old_act);
	sigaction(SIGBUS, &act, &action_stash.SIGBUS_old_act);

	{
		volatile uint16_t *is_init = &dest_nodeinfo->is_init;
		while (*is_init == 0)
			usleep(1);
		ips_sync_reads();
		_HFI_CONNDBG("Got a published remote dirpage page at "
			   "%p, size=%d\n", dest_mapptr, (int)segsz);
	}

	shmidx = -1;
	if ((ptl->max_ep_idx + 1) == ptl->am_ep_size) {
		err = psmi_epdir_extend(ptl_gen);
		if (err)
			goto fail;

		for (i = 0; i <= ptl->max_ep_idx; i++) {
			if (ptl->am_ep[i].epid != 0)
				am_update_directory(&ptl->am_ep[i]);
		}
	}
	for (i = 0; i < ptl->am_ep_size; i++) {
		psmi_assert(ptl->am_ep[i].epid != epid);
		if (ptl->am_ep[i].epid == 0) {
			ptl->am_ep[i].epid = epid;
			ptl->am_ep[i].psm_verno = dest_nodeinfo->psm_verno;
			ptl->am_ep[i].pid = dest_nodeinfo->pid;
			if (use_kassist) {
				/* If we are able to use CMA assume everyone
				 * else on the node can also use it.
				 * Advertise that CMA is active via the
				 * feature flag.
				 */

				if (cma_available()) {
					ptl->am_ep[i].amsh_features |=
					    AMSH_HAVE_CMA;
					psmi_shm_mq_rv_thresh =
					    PSMI_MQ_RV_THRESH_CMA;
				} else {
					ptl->psmi_kassist_mode =
					    PSMI_KASSIST_OFF;
					use_kassist = 0;
					psmi_shm_mq_rv_thresh =
					    PSMI_MQ_RV_THRESH_NO_KASSIST;
				}
			} else
				psmi_shm_mq_rv_thresh =
				    PSMI_MQ_RV_THRESH_NO_KASSIST;
			_HFI_CONNDBG("KASSIST MODE: %s\n",
				   psmi_kassist_getmode(ptl->psmi_kassist_mode));
			shmidx = *shmidx_o = i;
			_HFI_CONNDBG("Mapped epid 0x%"PRIx64" into shmidx %d\n", epid, shmidx);
			ptl->am_ep[i].amsh_shmbase = (uintptr_t) dest_mapptr;
			ptl->am_ep[i].amsh_qsizes = dest_nodeinfo->amsh_qsizes;
			if (i > ptl->max_ep_idx)
				ptl->max_ep_idx = i;
			break;
		}
	}

	/* install the old sighandler back */
	sigaction(SIGSEGV, &action_stash.SIGSEGV_old_act, NULL);
	sigaction(SIGBUS, &action_stash.SIGBUS_old_act, NULL);

	if (shmidx == (uint16_t)-1)
		err = psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
					"Could not connect to local endpoint");
fail:
	return err;
}

/**
 * Initialize pointer structure and locks for endpoint shared-memory AM.
 */

#define AMSH_QSIZE(type)                                                \
	PSMI_ALIGNUP(amsh_qelemsz.q ## type * amsh_qcounts.q ## type,   \
		     PSMI_PAGESIZE)

static psm2_error_t amsh_init_segment(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;

	/* Preconditions */
	psmi_assert_always(ptl != NULL);
	psmi_assert_always(ptl->ep != NULL);
	psmi_assert_always(ptl->epaddr != NULL);
	psmi_assert_always(ptl->ep->epid != 0);

	if ((err = psmi_shm_create(ptl_gen)))
		goto fail;

	ptl->self_nodeinfo->amsh_qsizes.qreqFifoShort = AMSH_QSIZE(reqFifoShort);
	ptl->self_nodeinfo->amsh_qsizes.qreqFifoLong = AMSH_QSIZE(reqFifoLong);
	ptl->self_nodeinfo->amsh_qsizes.qrepFifoShort = AMSH_QSIZE(repFifoShort);
	ptl->self_nodeinfo->amsh_qsizes.qrepFifoLong = AMSH_QSIZE(repFifoLong);

	/* We core dump right after here if we don't check the mmap */

	struct sigaction act = {
		.sa_sigaction = amsh_mmap_fault,
		.sa_flags = SA_SIGINFO
	};

	sigaction(SIGSEGV, &act, &action_stash.SIGSEGV_old_act);
	sigaction(SIGBUS, &act, &action_stash.SIGBUS_old_act);

	/*
	 * Now that we know our epid, update it in the shmidx array
	 */
	ptl->reqH.base = ptl->reqH.head = ptl->reqH.end = NULL;
	ptl->repH.base = ptl->repH.head = ptl->repH.end = NULL;

	am_update_directory(ptl->self_nodeinfo);

	ptl->reqH.head = ptl->reqH.base = (am_pkt_short_t *)
		(((uintptr_t)ptl->self_nodeinfo->qdir.qreqFifoShort));
	ptl->reqH.end = (am_pkt_short_t *)
		(((uintptr_t)ptl->self_nodeinfo->qdir.qreqFifoShort) +
		 amsh_qcounts.qreqFifoShort * amsh_qelemsz.qreqFifoShort);

	ptl->repH.head = ptl->repH.base = (am_pkt_short_t *)
		(((uintptr_t)ptl->self_nodeinfo->qdir.qrepFifoShort));
	ptl->repH.end = (am_pkt_short_t *)
		(((uintptr_t)ptl->self_nodeinfo->qdir.qrepFifoShort) +
		 amsh_qcounts.qrepFifoShort * amsh_qelemsz.qrepFifoShort);

	am_ctl_qhdr_init(&ptl->self_nodeinfo->qdir.qreqH->shortq,
			 amsh_qcounts.qreqFifoShort,
			 amsh_qelemsz.qreqFifoShort);
	am_ctl_qhdr_init(&ptl->self_nodeinfo->qdir.qreqH->longbulkq,
			 amsh_qcounts.qreqFifoLong, amsh_qelemsz.qreqFifoLong);
	am_ctl_qhdr_init(&ptl->self_nodeinfo->qdir.qrepH->shortq,
			 amsh_qcounts.qrepFifoShort,
			 amsh_qelemsz.qrepFifoShort);
	am_ctl_qhdr_init(&ptl->self_nodeinfo->qdir.qrepH->longbulkq,
			 amsh_qcounts.qrepFifoLong, amsh_qelemsz.qrepFifoLong);

	/* Set bulkidx in every bulk packet */
	am_ctl_bulkpkt_init(ptl->self_nodeinfo->qdir.qreqFifoLong,
			    amsh_qelemsz.qreqFifoLong,
			    amsh_qcounts.qreqFifoLong);
	am_ctl_bulkpkt_init(ptl->self_nodeinfo->qdir.qrepFifoLong,
			    amsh_qelemsz.qrepFifoLong,
			    amsh_qcounts.qrepFifoLong);

	/* install the old sighandler back */
	sigaction(SIGSEGV, &action_stash.SIGSEGV_old_act, NULL);
	sigaction(SIGBUS, &action_stash.SIGBUS_old_act, NULL);

fail:
	return err;
}

psm2_error_t psmi_shm_detach(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	uintptr_t shmbase;

	if (ptl->self_nodeinfo == NULL)
		return err;

	_HFI_PRDBG("unlinking shm file %s\n", ptl->amsh_keyname + 1);
	shmbase = ptl->self_nodeinfo->amsh_shmbase;
	shm_unlink(ptl->amsh_keyname);
	psmi_free(ptl->amsh_keyname);

	if (munmap((void *)shmbase, am_ctl_sizeof_block())) {
		err =
		    psmi_handle_error(NULL, PSM2_SHMEM_SEGMENT_ERR,
				      "Error with munmap of shared segment: %s",
				      strerror(errno));
		goto fail;
	}
	ptl->self_nodeinfo = NULL;
	return PSM2_OK;

fail:
	return err;
}

/**
 * Update locally shared-pointer directory.  The directory must be
 * updated when a new epaddr is connected to or on every epaddr already
 * connected to whenever the shared memory segment is relocated via mremap.
 *
 * @param epaddr Endpoint address for which to update local directory.
 */

static
void am_update_directory(struct am_ctl_nodeinfo *nodeinfo)
{
	uintptr_t base_this;

	base_this = nodeinfo->amsh_shmbase +
		AMSH_BLOCK_HEADER_SIZE;

	/* Request queues */
	nodeinfo->qdir.qreqH = (am_ctl_blockhdr_t *) base_this;
	nodeinfo->qdir.qreqFifoShort = (am_pkt_short_t *)
	    ((uintptr_t) nodeinfo->qdir.qreqH +
	     PSMI_ALIGNUP(sizeof(am_ctl_blockhdr_t), PSMI_PAGESIZE));

	nodeinfo->qdir.qreqFifoLong = (am_pkt_bulk_t *)
	    ((uintptr_t) nodeinfo->qdir.qreqFifoShort +
	     nodeinfo->amsh_qsizes.qreqFifoShort);

	/* Reply queues */
	nodeinfo->qdir.qrepH = (am_ctl_blockhdr_t *)
	    ((uintptr_t) nodeinfo->qdir.qreqFifoLong +
	     nodeinfo->amsh_qsizes.qreqFifoLong);

	nodeinfo->qdir.qrepFifoShort = (am_pkt_short_t *)
	    ((uintptr_t) nodeinfo->qdir.qrepH +
	     PSMI_ALIGNUP(sizeof(am_ctl_blockhdr_t), PSMI_PAGESIZE));
	nodeinfo->qdir.qrepFifoLong = (am_pkt_bulk_t *)
	    ((uintptr_t) nodeinfo->qdir.qrepFifoShort +
	     nodeinfo->amsh_qsizes.qrepFifoShort);

	_HFI_VDBG("epaddr=%p Request Hdr=%p,Pkt=%p,Long=%p\n",
		  nodeinfo->epaddr,
		  nodeinfo->qdir.qreqH,
		  nodeinfo->qdir.qreqFifoShort,
		  nodeinfo->qdir.qreqFifoLong);
	_HFI_VDBG("epaddr=%p Reply   Hdr=%p,Pkt=%p,Long=%p\n",
		  nodeinfo->epaddr,
		  nodeinfo->qdir.qrepH,
		  nodeinfo->qdir.qrepFifoShort,
		  nodeinfo->qdir.qrepFifoLong);

	/* Sanity check */
	uintptr_t base_next =
	    (uintptr_t) nodeinfo->qdir.qrepFifoLong +
	    nodeinfo->amsh_qsizes.qrepFifoLong;

	psmi_assert_always(base_next - base_this <= am_ctl_sizeof_block());
}


/* ep_epid_share_memory wrapper */
static
int amsh_epid_reachable(ptl_t *ptl_gen, psm2_epid_t epid)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	int result;
	psm2_error_t err;
	err = psm2_ep_epid_share_memory(ptl->ep, epid, &result);
	psmi_assert_always(err == PSM2_OK);
	return result;
}

static
psm2_error_t
amsh_epaddr_add(ptl_t *ptl_gen, psm2_epid_t epid, uint16_t shmidx, psm2_epaddr_t *epaddr_o)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_epaddr_t epaddr;
	am_epaddr_t *amaddr;
	psm2_error_t err = PSM2_OK;

	psmi_assert(psmi_epid_lookup(ptl->ep, epid) == NULL);

	/* The self PTL handles loopback communication. */
	psmi_assert(epid != ptl->epid);

	/* note the size of the memory is am_epaddr_t */
	epaddr = (psm2_epaddr_t) psmi_calloc(ptl->ep,
					    PER_PEER_ENDPOINT, 1,
					    sizeof(am_epaddr_t));
	if (epaddr == NULL) {
		return PSM2_NO_MEMORY;
	}
	psmi_assert_always(ptl->am_ep[shmidx].epaddr == NULL);

	if ((err = psmi_epid_set_hostname(psm2_epid_nid(epid),
					  psmi_gethostname(), 0)))
		goto fail;

	epaddr->ptlctl = ptl->ctl;
	epaddr->epid = epid;

	/* convert to am_epaddr_t */
	amaddr = (am_epaddr_t *) epaddr;
	/* tell the other endpoint their location in our directory */
	amaddr->shmidx = shmidx;
	/* we haven't connected yet, so we can't give them the same hint */
	amaddr->return_shmidx = -1;
	amaddr->cstate_outgoing = AMSH_CSTATE_OUTGOING_NONE;
	amaddr->cstate_incoming = AMSH_CSTATE_INCOMING_NONE;

	/* other setup */
	ptl->am_ep[shmidx].epaddr = epaddr;
	am_update_directory(&ptl->am_ep[shmidx]);
	/* Finally, add to table */
	if ((err = psmi_epid_add(ptl->ep, epid, epaddr)))
		goto fail;
	_HFI_CONNDBG("epaddr=%p %s added to ptl=%p\n",
		  epaddr, psmi_epaddr_get_name(epid), ptl);
	*epaddr_o = epaddr;
	return PSM2_OK;
fail:
	if (epaddr != ptl->epaddr)
		psmi_free(epaddr);
	return err;
}

static
void
amsh_epaddr_update(ptl_t *ptl_gen, psm2_epaddr_t epaddr)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	am_epaddr_t *amaddr;
	uint16_t shmidx;
	struct am_ctl_nodeinfo *nodeinfo;

	amaddr = (am_epaddr_t *) epaddr;
	shmidx = amaddr->shmidx;
	nodeinfo = (struct am_ctl_nodeinfo *) ptl->am_ep[shmidx].amsh_shmbase;

	/* restart the connection process */
	amaddr->return_shmidx = -1;
	amaddr->cstate_outgoing = AMSH_CSTATE_OUTGOING_NONE;

	/* wait for the other process to init again */
	{
		volatile uint16_t *is_init = &nodeinfo->is_init;
		while (*is_init == 0)
			usleep(1);
		ips_sync_reads();
	}

	/* get the updated values from the new nodeinfo page */
	ptl->am_ep[shmidx].psm_verno = nodeinfo->psm_verno;
	ptl->am_ep[shmidx].pid = nodeinfo->pid;
	ptl->am_ep[shmidx].amsh_qsizes = nodeinfo->amsh_qsizes;
	am_update_directory(&ptl->am_ep[shmidx]);
	return;
}

struct ptl_connection_req {
	int isdone;
	int op;			/* connect or disconnect */
	int numep;
	int numep_left;
	int phase;

	int *epid_mask;
	const psm2_epid_t *epids;	/* input epid list */
	psm2_epaddr_t *epaddr;
	psm2_error_t *errors;	/* inout errors */

	/* Used for connect/disconnect */
	psm2_amarg_t args[4];
};

static
void amsh_free_epaddr(psm2_epaddr_t epaddr)
{
	psmi_epid_remove(epaddr->ptlctl->ep, epaddr->epid);
	psmi_free(epaddr);
	return;
}

#define PTL_OP_CONNECT      0
#define PTL_OP_DISCONNECT   1
#define PTL_OP_ABORT        2

static
psm2_error_t
amsh_ep_connreq_init(ptl_t *ptl_gen, int op, /* connect, disconnect or abort */
		     int numep, const psm2_epid_t *array_of_epid, /* non-NULL on connect */
		     const int array_of_epid_mask[],
		     psm2_error_t *array_of_errors,
		     psm2_epaddr_t *array_of_epaddr,
		     struct ptl_connection_req **req_o)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	int i, cstate;
	psm2_epaddr_t epaddr;
	psm2_epid_t epid;
	struct ptl_connection_req *req = NULL;

	req = (struct ptl_connection_req *)
	    psmi_calloc(ptl->ep, PER_PEER_ENDPOINT, 1,
			sizeof(struct ptl_connection_req));
	if (req == NULL)
		return PSM2_NO_MEMORY;
	req->isdone = 0;
	req->op = op;
	req->numep = numep;
	req->numep_left = 0;
	req->phase = ptl->connect_phase;
	req->epid_mask = (int *)
	    psmi_calloc(ptl->ep, PER_PEER_ENDPOINT, numep, sizeof(int));
	if (req->epid_mask == NULL) {
		psmi_free(req);
		return PSM2_NO_MEMORY;
	}
	req->epaddr = array_of_epaddr;
	req->epids = array_of_epid;
	req->errors = array_of_errors;

	/* First check if there's really something to connect/disconnect
	 * for this PTL */
	for (i = 0; i < numep; i++) {
		req->epid_mask[i] = AMSH_CMASK_NONE;	/* no connect by default */
		if (!array_of_epid_mask[i])
			continue;
		if (op == PTL_OP_CONNECT) {
			epid = array_of_epid[i];

			/* Connect only to other processes reachable by shared memory.
			   The self PTL handles loopback communication, so explicitly
			   refuse to connect to self. */
			if (!amsh_epid_reachable(ptl_gen, epid)
			    || epid == ptl->epid) {
				array_of_errors[i] = PSM2_EPID_UNREACHABLE;
				array_of_epaddr[i] = NULL;
				continue;
			}

			_HFI_CONNDBG("Connect epid 0x%"PRIx64"\n", epid);
			epaddr = psmi_epid_lookup(ptl->ep, epid);
			if (epaddr != NULL) {
				if (epaddr->ptlctl->ptl != ptl_gen) {
					array_of_errors[i] =
					    PSM2_EPID_UNREACHABLE;
					array_of_epaddr[i] = NULL;
					continue;
				}
				cstate = ((am_epaddr_t *) epaddr)->cstate_outgoing;
				if (cstate == AMSH_CSTATE_OUTGOING_ESTABLISHED) {
					array_of_epaddr[i] = epaddr;
					array_of_errors[i] = PSM2_OK;
				} else {
					psmi_assert(cstate ==
						    AMSH_CSTATE_OUTGOING_NONE);
					array_of_errors[i] = PSM2_TIMEOUT;
					array_of_epaddr[i] = epaddr;
					req->epid_mask[i] = AMSH_CMASK_PREREQ;
				}
			} else {
				req->epid_mask[i] = AMSH_CMASK_PREREQ;
				array_of_epaddr[i] = NULL;
			}
		} else {	/* disc or abort */
			epaddr = array_of_epaddr[i];
			if (epaddr->ptlctl->ptl != ptl_gen)
				continue;

			psmi_assert(epaddr != NULL);
			_HFI_CONNDBG("Disconnect force=%d epid 0x%"PRIx64"\n",
				(op == PTL_OP_ABORT), epaddr->epid);
			cstate = ((am_epaddr_t *) epaddr)->cstate_outgoing;
			if (cstate == AMSH_CSTATE_OUTGOING_ESTABLISHED) {
				req->epid_mask[i] = AMSH_CMASK_PREREQ;
				_HFI_VDBG
				    ("Just set index %d to AMSH_CMASK_PREREQ\n",
				     i);
			}
			/* XXX undef ? */
		}
		if (req->epid_mask[i] != AMSH_CMASK_NONE)
			req->numep_left++;
	}

	if (req->numep_left == 0) {	/* nothing to do */
		psmi_free(req->epid_mask);
		psmi_free(req);
		if (op != PTL_OP_ABORT) {
			_HFI_CONNDBG("Nothing to connect, bump up phase\n");
			ptl->connect_phase++;
		}
		*req_o = NULL;
		return PSM2_OK;
	} else {
		*req_o = req;
		return PSM2_OK_NO_PROGRESS;
	}
}

static
psm2_error_t
amsh_ep_connreq_poll(ptl_t *ptl_gen, struct ptl_connection_req *req)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	int i, j, cstate;
	uint16_t shmidx = (uint16_t)-1;
	psm2_error_t err = PSM2_OK;
	psm2_epid_t epid;
	psm2_epaddr_t epaddr;

	if (req == NULL || req->isdone)
		return PSM2_OK;

	psmi_assert_always(ptl->connect_phase == req->phase);

	if (req->op == PTL_OP_DISCONNECT || req->op == PTL_OP_ABORT) {
		for (i = 0; i < req->numep; i++) {
			if (req->epid_mask[i] == AMSH_CMASK_NONE ||
			    req->epid_mask[i] == AMSH_CMASK_DONE)
				continue;

			epaddr = req->epaddr[i];
			psmi_assert(epaddr != NULL);
			if (req->epid_mask[i] == AMSH_CMASK_PREREQ) {
				shmidx = ((am_epaddr_t *) epaddr)->shmidx;
				/* Make sure the target of the disconnect is still there */
				if (ptl->am_ep[shmidx].
				    epid != epaddr->epid) {
					req->numep_left--;
					req->epid_mask[i] = AMSH_CMASK_DONE;
					((am_epaddr_t *) epaddr)->cstate_outgoing =
						AMSH_CSTATE_OUTGOING_NONE;
				}
			}

			if (req->epid_mask[i] == AMSH_CMASK_PREREQ) {
				req->args[0].u16w0 = PSMI_AM_DISC_REQ;
				req->args[0].u16w1 = shmidx;
				req->args[0].u32w1 = ptl->connect_phase;
				req->args[1].u64w0 = (uint64_t) ptl->epid;
				psmi_assert(shmidx != (uint16_t)-1);
				req->args[2].u32w0 = create_extra_ep_data();
				req->args[2].u32w1 = PSM2_OK;
				if (req->op != PTL_OP_ABORT)
					req->args[3].u64w0 =
					    (uint64_t) (uintptr_t) &req->errors[i];
				else
					req->args[3].u64w0 = 0;
				psmi_amsh_short_request(ptl_gen, epaddr,
							amsh_conn_handler_hidx,
							req->args, 4, NULL, 0,
							0);
				((am_epaddr_t *) epaddr)->cstate_outgoing =
					AMSH_CSTATE_OUTGOING_DISC_REQUESTED;
				/**
				* Only munmap if we have nothing more to
				* communicate with the other node, i.e. we
				* already recieved a disconnect req from the
				* other node.
				*/
				if (((am_epaddr_t *) epaddr)->cstate_incoming ==
					AMSH_CSTATE_INCOMING_DISC_REQUESTED)
					err = psmi_do_unmap(ptl->am_ep[shmidx].amsh_shmbase);
				req->epid_mask[i] = AMSH_CMASK_POSTREQ;
			} else if (req->epid_mask[i] == AMSH_CMASK_POSTREQ) {
				cstate = ((am_epaddr_t *) epaddr)->cstate_outgoing;
				if (cstate == AMSH_CSTATE_OUTGOING_DISC_REPLIED) {
					req->numep_left--;
					req->epid_mask[i] = AMSH_CMASK_DONE;
					((am_epaddr_t *) epaddr)->cstate_outgoing =
						AMSH_CSTATE_OUTGOING_NONE;
				}
			}
		}
	} else {
		/* First see if we've made progress on any postreqs */
		int n_prereq = 0;
		for (i = 0; i < req->numep; i++) {
			int cstate;
			if (req->epid_mask[i] != AMSH_CMASK_POSTREQ) {
				if (req->epid_mask[i] == AMSH_CMASK_PREREQ)
					n_prereq++;
				continue;
			}
			epaddr = req->epaddr[i];
			psmi_assert(epaddr != NULL);

			/* detect if a race has occurred on due to re-using an
			 * old shm file - if so, restart the connection */
			shmidx = ((am_epaddr_t *) epaddr)->shmidx;
			if (ptl->am_ep[shmidx].pid !=
			    ((struct am_ctl_nodeinfo *) ptl->am_ep[shmidx].amsh_shmbase)->pid) {
				req->epid_mask[i] = AMSH_CMASK_PREREQ;
				((am_epaddr_t *) epaddr)->cstate_outgoing =
					AMSH_CSTATE_OUTGOING_NONE;
				n_prereq++;
				amsh_epaddr_update(ptl_gen, epaddr);
				continue;
			}

			cstate = ((am_epaddr_t *) epaddr)->cstate_outgoing;
			if (cstate == AMSH_CSTATE_OUTGOING_REPLIED) {
				req->numep_left--;
				((am_epaddr_t *) epaddr)->cstate_outgoing =
					AMSH_CSTATE_OUTGOING_ESTABLISHED;
				req->epid_mask[i] = AMSH_CMASK_DONE;
				continue;
			}
		}
		if (n_prereq > 0) {
			psmi_assert(req->numep_left > 0);
			/* Go through the list of peers we need to connect to and find out
			 * if they each shared ep is mapped into shm */
			for (i = 0; i < req->numep; i++) {
				if (req->epid_mask[i] != AMSH_CMASK_PREREQ)
					continue;
				epid = req->epids[i];
				epaddr = req->epaddr[i];
				/* Go through mapped epids and find the epid we're looking for */
				for (shmidx = -1, j = 0;
				     j <= ptl->max_ep_idx; j++) {
					/* epid is connected and ready to go */
					if (ptl->am_ep[j].
					    epid == epid) {
						shmidx = j;
						break;
					}
				}
				if (shmidx == (uint16_t)-1) {
					/* Couldn't find peer's epid in dirpage.
					   Check shmdir to see if epid is up now. */
					if ((err = psmi_shm_map_remote(ptl_gen, epid, &shmidx, 0))) {
						return err;
					}
					continue;
				}
				/* Before we even send the request out, check to see if
				 * versions are interoperable */
				if (!psmi_verno_isinteroperable
				    (ptl->am_ep[shmidx].
				     psm_verno)) {
					char buf[32];
					uint16_t their_verno =
					    ptl->am_ep[shmidx].
					    psm_verno;
					snprintf(buf, sizeof(buf), "%d.%d",
						 PSMI_VERNO_GET_MAJOR
						 (their_verno),
						 PSMI_VERNO_GET_MINOR
						 (their_verno));

					_HFI_INFO("Local endpoint id %" PRIx64
						  " has version %s "
						  "which is not supported by library version %d.%d",
						  epid, buf, PSM2_VERNO_MAJOR,
						  PSM2_VERNO_MINOR);
					req->errors[i] =
					    PSM2_EPID_INVALID_VERSION;
					req->numep_left--;
					req->epid_mask[i] = AMSH_CMASK_DONE;
					continue;
				}
				if (epaddr != NULL) {
					psmi_assert(((am_epaddr_t *) epaddr)->
						    shmidx == shmidx);
				} else
				    if ((epaddr =
					 psmi_epid_lookup(ptl->ep,
							  epid)) == NULL) {
					if ((err =
					     amsh_epaddr_add(ptl_gen, epid, shmidx,
							     &epaddr))) {
						return err;
					}
					/* Remote pid is unknown at the moment */
					((am_epaddr_t *) epaddr)->pid =
						AMSH_PID_UNKNOWN;
				}
				req->epaddr[i] = epaddr;
				req->args[0].u16w0 = PSMI_AM_CONN_REQ;
				/* tell the other process its shmidx here */
				req->args[0].u16w1 = shmidx;
				req->args[0].u32w1 = ptl->connect_phase;
				req->args[1].u64w0 = (uint64_t) ptl->epid;
				req->args[2].u32w0 = create_extra_ep_data();
				req->args[2].u32w1 = PSM2_OK;
				req->args[3].u64w0 =
				    (uint64_t) (uintptr_t) &req->errors[i];
				req->epid_mask[i] = AMSH_CMASK_POSTREQ;
				psmi_amsh_short_request(ptl_gen, epaddr,
							amsh_conn_handler_hidx,
							req->args, 4, NULL, 0,
							0);
				_HFI_CONNDBG("epaddr=%p, epid=0x%" PRIx64
					   " at shmidx=%d\n", epaddr, epid,
					   shmidx);
			}
		}
	}

	if (req->numep_left == 0) {	/* we're all done */
		req->isdone = 1;
		return PSM2_OK;
	} else {
		sched_yield();
		return PSM2_OK_NO_PROGRESS;
	}
}

static
psm2_error_t
amsh_ep_connreq_fini(ptl_t *ptl_gen, struct ptl_connection_req *req)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	int i;

	/* Wherever we are at in our connect process, we've been instructed to
	 * finish the connection process */
	if (req == NULL)
		return PSM2_OK;

	/* This prevents future connect replies from referencing data structures
	 * that disappeared.  For abort we aren't waiting for DISC_REP so
	 * we want to keep same phase so we accept them after this function */
	if (req->op != PTL_OP_ABORT)
		ptl->connect_phase++;

	/* First process any leftovers in postreq or prereq */
	for (i = 0; i < req->numep; i++) {
		if (req->epid_mask[i] == AMSH_CMASK_NONE
			|| req->op == PTL_OP_ABORT)
			continue;
		else if (req->epid_mask[i] == AMSH_CMASK_POSTREQ) {
			int cstate;
			req->epid_mask[i] = AMSH_CMASK_DONE;
			cstate = ((am_epaddr_t *) req->epaddr[i])->cstate_outgoing;
			if (cstate == AMSH_CSTATE_OUTGOING_REPLIED) {
				req->numep_left--;
				((am_epaddr_t *) req->epaddr[i])->cstate_outgoing =
					AMSH_CSTATE_OUTGOING_ESTABLISHED;
			} else {	/* never actually got reply */
				req->errors[i] = PSM2_TIMEOUT;
			}
		}
		/* If we couldn't go from prereq to postreq, that means we couldn't
		 * find the shmidx for an epid in time.  This can only be a case of
		 * time out */
		else if (req->epid_mask[i] == AMSH_CMASK_PREREQ) {
			req->errors[i] = PSM2_TIMEOUT;
			req->numep_left--;
			req->epid_mask[i] = AMSH_CMASK_DONE;
		}
	}

	/* Whatever is left can only be in DONE or NONE state */
	for (i = 0; i < req->numep; i++) {
		if (req->epid_mask[i] == AMSH_CMASK_NONE)
			continue;
		if (req->op == PTL_OP_ABORT
			 && req->epid_mask[i] != AMSH_CMASK_DONE) {
			req->epid_mask[i] = AMSH_CMASK_DONE;
			continue;
		}
		psmi_assert(req->epid_mask[i] == AMSH_CMASK_DONE);

		err = psmi_error_cmp(err, req->errors[i]);
		/* XXX TODO: Report errors in connection. */
		/* Only free epaddr if they have disconnected from us */
		int cstate = ((am_epaddr_t *) req->epaddr[i])->cstate_incoming;
		if (cstate == AMSH_CSTATE_INCOMING_DISC_REQUESTED) {
			if (req->op == PTL_OP_DISCONNECT || req->op == PTL_OP_ABORT) {
				psmi_assert(req->epaddr[i] != NULL);
				amsh_free_epaddr(req->epaddr[i]);
				req->epaddr[i] = NULL;
			}
		}
	}

	psmi_free(req->epid_mask);
	psmi_free(req);

	return err;
}

/* Wrapper for 2.0's use of connect/disconnect.  The plan is to move the
 * init/poll/fini interface up to the PTL level for 2.2 */
#define CONNREQ_ZERO_POLLS_BEFORE_YIELD  20
static
psm2_error_t
amsh_ep_connreq_wrap(ptl_t *ptl_gen, int op,
		     int numep,
		     const psm2_epid_t *array_of_epid,
		     const int array_of_epid_mask[],
		     psm2_error_t *array_of_errors,
		     psm2_epaddr_t *array_of_epaddr, uint64_t timeout_ns)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err;
	uint64_t t_start;
	struct ptl_connection_req *req;
	int num_polls_noprogress = 0;
	static int shm_polite_attach = -1;

	if (shm_polite_attach == -1) {
		char *p = getenv("PSM3_SHM_POLITE_ATTACH");
		if (p && *p && atoi(p) != 0) {
			fprintf(stderr, "%s: Using Polite SHM segment attach\n",
				psmi_gethostname());
			shm_polite_attach = 1;
		}
		shm_polite_attach = 0;
	}

	/* Initialize */
	err = amsh_ep_connreq_init(ptl_gen, op, numep,
				   array_of_epid, array_of_epid_mask,
				   array_of_errors, array_of_epaddr, &req);
	if (err != PSM2_OK_NO_PROGRESS)	/* Either we're all done with connect or
					 * there was an error */
		return err;

	if (op == PTL_OP_ABORT) {
		int i;
		/* loop a couple times only, ignore timeout */
		/* this will move from PREREQ to POSTREQ and check once
		 * for reply, but not wait for reply
		 */
		for (i=0; i < 2; i++) {
			psmi_poll_internal(ptl->ep, 1);
			err = amsh_ep_connreq_poll(ptl_gen, req);
			if (err != PSM2_OK && err != PSM2_OK_NO_PROGRESS) {
				psmi_free(req->epid_mask);
				psmi_free(req);
				goto fail;
			}
		}
		goto fini;
	}

	/* Poll until either
	 * 1. We time out
	 * 2. We are done with connecting
	 */
	t_start = get_cycles();
	do {
		psmi_poll_internal(ptl->ep, 1);
		err = amsh_ep_connreq_poll(ptl_gen, req);
		if (err == PSM2_OK)
			break;	/* Finished before timeout */
		else if (err != PSM2_OK_NO_PROGRESS) {
			psmi_free(req->epid_mask);
			psmi_free(req);
			goto fail;
		} else if (shm_polite_attach &&
			   ++num_polls_noprogress ==
			   CONNREQ_ZERO_POLLS_BEFORE_YIELD) {
			num_polls_noprogress = 0;
			PSMI_YIELD(ptl->ep->mq->progress_lock);
		}
	}
	while (psmi_cycles_left(t_start, timeout_ns));

fini:
	err = amsh_ep_connreq_fini(ptl_gen, req);

fail:
	return err;
}

static
psm2_error_t
amsh_ep_connect(ptl_t *ptl,
		int numep,
		const psm2_epid_t *array_of_epid,
		const int array_of_epid_mask[],
		psm2_error_t *array_of_errors,
		psm2_epaddr_t *array_of_epaddr, uint64_t timeout_ns)
{
	return amsh_ep_connreq_wrap(ptl, PTL_OP_CONNECT, numep, array_of_epid,
				    array_of_epid_mask, array_of_errors,
				    array_of_epaddr, timeout_ns);
}

static
psm2_error_t
amsh_ep_disconnect(ptl_t *ptl, int force, int numep,
		   psm2_epaddr_t array_of_epaddr[],
		   const int array_of_epaddr_mask[],
		   psm2_error_t array_of_errors[], uint64_t timeout_ns)
{
	return amsh_ep_connreq_wrap(ptl,
				    force ? PTL_OP_ABORT : PTL_OP_DISCONNECT,
				    numep, NULL, array_of_epaddr_mask,
				    array_of_errors,
				    array_of_epaddr,
				    timeout_ns);
}

#undef CSWAP
PSMI_ALWAYS_INLINE(
int32_t
cswap(volatile int32_t *p, int32_t old_value, int32_t new_value))
{
	asm volatile ("lock cmpxchg %2, %0" :
		      "+m" (*p), "+a"(old_value) : "r"(new_value) : "memory");
	return old_value;
}

PSMI_ALWAYS_INLINE(
am_pkt_short_t *
am_ctl_getslot_pkt_inner(volatile am_ctl_qhdr_t *shq, am_pkt_short_t *pkt0))
{
	am_pkt_short_t *pkt;
	uint32_t idx;
#ifndef CSWAP
	pthread_spin_lock(&shq->lock);
	idx = shq->tail;
	pkt = (am_pkt_short_t *) ((uintptr_t) pkt0 + idx * shq->elem_sz);
	if (pkt->flag == QFREE) {
		ips_sync_reads();
		pkt->flag = QUSED;
		shq->tail += 1;
		if (shq->tail == shq->elem_cnt)
			shq->tail = 0;
	} else {
		pkt = 0;
	}
	pthread_spin_unlock(&shq->lock);
#else
	uint32_t idx_next;
	do {
		idx = shq->tail;
		idx_next = (idx + 1 == shq->elem_cnt) ? 0 : idx + 1;
	} while (cswap(&shq->tail, idx, idx_next) != idx);

	pkt = (am_pkt_short_t *) ((uintptr_t) pkt0 + idx * shq->elem_sz);
	while (cswap(&pkt->flag, QFREE, QUSED) != QFREE);
#endif
	return pkt;
}

/* This is safe because 'flag' is at the same offset on both pkt and bulkpkt */
#define am_ctl_getslot_bulkpkt_inner(shq, pkt0) ((am_pkt_bulk_t *) \
	am_ctl_getslot_pkt_inner(shq, (am_pkt_short_t *)(pkt0)))

PSMI_ALWAYS_INLINE(
am_pkt_short_t *
am_ctl_getslot_pkt(ptl_t *ptl_gen, uint16_t shmidx, int is_reply))
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	volatile am_ctl_qhdr_t *shq;
	am_pkt_short_t *pkt0;
	if (!is_reply) {
		shq = &(ptl->am_ep[shmidx].qdir.qreqH->shortq);
		pkt0 = ptl->am_ep[shmidx].qdir.qreqFifoShort;
	} else {
		shq = &(ptl->am_ep[shmidx].qdir.qrepH->shortq);
		pkt0 = ptl->am_ep[shmidx].qdir.qrepFifoShort;
	}
	return am_ctl_getslot_pkt_inner(shq, pkt0);
}

PSMI_ALWAYS_INLINE(
am_pkt_bulk_t *
am_ctl_getslot_long(ptl_t *ptl_gen, uint16_t shmidx, int is_reply))
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	volatile am_ctl_qhdr_t *shq;
	am_pkt_bulk_t *pkt0;
	if (!is_reply) {
		shq = &(ptl->am_ep[shmidx].qdir.qreqH->longbulkq);
		pkt0 = ptl->am_ep[shmidx].qdir.qreqFifoLong;
	} else {
		shq = &(ptl->am_ep[shmidx].qdir.qrepH->longbulkq);
		pkt0 = ptl->am_ep[shmidx].qdir.qrepFifoLong;
	}
	return am_ctl_getslot_bulkpkt_inner(shq, pkt0);
}

psmi_handlertab_t psmi_allhandlers[] = {
	{0}
	,
	{amsh_conn_handler}
	,
	{psmi_am_mq_handler}
	,
	{psmi_am_mq_handler_data}
	,
	{psmi_am_mq_handler_rtsmatch}
	,
	{psmi_am_mq_handler_rtsdone}
	,
	{psmi_am_handler}
};

PSMI_ALWAYS_INLINE(void advance_head(volatile am_ctl_qshort_cache_t *hdr))
{
	QMARKFREE(hdr->head);
	hdr->head++;
	if (hdr->head == hdr->end)
		hdr->head = hdr->base;
}

#define AMSH_ZERO_POLLS_BEFORE_YIELD    64
#define AMSH_POLLS_BEFORE_PSM_POLL      16

/* XXX this can be made faster.  Instead of checking the flag of the head, keep
 * a cached copy of the integer value of the tail and compare it against the
 * previous one we saw.
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
amsh_poll_internal_inner(ptl_t *ptl_gen, int replyonly,
			 int is_internal))
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK_NO_PROGRESS;
	/* poll replies */
	if (!QISEMPTY(ptl->repH.head->flag)) {
		do {
			ips_sync_reads();
			process_packet(ptl_gen, (am_pkt_short_t *) ptl->repH.head,
				       0);
			advance_head(&ptl->repH);
			err = PSM2_OK;
		} while (!QISEMPTY(ptl->repH.head->flag));
	}

	if (!replyonly) {
		/* Request queue not enable for 2.0, will be re-enabled to support long
		 * replies */
		if (!is_internal && ptl->psmi_am_reqq_fifo.first != NULL) {
			psmi_am_reqq_drain(ptl_gen);
			err = PSM2_OK;
		}
		if (!QISEMPTY(ptl->reqH.head->flag)) {
			do {
				ips_sync_reads();
				process_packet(ptl_gen,
					       (am_pkt_short_t *) ptl->reqH.
					       head, 1);
				advance_head(&ptl->reqH);
				err = PSM2_OK;
			} while (!QISEMPTY(ptl->reqH.head->flag));
		}
	}

	if (is_internal) {
		if (err == PSM2_OK)	/* some progress, no yields */
			ptl->zero_polls = 0;
		else if (++ptl->zero_polls == AMSH_ZERO_POLLS_BEFORE_YIELD) {
			/* no progress for AMSH_ZERO_POLLS_BEFORE_YIELD */
			sched_yield();
			ptl->zero_polls = 0;
		}

		if (++ptl->amsh_only_polls == AMSH_POLLS_BEFORE_PSM_POLL) {
			psmi_poll_internal(ptl->ep, 0);
			ptl->amsh_only_polls = 0;
		}
	}
	return err;		/* if we actually did something */
}

/* non-inlined version */
static
psm2_error_t
amsh_poll_internal(ptl_t *ptl, int replyonly)
{
	return amsh_poll_internal_inner(ptl, replyonly, 1);
}

#ifdef PSM_PROFILE
#define AMSH_POLL_UNTIL(ptl, isreply, cond) \
	do {								\
		PSMI_PROFILE_BLOCK();					\
		while (!(cond)) {					\
			PSMI_PROFILE_REBLOCK(				\
				amsh_poll_internal(ptl, isreply) ==	\
					PSM2_OK_NO_PROGRESS);		\
		}							\
		PSMI_PROFILE_UNBLOCK();					\
	} while (0)
#else
#define AMSH_POLL_UNTIL(ptl, isreply, cond)			\
	do {							\
		while (!(cond)) {				\
			amsh_poll_internal(ptl, isreply);	\
		}						\
	} while (0)
#endif

static psm2_error_t amsh_poll(ptl_t *ptl, int replyonly)
{
	return amsh_poll_internal_inner(ptl, replyonly, 0);
}

PSMI_ALWAYS_INLINE(
void
am_send_pkt_short(ptl_t *ptl, uint32_t destidx, uint32_t returnidx,
		  uint32_t bulkidx, uint16_t fmt, uint16_t nargs,
		  uint16_t handleridx, psm2_amarg_t *args,
		  const void *src, uint32_t len, int isreply))
{
	int i;
	volatile am_pkt_short_t *pkt;
	int copy_nargs;

	AMSH_POLL_UNTIL(ptl, isreply,
			(pkt =
			 am_ctl_getslot_pkt(ptl, destidx, isreply)) != NULL);

	/* got a free pkt... fill it in */
	pkt->bulkidx = bulkidx;
	pkt->shmidx = returnidx;
	pkt->type = fmt;
	pkt->nargs = nargs;
	pkt->handleridx = handleridx;

	/* Limit the number of args copied here to NSHORT_ARGS.  Additional args
	   are carried in the bulkpkt. */
	copy_nargs = nargs;
	if (copy_nargs > NSHORT_ARGS) {
		copy_nargs = NSHORT_ARGS;
	}

	for (i = 0; i < copy_nargs; i++)
		pkt->args[i] = args[i];

	if (fmt == AMFMT_SHORT_INLINE)
		mq_copy_tiny((uint32_t *) &pkt->args[nargs], (uint32_t *) src,
			     len);

	_HFI_VDBG("pkt=%p fmt=%d bulkidx=%d,flag=%d,nargs=%d,"
		  "buf=%p,len=%d,hidx=%d,value=%d\n", pkt, (int)fmt, bulkidx,
		  pkt->flag, pkt->nargs, src, (int)len, (int)handleridx,
		  src != NULL ? *((uint32_t *) src) : 0);
	QMARKREADY(pkt);
}

#define amsh_shm_copy_short psmi_mq_mtucpy
#define amsh_shm_copy_long  psmi_mq_mtucpy

PSMI_ALWAYS_INLINE(
int
psmi_amsh_generic_inner(uint32_t amtype, ptl_t *ptl_gen, psm2_epaddr_t epaddr,
			psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			const void *src, size_t len, void *dst, int flags))
{
#ifdef PSM_DEBUG
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
#endif
	uint16_t type;
	uint32_t bulkidx;
	uint16_t hidx = (uint16_t) handler;
	int destidx = ((am_epaddr_t *) epaddr)->shmidx;
	int returnidx = ((am_epaddr_t *) epaddr)->return_shmidx;
	int is_reply = AM_IS_REPLY(amtype);
	volatile am_pkt_bulk_t *bulkpkt;

	_HFI_VDBG("%s epaddr=%s, shmidx=%d, type=%d\n",
		  is_reply ? "reply" : "request",
		  psmi_epaddr_get_name(epaddr->epid),
		  ((am_epaddr_t *) epaddr)->shmidx, amtype);
	psmi_assert(epaddr != ptl->epaddr);

	switch (amtype) {
	case AMREQUEST_SHORT:
	case AMREPLY_SHORT:
		if (len + (nargs << 3) <= (NSHORT_ARGS << 3)) {
			/* Payload fits in args packet */
			type = AMFMT_SHORT_INLINE;
			bulkidx = len;
		} else {
			int i;

			psmi_assert(len < amsh_qelemsz.qreqFifoLong);
			psmi_assert(src != NULL || nargs > NSHORT_ARGS);
			type = AMFMT_SHORT;

			AMSH_POLL_UNTIL(ptl_gen, is_reply,
					(bulkpkt =
					 am_ctl_getslot_long(ptl_gen, destidx,
							     is_reply)) !=
					NULL);

			bulkidx = bulkpkt->idx;
			bulkpkt->len = len;
			_HFI_VDBG("bulkpkt %p flag is %d from idx %d\n",
				  bulkpkt, bulkpkt->flag, destidx);

			for (i = 0; i < nargs - NSHORT_ARGS; i++) {
				bulkpkt->args[i] = args[i + NSHORT_ARGS];
			}

			amsh_shm_copy_short((void *)bulkpkt->payload, src,
					    (uint32_t) len);
			QMARKREADY(bulkpkt);
		}
		am_send_pkt_short(ptl_gen, destidx, returnidx, bulkidx, type,
				  nargs, hidx, args, src, len, is_reply);
		break;

	case AMREQUEST_LONG:
	case AMREPLY_LONG:
		{
			uint32_t bytes_left = len;
			uint8_t *src_this = (uint8_t *) src;
			uint8_t *dst_this = (uint8_t *) dst;
			uint32_t bytes_this;

			type = AMFMT_LONG;

			_HFI_VDBG("[long][%s] src=%p,dest=%p,len=%d,hidx=%d\n",
				  is_reply ? "rep" : "req", src, dst,
				  (uint32_t) len, hidx);
			while (bytes_left) {
				bytes_this = min(bytes_left, AMLONG_MTU);
				AMSH_POLL_UNTIL(ptl_gen, is_reply,
						(bulkpkt =
						 am_ctl_getslot_long(ptl_gen,
								     destidx,
								     is_reply))
						!= NULL);
				bytes_left -= bytes_this;
				if (bytes_left == 0)
					type = AMFMT_LONG_END;
				bulkidx = bulkpkt->idx;
				amsh_shm_copy_long((void *)bulkpkt->payload,
						   src_this, bytes_this);

				bulkpkt->dest = (uintptr_t) dst;
				bulkpkt->dest_off =
				    (uint32_t) ((uintptr_t) dst_this -
						(uintptr_t) dst);
				bulkpkt->len = bytes_this;
				QMARKREADY(bulkpkt);
				am_send_pkt_short(ptl_gen, destidx, returnidx,
						  bulkidx, type, nargs, hidx,
						  args, NULL, 0, is_reply);
				src_this += bytes_this;
				dst_this += bytes_this;
			}
			break;
		}
	default:
		break;
	}
	return 1;
}

/* A generic version that's not inlined */
int
psmi_amsh_generic(uint32_t amtype, ptl_t *ptl, psm2_epaddr_t epaddr,
		  psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		  const void *src, size_t len, void *dst, int flags)
{
	return psmi_amsh_generic_inner(amtype, ptl, epaddr, handler, args,
				       nargs, src, len, dst, flags);
}

int
psmi_amsh_short_request(ptl_t *ptl, psm2_epaddr_t epaddr,
			psm2_handler_t handler, psm2_amarg_t *args, int nargs,
			const void *src, size_t len, int flags)
{
	return psmi_amsh_generic_inner(AMREQUEST_SHORT, ptl, epaddr, handler,
				       args, nargs, src, len, NULL, flags);
}

int
psmi_amsh_long_request(ptl_t *ptl, psm2_epaddr_t epaddr,
		       psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		       const void *src, size_t len, void *dest, int flags)
{
	return psmi_amsh_generic_inner(AMREQUEST_LONG, ptl, epaddr, handler,
				       args, nargs, src, len, dest, flags);
}

void
psmi_amsh_short_reply(amsh_am_token_t *tok,
		      psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		      const void *src, size_t len, int flags)
{
	psmi_amsh_generic_inner(AMREPLY_SHORT, tok->ptl, tok->tok.epaddr_incoming,
				handler, args, nargs, src, len, NULL, flags);
	return;
}

void
psmi_amsh_long_reply(amsh_am_token_t *tok,
		     psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		     const void *src, size_t len, void *dest, int flags)
{
	psmi_amsh_generic_inner(AMREPLY_LONG, tok->ptl, tok->tok.epaddr_incoming,
				handler, args, nargs, src, len, dest, flags);
	return;
}

void psmi_am_reqq_init(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	ptl->psmi_am_reqq_fifo.first = NULL;
	ptl->psmi_am_reqq_fifo.lastp = &ptl->psmi_am_reqq_fifo.first;
}

psm2_error_t psmi_am_reqq_drain(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	am_reqq_t *reqn = ptl->psmi_am_reqq_fifo.first;
	am_reqq_t *req;
	psm2_error_t err = PSM2_OK_NO_PROGRESS;

	/* We're going to process the entire list, and running the generic handler
	 * below can cause other requests to be enqueued in the queue that we're
	 * processing. */
	ptl->psmi_am_reqq_fifo.first = NULL;
	ptl->psmi_am_reqq_fifo.lastp = &ptl->psmi_am_reqq_fifo.first;

	while ((req = reqn) != NULL) {
		err = PSM2_OK;
		reqn = req->next;
		_HFI_VDBG
		    ("push of reqq=%p epaddr=%s localreq=%p remotereq=%p\n",
		     req, psmi_epaddr_get_hostname(req->epaddr->epid),
		     (void *)(uintptr_t) req->args[1].u64w0,
		     (void *)(uintptr_t) req->args[0].u64w0);
		psmi_amsh_generic(req->amtype, req->ptl, req->epaddr,
				  req->handler, req->args, req->nargs, req->src,
				  req->len, req->dest, req->amflags);
		if (req->flags & AM_FLAG_SRC_TEMP)
			psmi_free(req->src);
		psmi_free(req);
	}
	return err;
}

void
psmi_am_reqq_add(int amtype, ptl_t *ptl_gen, psm2_epaddr_t epaddr,
		 psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		 void *src, size_t len, void *dest, int amflags)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	int i;
	int flags = 0;
	am_reqq_t *nreq =
	    (am_reqq_t *) psmi_malloc(ptl->ep, UNDEFINED, sizeof(am_reqq_t));
	psmi_assert_always(nreq != NULL);
	_HFI_VDBG("alloc of reqq=%p, to epaddr=%s, ptr=%p, len=%d, "
		  "localreq=%p, remotereq=%p\n", nreq,
		  psmi_epaddr_get_hostname(epaddr->epid), dest,
		  (int)len, (void *)(uintptr_t) args[1].u64w0,
		  (void *)(uintptr_t) args[0].u64w0);

	psmi_assert(nargs <= 8);
	nreq->next = NULL;
	nreq->amtype = amtype;
	nreq->ptl = ptl_gen;
	nreq->epaddr = epaddr;
	nreq->handler = handler;
	for (i = 0; i < nargs; i++)
		nreq->args[i] = args[i];
	nreq->nargs = nargs;
	if (AM_IS_LONG(amtype) && src != NULL &&
	    len > 0 && !(amflags & AM_FLAG_SRC_ASYNC)) {
		abort();
		flags |= AM_FLAG_SRC_TEMP;
		nreq->src = psmi_malloc(ptl->ep, UNDEFINED, len);
		psmi_assert_always(nreq->src != NULL);	/* XXX mem */
		amsh_shm_copy_short(nreq->src, src, len);
	} else
		nreq->src = src;
	nreq->len = len;
	nreq->dest = dest;
	nreq->amflags = amflags;
	nreq->flags = flags;

	nreq->next = NULL;
	*(ptl->psmi_am_reqq_fifo.lastp) = nreq;
	ptl->psmi_am_reqq_fifo.lastp = &nreq->next;
}

static
void process_packet(ptl_t *ptl_gen, am_pkt_short_t *pkt, int isreq)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	amsh_am_token_t tok;
	psmi_handler_fn_t fn;
	psm2_amarg_t *args = pkt->args;
	uint16_t shmidx = pkt->shmidx;
	int nargs = pkt->nargs;

	tok.tok.epaddr_incoming = ((shmidx != (uint16_t)-1) ? ptl->am_ep[shmidx].epaddr : 0);
	tok.ptl = ptl_gen;
	tok.mq = ptl->ep->mq;
	tok.shmidx = shmidx;

	uint16_t hidx = (uint16_t) pkt->handleridx;
	uint32_t bulkidx = pkt->bulkidx;
	uintptr_t bulkptr;
	am_pkt_bulk_t *bulkpkt;

	fn = (psmi_handler_fn_t) psmi_allhandlers[hidx].fn;
	psmi_assert(fn != NULL);
	psmi_assert((uintptr_t) pkt > ptl->self_nodeinfo->amsh_shmbase);

	if (pkt->type == AMFMT_SHORT_INLINE) {
		_HFI_VDBG
		    ("%s inline flag=%d nargs=%d from_idx=%d pkt=%p hidx=%d\n",
		     isreq ? "request" : "reply", pkt->flag, nargs, shmidx, pkt,
		     hidx);

		fn(&tok, args, nargs, pkt->length > 0 ?
		   (void *)&args[nargs] : NULL, pkt->length);
	} else {
		int isend = 0;
		switch (pkt->type) {
		case AMFMT_LONG_END:
			isend = 1;
		/* fall through */
		case AMFMT_LONG:
		case AMFMT_SHORT:
			if (isreq) {
				bulkptr =
				    (uintptr_t) ptl->self_nodeinfo->qdir.
				    qreqFifoLong;
				bulkptr += bulkidx * amsh_qelemsz.qreqFifoLong;
			} else {
				bulkptr =
				    (uintptr_t) ptl->self_nodeinfo->qdir.
				    qrepFifoLong;
				bulkptr += bulkidx * amsh_qelemsz.qrepFifoLong;
			}
			break;
		default:
			bulkptr = 0;
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "Unknown/unhandled packet type 0x%x",
					  pkt->type);
			return;
		}

		bulkpkt = (am_pkt_bulk_t *) bulkptr;
		_HFI_VDBG("ep=%p mq=%p type=%d bulkidx=%d flag=%d/%d nargs=%d "
			  "from_idx=%d pkt=%p/%p hidx=%d\n",
			  ptl->ep, ptl->ep->mq, pkt->type, bulkidx, pkt->flag,
			  bulkpkt->flag, nargs, shmidx, pkt, bulkpkt, hidx);
		psmi_assert(bulkpkt->flag == QREADY);

		if (nargs > NSHORT_ARGS || isend == 1) {
			/* Either there are more args in the bulkpkt, or this is the last
			   packet of a long payload.  In either case, copy the args. */
			int i;
			args =
			    alloca((NSHORT_ARGS +
				    NBULK_ARGS) * sizeof(psm2_amarg_t));

			for (i = 0; i < NSHORT_ARGS; i++) {
				args[i] = pkt->args[i];
			}

			for (; i < nargs; i++) {
				args[i] = bulkpkt->args[i - NSHORT_ARGS];
			}
		}

		if (pkt->type == AMFMT_SHORT) {
			fn(&tok, args, nargs,
			   (void *)bulkpkt->payload, bulkpkt->len);
			QMARKFREE(bulkpkt);
		} else {
			amsh_shm_copy_long((void *)(bulkpkt->dest +
						    bulkpkt->dest_off),
					   bulkpkt->payload, bulkpkt->len);

			/* If this is the last packet, copy args before running the
			 * handler */
			if (isend) {
				void *dest = (void *)bulkpkt->dest;
				size_t len =
				    (size_t) (bulkpkt->dest_off + bulkpkt->len);
				QMARKFREE(bulkpkt);
				fn(&tok, args, nargs, dest, len);
			} else
				QMARKFREE(bulkpkt);
		}
	}
	return;
}

static
psm2_error_t
amsh_mq_rndv(ptl_t *ptl, psm2_mq_t mq, psm2_mq_req_t req,
	     psm2_epaddr_t epaddr, psm2_mq_tag_t *tag, const void *buf,
	     uint32_t len)
{
	psm2_amarg_t args[5];
	psm2_error_t err = PSM2_OK;

	args[0].u32w0 = MQ_MSG_LONGRTS;
	args[0].u32w1 = len;
	args[1].u32w1 = tag->tag[0];
	args[1].u32w0 = tag->tag[1];
	args[2].u32w1 = tag->tag[2];
	args[3].u64w0 = (uint64_t) (uintptr_t) req;
	args[4].u64w0 = (uint64_t) (uintptr_t) buf;

	psmi_assert(req != NULL);
	req->type = MQE_TYPE_SEND;
	req->req_data.buf = (void *)buf;
	req->req_data.buf_len = len;
	req->req_data.send_msglen = len;
	req->send_msgoff = 0;

#ifdef PSM_CUDA
	/* If the send buffer is on gpu, we create a cuda IPC
	 * handle and send it as payload in the RTS */
	if (req->is_buf_gpu_mem) {
		CUdeviceptr buf_base_ptr;
		PSMI_CUDA_CALL(cuMemGetAddressRange, &buf_base_ptr, NULL, (CUdeviceptr)buf);

		/* Offset in GPU buffer from which we copy data, we have to
			* send it separetly because this offset is lost
			* when cuIpcGetMemHandle  is called */
		req->cuda_ipc_offset = buf - (void*)buf_base_ptr;
		args[2].u32w0 = (uint32_t)req->cuda_ipc_offset;

		PSMI_CUDA_CALL(cuIpcGetMemHandle,
				&req->cuda_ipc_handle,
				(CUdeviceptr) buf);
		if (req->flags_internal & PSMI_REQ_FLAG_FASTPATH) {
			psmi_am_reqq_add(AMREQUEST_SHORT, ptl,
						epaddr, mq_handler_hidx,
						args, 5, (void*)&req->cuda_ipc_handle,
						sizeof(CUipcMemHandle), NULL, 0);
		} else {
			psmi_amsh_short_request(ptl, epaddr, mq_handler_hidx,
						args, 5, (void*)&req->cuda_ipc_handle,
						sizeof(CUipcMemHandle), 0);
		}
		req->cuda_ipc_handle_attached = 1;
	} else
#endif
	if (req->flags_internal & PSMI_REQ_FLAG_FASTPATH) {
		psmi_am_reqq_add(AMREQUEST_SHORT, ptl, epaddr, mq_handler_hidx,
					args, 5, NULL, 0, NULL, 0);
	} else {
		psmi_amsh_short_request(ptl, epaddr, mq_handler_hidx,
					args, 5, NULL, 0, 0);
	}

	mq->stats.tx_num++;
	mq->stats.tx_shm_num++;
	mq->stats.tx_rndv_num++;
	// tx_rndv_bytes tabulated when get CTS

	return err;
}

PSMI_ALWAYS_INLINE(
psm2_error_t
amsh_mq_send_inner_eager(psm2_mq_t mq, psm2_mq_req_t req, psm2_epaddr_t epaddr,
			psm2_amarg_t *args, uint32_t flags_user, uint32_t flags_internal,
			psm2_mq_tag_t *tag, const void *ubuf, uint32_t len))
{
	uint32_t bytes_left = len;
	uint32_t bytes_this = 0;

	psm2_handler_t handler = mq_handler_hidx;

	args[1].u32w1 = tag->tag[0];
	args[1].u32w0 = tag->tag[1];
	args[2].u32w1 = tag->tag[2];
	args[2].u32w0 = 0;

	if (!flags_user && len <= AMLONG_MTU) {
		if (len <= 32)
			args[0].u32w0 = MQ_MSG_TINY;
		else
			args[0].u32w0 = MQ_MSG_SHORT;
	} else {
		args[0].u32w0 = MQ_MSG_EAGER;
		args[0].u32w1 = len;
	}

	do {
		args[2].u32w0 += bytes_this;
		bytes_this = min(bytes_left, AMLONG_MTU);

		/* Assume that shared-memory active messages are delivered in order */
		if (flags_internal & PSMI_REQ_FLAG_FASTPATH) {
			psmi_am_reqq_add(AMREQUEST_SHORT, epaddr->ptlctl->ptl,
					epaddr, handler, args, 3, (void *)ubuf,
					bytes_this, NULL, 0);
		} else {
			psmi_amsh_short_request(epaddr->ptlctl->ptl, epaddr,
						handler, args, 3, ubuf, bytes_this, 0);
		}

		ubuf = (void *)((uint8_t *)ubuf + bytes_this);
		bytes_left -= bytes_this;
		handler = mq_handler_data_hidx;
	} while(bytes_left);

	/* All eager async sends are always "all done" */
	if (req != NULL) {
		req->state = MQ_STATE_COMPLETE;
		mq_qq_append(&mq->completed_q, req);
	}

	mq->stats.tx_num++;
	mq->stats.tx_shm_num++;
	mq->stats.tx_shm_bytes += len;
	mq->stats.tx_eager_num++;
	mq->stats.tx_eager_bytes += len;

	return PSM2_OK;
}

/*
 * All shared am mq sends, req can be NULL
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
amsh_mq_send_inner(psm2_mq_t mq, psm2_mq_req_t req, psm2_epaddr_t epaddr,
		   uint32_t flags_user, uint32_t flags_internal, psm2_mq_tag_t *tag,
		   const void *ubuf, uint32_t len))
{
	psm2_amarg_t args[3];
	psm2_error_t err = PSM2_OK;
	int is_blocking = (req == NULL);

#ifdef PSM_CUDA
	int gpu_mem = 0;
	int ep_supports_p2p = (1 << ((am_epaddr_t *) epaddr)->gpuid) & gpu_p2p_supported();

	if (PSMI_IS_CUDA_ENABLED && PSMI_IS_CUDA_MEM(ubuf)) {
		gpu_mem = 1;

		/* All sends from a gpu buffer use the rendezvous protocol if p2p is supported */
		if (ep_supports_p2p) {
			goto do_rendezvous;
		}

		/*
		 * Use eager messages if P2P is unsupported between endpoints.
		 * Potentially use rendezvous with blocking requests only.
		 */
		if (!is_blocking)
			goto do_eager;
	}
#endif
	if (flags_user & PSM2_MQ_FLAG_SENDSYNC)
		goto do_rendezvous;

	if (len <= mq->shm_thresh_rv)
#ifdef PSM_CUDA
do_eager:
#endif
		return amsh_mq_send_inner_eager(mq, req, epaddr, args, flags_user,
						flags_internal, tag, ubuf, len);
do_rendezvous:
	if (is_blocking) {
		req = psmi_mq_req_alloc(mq, MQE_TYPE_SEND);
		if_pf(req == NULL)
			return PSM2_NO_MEMORY;
		req->req_data.send_msglen = len;
		req->req_data.tag = *tag;

		/* Since SEND command is blocking, this request is
		 * entirely internal and we will not be exposed to user.
		 * Setting as internal so it will not be added to
		 * mq->completed_q */
		req->flags_internal |= (flags_internal | PSMI_REQ_FLAG_IS_INTERNAL);
	}
#ifdef PSM_CUDA
	void *host_buf = NULL;

	req->is_buf_gpu_mem = gpu_mem;
	if (req->is_buf_gpu_mem) {
		psmi_cuda_set_attr_sync_memops(ubuf);

		/* Use host buffer for blocking requests if GPU P2P is
		 * unsupported between endpoints.
		 * This will be only used with blocking requests. */
		if (!ep_supports_p2p) {
			host_buf = psmi_malloc(epaddr->ptlctl->ep, UNDEFINED, len);
			PSMI_CUDA_CALL(cuMemcpyDtoH, host_buf, (CUdeviceptr)ubuf, len);

			/* Reset is_buf_gpu_mem since host buffer is being used
			 * instead of one from GPU. */
			ubuf = host_buf;
			req->is_buf_gpu_mem = 0;
		}
	}
#endif

	err = amsh_mq_rndv(epaddr->ptlctl->ptl, mq, req, epaddr, tag, ubuf, len);

	if (err == PSM2_OK && is_blocking) {	/* wait... */
		err = psmi_mq_wait_internal(&req);
	}

#ifdef PSM_CUDA
	if (err == PSM2_OK && host_buf)
		psmi_free(host_buf);
#endif

	return err;
}

static
psm2_error_t
amsh_mq_isend(psm2_mq_t mq, psm2_epaddr_t epaddr, uint32_t flags_user,
	      uint32_t flags_internal, psm2_mq_tag_t *tag, const void *ubuf,
	      uint32_t len, void *context, psm2_mq_req_t *req_o)
{
	psm2_mq_req_t req = psmi_mq_req_alloc(mq, MQE_TYPE_SEND);
	if_pf(req == NULL)
	    return PSM2_NO_MEMORY;

	req->req_data.send_msglen = len;
	req->req_data.tag = *tag;
	req->req_data.context = context;
	req->flags_user = flags_user;
	req->flags_internal = flags_internal;
	_HFI_VDBG("[ishrt][%s->%s][n=0][b=%p][l=%d][t=%08x.%08x.%08x]\n",
		  psmi_epaddr_get_name(epaddr->ptlctl->ep->epid),
		  psmi_epaddr_get_name(epaddr->epid), ubuf, len,
		  tag->tag[0], tag->tag[1], tag->tag[2]);

	amsh_mq_send_inner(mq, req, epaddr, flags_user, flags_internal, tag, ubuf, len);

	*req_o = req;
	return PSM2_OK;
}

static
psm2_error_t
amsh_mq_send(psm2_mq_t mq, psm2_epaddr_t epaddr, uint32_t flags,
	     psm2_mq_tag_t *tag, const void *ubuf, uint32_t len)
{
	_HFI_VDBG("[shrt][%s->%s][n=0][b=%p][l=%d][t=%08x.%08x.%08x]\n",
		  psmi_epaddr_get_name(epaddr->ptlctl->ep->epid),
		  psmi_epaddr_get_name(epaddr->epid), ubuf, len,
		  tag->tag[0], tag->tag[1], tag->tag[2]);

	amsh_mq_send_inner(mq, NULL, epaddr, flags, PSMI_REQ_FLAG_NORMAL, tag, ubuf, len);

	return PSM2_OK;
}

/* kassist-related handling */
int psmi_epaddr_pid(psm2_epaddr_t epaddr)
{
	uint16_t shmidx = ((am_epaddr_t *) epaddr)->shmidx;
	return ((struct ptl_am *)(epaddr->ptlctl->ptl))->am_ep[shmidx].pid;
}
#if _HFI_DEBUGGING
static
const char *psmi_kassist_getmode(int mode)
{
	switch (mode) {
	case PSMI_KASSIST_OFF:
		return "kassist off";
	case PSMI_KASSIST_CMA_GET:
		return "cma get";
	case PSMI_KASSIST_CMA_PUT:
		return "cma put";
	default:
		return "unknown";
	}
}
#endif

static
int psmi_get_kassist_mode()
{
	/* Cuda PSM2 supports only KASSIST_CMA_GET */
	int mode = PSMI_KASSIST_CMA_GET;
#ifndef PSM_CUDA
	union psmi_envvar_val env_kassist;

	if (!psmi_getenv("PSM3_KASSIST_MODE",
			 "PSM Shared memory kernel assist mode "
			 "(cma-put, cma-get, none)",
			 PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
			 (union psmi_envvar_val)
			 PSMI_KASSIST_MODE_DEFAULT_STRING, &env_kassist)) {
		char *s = env_kassist.e_str;
		if (strcasecmp(s, "cma-put") == 0)
			mode = PSMI_KASSIST_CMA_PUT;
		else if (strcasecmp(s, "cma-get") == 0)
			mode = PSMI_KASSIST_CMA_GET;
		else
			mode = PSMI_KASSIST_OFF;
	}
#endif
	return mode;
}

/* Connection handling for shared memory AM.
 *
 * arg0 => conn_op, result (PSM error type)
 * arg1 => epid (always)
 * arg2 => pid, version.
 * arg3 => pointer to error for replies.
 */
static
void
amsh_conn_handler(void *toki, psm2_amarg_t *args, int narg, void *buf,
		  size_t len)
{
	int op = args[0].u16w0;
	int phase = args[0].u32w1;
	psm2_epid_t epid = args[1].u64w0;
	int16_t return_shmidx = args[0].u16w1;
	psm2_error_t err = (psm2_error_t) args[2].u32w1;
	psm2_error_t *perr = (psm2_error_t *) (uintptr_t) args[3].u64w0;
	unsigned int pid;
	unsigned int gpuid;
	int force_remap = 0;

	psm2_epaddr_t epaddr;
	amsh_am_token_t *tok = (amsh_am_token_t *) toki;
	uint16_t shmidx = tok->shmidx;
	int is_valid;
	struct ptl_am *ptl = (struct ptl_am *)(tok->ptl);
	ptl_t *ptl_gen = tok->ptl;
	int cstate;

	/* We do this because it's an assumption below */
	psmi_assert_always(buf == NULL && len == 0);
	read_extra_ep_data(args[2].u32w0, &pid, &gpuid);

	_HFI_CONNDBG("Conn op=%d, phase=%d, epid=0x%llx, err=%d\n",
		  op, phase, (unsigned long long)epid, err);

	switch (op) {
	case PSMI_AM_CONN_REQ:
		_HFI_CONNDBG("Connect from 0x%"PRIx64":%"PRIu64"\n",
			  psm2_epid_nid(epid), psm2_epid_context(epid));
		epaddr = psmi_epid_lookup(ptl->ep, epid);
		if (epaddr && ((am_epaddr_t *) epaddr)->pid != pid) {
			/* If old pid is unknown consider new pid the correct one */
			if (((am_epaddr_t *) epaddr)->pid == AMSH_PID_UNKNOWN) {
				((am_epaddr_t *) epaddr)->pid = pid;
				((am_epaddr_t *) epaddr)->gpuid = gpuid;
			} else {
				psmi_epid_remove(ptl->ep, epid);
				epaddr = NULL;
				force_remap = 1;
			}
		}

		if (shmidx == (uint16_t)-1) {
			/* incoming packet will never be from our shmidx slot 0
			   thus the other process doesn't know our return info.
			   attach_to will lookup or create the proper shmidx */
			if ((err = psmi_shm_map_remote(ptl_gen, epid, &shmidx, force_remap))) {
				psmi_handle_error(PSMI_EP_NORETURN, err,
						  "Fatal error in "
						  "connecting to shm segment");
			}
			am_update_directory(&ptl->am_ep[shmidx]);
			tok->shmidx = shmidx;
		}

		if (epaddr == NULL) {
			uintptr_t args_segoff =
				(uintptr_t) args - ptl->self_nodeinfo->amsh_shmbase;
			if ((err = amsh_epaddr_add(ptl_gen, epid, shmidx, &epaddr)))
				/* Unfortunately, no way out of here yet */
				psmi_handle_error(PSMI_EP_NORETURN, err,
						  "Fatal error "
						  "in connecting to shm segment");
			args =
			    (psm2_amarg_t *) (ptl->self_nodeinfo->amsh_shmbase +
					     args_segoff);

			((am_epaddr_t *) epaddr)->pid = pid;
			((am_epaddr_t *) epaddr)->gpuid = gpuid;
		}

		/* Rewrite args */
		ptl->connect_incoming++;
		args[0].u16w0 = PSMI_AM_CONN_REP;
		/* and return our shmidx for the connecting process */
		args[0].u16w1 = shmidx;
		args[1].u64w0 = (psm2_epid_t) ptl->epid;
		args[2].u32w0 = create_extra_ep_data();
		args[2].u32w1 = PSM2_OK;
		((am_epaddr_t *) epaddr)->cstate_incoming =
			AMSH_CSTATE_INCOMING_ESTABLISHED;
		((am_epaddr_t *) epaddr)->return_shmidx = return_shmidx;
		tok->tok.epaddr_incoming = epaddr;	/* adjust token */
		psmi_amsh_short_reply(tok, amsh_conn_handler_hidx,
				      args, narg, NULL, 0, 0);
		break;

	case PSMI_AM_CONN_REP:
		if (ptl->connect_phase != phase) {
			_HFI_CONNDBG("Out of phase connect reply exp %d got %d\n", ptl->connect_phase, phase);
			return;
		}
		epaddr = ptl->am_ep[shmidx].epaddr;
		/* check if a race has occurred on shm-file reuse.
		 * if so, don't transition to the next state.
		 * the next call to connreq_poll() will restart the
		 * connection.
		*/
		if (ptl->am_ep[shmidx].pid !=
		    ((struct am_ctl_nodeinfo *) ptl->am_ep[shmidx].amsh_shmbase)->pid)
			break;

		*perr = err;
		((am_epaddr_t *) epaddr)->cstate_outgoing
			= AMSH_CSTATE_OUTGOING_REPLIED;
		((am_epaddr_t *) epaddr)->return_shmidx = return_shmidx;
		ptl->connect_outgoing++;
		_HFI_CONNDBG("CCC epaddr=%s connected to ptl=%p\n",
			  psmi_epaddr_get_name(epaddr->epid), ptl);
		break;

	case PSMI_AM_DISC_REQ:
		epaddr = psmi_epid_lookup(ptl->ep, epid);
		if (!epaddr) {
			_HFI_CONNDBG("Dropping disconnect request from an epid that we are not connected to 0x%"PRIx64"\n", epid);
			return;
		}
		args[0].u16w0 = PSMI_AM_DISC_REP;
		args[2].u32w1 = PSM2_OK;
		((am_epaddr_t *) epaddr)->cstate_incoming =
			AMSH_CSTATE_INCOMING_DISC_REQUESTED;
		ptl->connect_incoming--;
		/* Before sending the reply, make sure the process
		 * is still connected */

		if (ptl->am_ep[shmidx].epid != epaddr->epid)
			is_valid = 0;
		else
			is_valid = 1;

		if (is_valid) {
			psmi_amsh_short_reply(tok, amsh_conn_handler_hidx,
					      args, narg, NULL, 0, 0);
			/**
			* Only munmap if we have nothing more to
			* communicate with the other node, i.e. we are
			* already disconnected with the other node
			* or have sent a disconnect request.
			*/
			cstate = ((am_epaddr_t *) epaddr)->cstate_outgoing;
			if (cstate == AMSH_CSTATE_OUTGOING_DISC_REQUESTED) {
				err = psmi_do_unmap(ptl->am_ep[shmidx].amsh_shmbase);
				psmi_epid_remove(epaddr->ptlctl->ep, epaddr->epid);
			}
		}
		break;

	case PSMI_AM_DISC_REP:
		if (ptl->connect_phase != phase) {
			_HFI_CONNDBG("Out of phase disconnect reply exp %d got %d\n", ptl->connect_phase, phase);
			return;
		}
		if (perr)
			*perr = err;
		epaddr = tok->tok.epaddr_incoming;
		((am_epaddr_t *) epaddr)->cstate_outgoing =
			AMSH_CSTATE_OUTGOING_DISC_REPLIED;
		ptl->connect_outgoing--;
		break;

	default:
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Unknown/unhandled connect handler op=%d",
				  op);
		break;
	}
	return;
}

static
size_t amsh_sizeof(void)
{
	return sizeof(struct ptl_am);
}

/* Fill in AM capabilities parameters */
psm2_error_t
psmi_amsh_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters)
{
	if (parameters == NULL) {
		return PSM2_PARAM_ERR;
	}

	parameters->max_handlers = PSMI_AM_NUM_HANDLERS;
	parameters->max_nargs = PSMI_AM_MAX_ARGS;
	parameters->max_request_short = AMLONG_MTU;
	parameters->max_reply_short = AMLONG_MTU;

	return PSM2_OK;
}

/**
 * @param ep PSM Endpoint, guaranteed to have initialized epaddr and epid.
 * @param ptl Pointer to caller-allocated space for PTL (fill in)
 * @param ctl Pointer to caller-allocated space for PTL-control
 *            structure (fill in)
 */
static
psm2_error_t
amsh_init(psm2_ep_t ep, ptl_t *ptl_gen, ptl_ctl_t *ctl)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;

	/* Preconditions */
	psmi_assert_always(ep != NULL);
	psmi_assert_always(ep->epaddr != NULL);
	psmi_assert_always(ep->epid != 0);

	ptl->ep = ep;		/* back pointer */
	ptl->epid = ep->epid;	/* cache epid */
	ptl->epaddr = ep->epaddr;	/* cache a copy */
	ptl->ctl = ctl;
	ptl->zero_polls = 0;

	ptl->connect_phase = 0;
	ptl->connect_incoming = 0;
	ptl->connect_outgoing = 0;

	memset(&ptl->amsh_empty_shortpkt, 0, sizeof(ptl->amsh_empty_shortpkt));
	memset(&ptl->psmi_am_reqq_fifo, 0, sizeof(ptl->psmi_am_reqq_fifo));

	ptl->max_ep_idx = -1;
	ptl->am_ep_size = AMSH_DIRBLOCK_SIZE;

	ptl->am_ep = (struct am_ctl_nodeinfo *)
		psmi_memalign(ptl->ep, PER_PEER_ENDPOINT, 64,
			      ptl->am_ep_size * sizeof(struct am_ctl_nodeinfo));

	if (ptl->am_ep == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}
	memset(ptl->am_ep, 0, ptl->am_ep_size * sizeof(struct am_ctl_nodeinfo));

	if ((err = amsh_init_segment(ptl_gen)))
		goto fail;

	ptl->self_nodeinfo->psm_verno = PSMI_VERNO;
	if (ptl->psmi_kassist_mode != PSMI_KASSIST_OFF) {
		if (cma_available()) {
			ptl->self_nodeinfo->amsh_features |=
				AMSH_HAVE_CMA;
			psmi_shm_mq_rv_thresh =
				PSMI_MQ_RV_THRESH_CMA;
		} else {
			ptl->psmi_kassist_mode =
				PSMI_KASSIST_OFF;
			psmi_shm_mq_rv_thresh =
				PSMI_MQ_RV_THRESH_NO_KASSIST;
		}
	} else {
		psmi_shm_mq_rv_thresh =
			PSMI_MQ_RV_THRESH_NO_KASSIST;
	}
	ptl->self_nodeinfo->pid = getpid();
	ptl->self_nodeinfo->epid = ep->epid;
	ptl->self_nodeinfo->epaddr = ep->epaddr;

	ips_mb();
	ptl->self_nodeinfo->is_init = 1;

	psmi_am_reqq_init(ptl_gen);
	memset(ctl, 0, sizeof(*ctl));

	/* Fill in the control structure */
	ctl->ep = ep;
	ctl->ptl = ptl_gen;
	ctl->ep_poll = amsh_poll;
	ctl->ep_connect = amsh_ep_connect;
	ctl->ep_disconnect = amsh_ep_disconnect;

	ctl->mq_send = amsh_mq_send;
	ctl->mq_isend = amsh_mq_isend;

	ctl->am_get_parameters = psmi_amsh_am_get_parameters;
	ctl->am_short_request = psmi_amsh_am_short_request;
	ctl->am_short_reply = psmi_amsh_am_short_reply;

#if 0	// unused code, specific to QLogic MPI
	/* No stats in shm (for now...) */
	ctl->epaddr_stats_num = NULL;
	ctl->epaddr_stats_init = NULL;
	ctl->epaddr_stats_get = NULL;
#endif
#ifdef PSM_CUDA
	union psmi_envvar_val env_memcache_enabled;
	psmi_getenv("PSM3_CUDA_MEMCACHE_ENABLED",
		    "PSM cuda ipc memhandle cache enabled (default is enabled)",
		     PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		     (union psmi_envvar_val)
		      1, &env_memcache_enabled);
	if (PSMI_IS_CUDA_ENABLED && env_memcache_enabled.e_uint) {
		union psmi_envvar_val env_memcache_size;
		psmi_getenv("PSM3_CUDA_MEMCACHE_SIZE",
			    "Size of the cuda ipc memhandle cache ",
			    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			    (union psmi_envvar_val)
			    CUDA_MEMHANDLE_CACHE_SIZE, &env_memcache_size);
		if ((err = am_cuda_memhandle_cache_init(env_memcache_size.e_uint) != PSM2_OK))
			goto fail;
	}
#endif
fail:
	return err;
}

static psm2_error_t amsh_fini(ptl_t *ptl_gen, int force, uint64_t timeout_ns)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	struct psmi_eptab_iterator itor;
	psm2_epaddr_t epaddr;
	psm2_error_t err = PSM2_OK;
	psm2_error_t err_seg;
	uint64_t t_start = get_cycles();
	int i = 0;

	/* Close whatever has been left open -- this will be factored out for 2.1 */
	if (ptl->connect_outgoing > 0) {
		int num_disc = 0;
		int *mask;
		psm2_error_t *errs;
		psm2_epaddr_t *epaddr_array;

		psmi_epid_itor_init(&itor, ptl->ep);
		while ((epaddr = psmi_epid_itor_next(&itor))) {
			if (epaddr->ptlctl->ptl != ptl_gen)
				continue;
			if (((am_epaddr_t *) epaddr)->cstate_outgoing ==
			    AMSH_CSTATE_OUTGOING_ESTABLISHED)
				num_disc++;
		}
		psmi_epid_itor_fini(&itor);
		if (! num_disc)
			goto poll;

		mask =
		    (int *)psmi_calloc(ptl->ep, UNDEFINED, num_disc,
				       sizeof(int));
		errs = (psm2_error_t *)
		    psmi_calloc(ptl->ep, UNDEFINED, num_disc,
				sizeof(psm2_error_t));
		epaddr_array = (psm2_epaddr_t *)
		    psmi_calloc(ptl->ep, UNDEFINED, num_disc,
				sizeof(psm2_epaddr_t));

		if (errs == NULL || epaddr_array == NULL || mask == NULL) {
			if (epaddr_array)
				psmi_free(epaddr_array);
			if (errs)
				psmi_free(errs);
			if (mask)
				psmi_free(mask);
			err = PSM2_NO_MEMORY;
			goto fail;
		}
		psmi_epid_itor_init(&itor, ptl->ep);
		while ((epaddr = psmi_epid_itor_next(&itor))) {
			if (epaddr->ptlctl->ptl == ptl_gen) {
				if (((am_epaddr_t *) epaddr)->cstate_outgoing ==
				    AMSH_CSTATE_OUTGOING_ESTABLISHED) {
					mask[i] = 1;
					epaddr_array[i] = epaddr;
					i++;
				}
			}
		}
		psmi_epid_itor_fini(&itor);
		psmi_assert(i == num_disc && num_disc > 0);
		err = amsh_ep_disconnect(ptl_gen, force, num_disc, epaddr_array,
					 mask, errs, timeout_ns);
		psmi_free(mask);
		psmi_free(errs);
		psmi_free(epaddr_array);
	}

poll:
	if (ptl->connect_incoming > 0 || ptl->connect_outgoing > 0) {
		_HFI_CONNDBG("CCC polling disconnect from=%d,to=%d to=%"PRIu64" phase %d\n",
			  ptl->connect_incoming, ptl->connect_outgoing, timeout_ns, ptl->connect_phase);
		while (ptl->connect_incoming > 0 || ptl->connect_outgoing > 0) {
			if (!psmi_cycles_left(t_start, timeout_ns)) {
				err = PSM2_TIMEOUT;
				_HFI_CONNDBG("CCC timed out with from=%d,to=%d\n",
					  ptl->connect_incoming, ptl->connect_outgoing);
				break;
			}
			psmi_poll_internal(ptl->ep, 1);
		}
		_HFI_CONNDBG("CCC done polling disconnect from=%d,to=%d\n",
			  ptl->connect_incoming, ptl->connect_outgoing);
	} else
		_HFI_CONNDBG("CCC complete disconnect from=%d,to=%d\n",
			  ptl->connect_incoming, ptl->connect_outgoing);

	if ((err_seg = psmi_shm_detach(ptl_gen))) {
		err = err_seg;
		goto fail;
	}

	/* This prevents poll calls between now and the point where the endpoint is
	 * deallocated to reference memory that disappeared */
	ptl->repH.head = &ptl->amsh_empty_shortpkt;
	ptl->reqH.head = &ptl->amsh_empty_shortpkt;

	if (ptl->am_ep)
		psmi_free(ptl->am_ep);

#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED)
		am_cuda_memhandle_cache_map_fini();
#endif
	return PSM2_OK;
fail:
	return err;

}

static
psm2_error_t
amsh_setopt(const void *component_obj, int optname,
	    const void *optval, uint64_t optlen)
{
	/* No options for AM PTL at the moment */
	return psmi_handle_error(NULL, PSM2_PARAM_ERR,
				 "Unknown AM ptl option %u.", optname);
}

static
psm2_error_t
amsh_getopt(const void *component_obj, int optname,
	    void *optval, uint64_t *optlen)
{
	/* No options for AM PTL at the moment */
	return psmi_handle_error(NULL, PSM2_PARAM_ERR,
				 "Unknown AM ptl option %u.", optname);
}

/* Only symbol we expose out of here */
struct ptl_ctl_init
psmi_ptl_amsh = {
	amsh_sizeof, amsh_init, amsh_fini, amsh_setopt, amsh_getopt
};
