/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2009 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006-2007 Voltaire. All rights reserved.
 * Copyright (c) 2009-2010 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2010-2018 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2020      Google, LLC. All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 *  * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer listed
 *   in this license in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------[Copyright from inclusion of MPICH code]----------------
 *
 * The following is a notice of limited availability of the code, and disclaimer
 * which must be included in the prologue of the code and in all source listings
 * of the code.
 *
 * Copyright Notice
 *  + 2002 University of Chicago
 *
 * Permission is hereby granted to use, reproduce, prepare derivative works, and
 * to redistribute to others.  This software was authored by:
 *
 * Mathematics and Computer Science Division
 * Argonne National Laboratory, Argonne IL 60439
 *
 * (and)
 *
 * Department of Computer Science
 * University of Illinois at Urbana-Champaign
 *
 *
 * 			      GOVERNMENT LICENSE
 *
 * Portions of this material resulted from work developed under a U.S.
 * Government Contract and are subject to the following license: the Government
 * is granted for itself and others acting on its behalf a paid-up,
 * nonexclusive, irrevocable worldwide license in this computer software to
 * reproduce, prepare derivative works, and perform publicly and display
 * publicly.
 *
 * 				  DISCLAIMER
 *
 * This computer code material was prepared, in part, as an account of work
 * sponsored by an agency of the United States Government.  Neither the United
 * States, nor the University of Chicago, nor any of their employees, makes any
 * warranty express or implied, or assumes any legal liability or responsibility
 * for the accuracy, completeness, or usefulness of any information, apparatus,
 * product, or process disclosed, or represents that its use would not infringe
 * privately owned rights.
 */
/*
 * Copyright (c) 2023 Amazon.com, Inc. or its affiliates. All rights reserved.
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

/*
 * Multi Writer, Single Reader FIFO Queue (Not Thread Safe)
 * This implementation of this Queue is a one directional linked list
 * with head/tail pointers where every pointer is a relative offset
 * into the Shared Memory Region.
 */

#ifndef _SM2_FIFO_H_
#define _SM2_FIFO_H_

#include "sm2.h"
#include "sm2_atom.h"
#include <stdint.h>

#define SM2_FIFO_FREE (-3)

static inline void *sm2_relptr_to_absptr(int64_t relptr, struct sm2_mmap *map)
{
	return (void *) (map->base + relptr);
}

static inline int64_t sm2_absptr_to_relptr(void *absptr, struct sm2_mmap *map)
{
	return (int64_t) ((char *) absptr - map->base);
}

struct sm2_fifo {
	uintptr_t head;
	uintptr_t tail;
};

/* Initialize FIFO queue to empty state */
static inline void sm2_fifo_init(struct sm2_fifo *fifo)
{
	fifo->head = SM2_FIFO_FREE;
	fifo->tail = SM2_FIFO_FREE;
}

/* Write, Enqueue */
static inline void sm2_fifo_write(struct sm2_ep *ep, sm2_gid_t peer_gid,
				  struct sm2_xfer_entry *xfer_entry)
{
	struct sm2_region *peer_region = sm2_mmap_ep_region(ep->mmap, peer_gid);
	struct sm2_fifo *peer_fifo = sm2_recv_queue(peer_region);
	long int offset = sm2_absptr_to_relptr(xfer_entry, ep->mmap);
	struct sm2_xfer_entry *prev_xfer_entry;
	long int prev;

	assert(peer_fifo->head != 0);
	assert(peer_fifo->tail != 0);
	assert(offset != 0);

	xfer_entry->hdr.next = SM2_FIFO_FREE;

	atomic_wmb();
	prev = atomic_swap_ptr(&peer_fifo->tail, offset);
	atomic_rmb();

	assert(prev != offset);

	if (SM2_FIFO_FREE != prev) {
		prev_xfer_entry = sm2_relptr_to_absptr(prev, ep->mmap);
		prev_xfer_entry->hdr.next = offset;
	} else {
		peer_fifo->head = offset;
	}

	atomic_wmb();
}

/* Read, Dequeue */
static inline struct sm2_xfer_entry *sm2_fifo_read(struct sm2_ep *ep)
{
	struct sm2_fifo *self_fifo = sm2_recv_queue(ep->self_region);
	struct sm2_xfer_entry *xfer_entry;
	uintptr_t prev_head;

	assert(self_fifo->head != 0);
	assert(self_fifo->tail != 0);

	if (SM2_FIFO_FREE == self_fifo->head)
		return NULL;

	atomic_rmb();

	prev_head = self_fifo->head;
	xfer_entry = (struct sm2_xfer_entry *) sm2_relptr_to_absptr(prev_head,
								    ep->mmap);
	self_fifo->head = SM2_FIFO_FREE;

	assert(xfer_entry->hdr.next != prev_head);
	assert(xfer_entry != 0);
	assert(xfer_entry->hdr.next != 0);

	if (SM2_FIFO_FREE == xfer_entry->hdr.next) {
		atomic_rmb();
		if (!atomic_compare_exchange(&self_fifo->tail, &prev_head,
					     SM2_FIFO_FREE)) {
			while (SM2_FIFO_FREE == xfer_entry->hdr.next)
				atomic_rmb();
			self_fifo->head = xfer_entry->hdr.next;
		}
	} else {
		self_fifo->head = xfer_entry->hdr.next;
	}

	atomic_wmb();
	return xfer_entry;
}

static inline void sm2_fifo_write_back(struct sm2_ep *ep,
				       struct sm2_xfer_entry *xfer_entry)
{
	xfer_entry->hdr.proto_flags |= SM2_RETURN;
	assert(xfer_entry->hdr.sender_gid != ep->gid);
	sm2_fifo_write(ep, xfer_entry->hdr.sender_gid, xfer_entry);
}

static inline void
sm2_fifo_write_back_ipc_host_to_dev(struct sm2_ep *ep,
				    struct sm2_xfer_entry *xfer_entry)
{
	/* This function is called by the sender after the CUDA memcpy is
	 * complete. Receiver generates receive completion after receiving this
	 * message */
	sm2_gid_t receiver_gid = xfer_entry->hdr.sender_gid;

	xfer_entry->hdr.proto_flags &= ~SM2_CMA_HOST_TO_DEV;
	xfer_entry->hdr.proto_flags |= SM2_CMA_HOST_TO_DEV_ACK;
	assert(xfer_entry->hdr.sender_gid != ep->gid);
	xfer_entry->hdr.sender_gid = ep->gid;
	sm2_fifo_write(ep, receiver_gid, xfer_entry);
}

#endif /* _SM2_FIFO_H_ */
