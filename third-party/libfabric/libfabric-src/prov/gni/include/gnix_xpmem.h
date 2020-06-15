/*
 * Copyright (c) 2016      Los Alamos National Security, LLC.
 *                         All rights reserved.
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

#ifndef GNIX_XPMEM_H_
#define GNIX_XPMEM_H_

#include "gnix.h"
#if HAVE_XPMEM
#include <xpmem.h>
#else
typedef int64_t xpmem_apid_t;
typedef int64_t xpmem_segid_t;
#endif

struct gnix_xpmem_handle {
	struct gnix_reference ref_cnt;
	struct gnix_hashtable *apid_ht;
	fastlock_t lock;
};

struct gnix_xpmem_access_handle {
	struct gnix_xpmem_handle *xp_hndl;
	struct gnix_xpmem_ht_entry *entry;
	void *attach_addr;
	void *remote_base_addr;
	size_t access_len;
};

extern bool gnix_xpmem_disabled;

/*******************************************************************************
 * API Prototypes
 ******************************************************************************/

/**
 * @brief create an xpmem handle to use for subsequent
 *        xpmem operations
 *
 * @param [in] dom      pointer to a previously allocated
 *                      gnix_fid_domain struct
 * @param [out] handle  pointer to a memory location where
 *                      a pointer to an xpmem_handle will be
 *                      returned
 *
 * @return FI_SUCCESS   xpmem handle successfully allocated
 * @return -FI_EINVAL   Upon receiving an invalid parameter
 */
int _gnix_xpmem_handle_create(struct gnix_fid_domain *dom,
			      struct gnix_xpmem_handle **handle);

/**
 * @brief destroy an xpmem handle
 *
 * @param [in] handle   pointer to a previously allocated
 *                      xpmem_handle
 * @return FI_SUCCESS   xpmem handle successfully destroyed
 * @return -FI_EINVAL   Upon receiving an invalid parameter
 */
int _gnix_xpmem_handle_destroy(struct gnix_xpmem_handle *hndl);

/**
 * @brief get an access handle to a address range a peer's
 *        address space
 *
 * @param[in] xp_handle         pointer to previously created
 *                              xpmem handle
 * @param[in] peer_apid         xpmem apid for peer
 * @param[in] remote_vaddr      virtual address in process associated
 *                              with the target EP
 * @param[in] len               length in bytes of the region to
 *                              to be accessed in the target process
 * @param[out] access_hndl      access handle to be used to copy data
 *                              from the peer process in to the local
 *                              address space
 *
 * @return FI_SUCCESS   Upon xpmem successfully initialized
 * @return -FI_EINVAL   Upon receiving an invalid parameter
 * @return -FI_ENOSYS   Target EP can't be attached to local process
 *                      address space
 */
int _gnix_xpmem_access_hndl_get(struct gnix_xpmem_handle *xp_hndl,
			     xpmem_apid_t peer_apid,
			     uint64_t remote_vaddr,
			     size_t len,
			     struct gnix_xpmem_access_handle **access_hndl);


/**
 * @brief release an access handle
 *
 * @param[in] access_handle     pointer to previously created
 *                              access handle
 *
 * @return FI_SUCCESS   Upon xpmem successfully initialized
 * @return -FI_EINVAL   Upon receiving an invalid parameter
 */
int _gnix_xpmem_access_hndl_put(struct gnix_xpmem_access_handle *access_hndl);

/**
 * @brief memcpy from previously accessed memory in peer's
 *        virtual address space
 *
 * @param[in] access_hndl       pointer to previously created
 *                              xpmem access handle
 * @param[in] dst_addr          starting virtual address in the calling
 *                              process address space where data
 *                              will be copied
 * @param[in] remote_start_addr   starting virtual address in the target
 *                              address space from which data will be copied
 * @param[in] len		copy length in bytes
 *
 * @return FI_SUCCESS	Upon successful copy
 * @return -FI_EINVAL	Invalid argument
 */
int _gnix_xpmem_copy(struct gnix_xpmem_access_handle *access_hndl,
		     void *dst_addr,
		     void *remote_start_addr,
		     size_t len);

/**
 * @brief get the xpmem segid associated with an xpmem_handle
 *
 * @param[in] xp_handle         pointer to previously created
 *                              will be copied
 * @param[out] seg_id           pointer to memory location where
 *                              the segid value will be returned
 *
 * @return FI_SUCCESS	Upon success
 * @return -FI_EINVAL	Invalid argument
 */
int _gnix_xpmem_get_my_segid(struct gnix_xpmem_handle *xp_hndl,
				xpmem_segid_t *seg_id);

/**
 * @brief get the xpmem apid associated with an xpmem_handle
 *        and input segid
 *
 * @param[in] xp_handle         pointer to previously created
 *                              will be copied
 * @param[in] seg_id            seg_id obtained from process
 *                              whose memory is to be accessed
 *                              via xpmem.
 * @param[out] peer_apid        pointer to memory location where
 *                              the apid value to use for accessing
 *                              the address space of the peer
 *                              process.
 *
 * @return FI_SUCCESS	Upon success
 * @return -FI_EINVAL	Invalid argument
 */
int _gnix_xpmem_get_apid(struct gnix_xpmem_handle *xp_hndl,
				xpmem_segid_t segid,
				xpmem_apid_t *peer_apid);

/**
 * @brief determine if a process at a given gnix_address can
 *        be accessed using xpmem
 *
 * @param[in] ep                pointer to a previously allocated
 *                              gnix_fid_ep structure
 * @param[in] addr              address used by an endpoint of the
 *                              peer process
 * @param[out] accessible       set to true if endpoint with
 *                              gnix_address addr can be accessed
 *                              using xpmem, otherwise false
 *
 * @return FI_SUCCESS	Upon success
 * @return -FI_EINVAL	Invalid argument
 */
int _gnix_xpmem_accessible(struct gnix_fid_ep *ep,
			   struct gnix_address addr,
			   bool *accessible);




#endif /* GNIX_XPMEM_H_ */
