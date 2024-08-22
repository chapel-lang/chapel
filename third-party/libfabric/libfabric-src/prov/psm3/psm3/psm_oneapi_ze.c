/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2021 Intel Corporation.

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

  Copyright(c) 2021 Intel Corporation.

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
#ifdef PSM_ONEAPI
#include <dirent.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <sys/poll.h>
#include "psm_user.h"
#include "psm_mq_internal.h"
#include "ptl_am/psm_am_internal.h"
#include "psmi_wrappers.h"

#ifndef PSM_HAVE_PIDFD
static int psm3_ze_dev_fds[MAX_ZE_DEVICES];
int psm3_num_ze_dev_fds;
#endif
int psm3_oneapi_immed_sync_copy;
int psm3_oneapi_immed_async_copy;
unsigned psm3_oneapi_parallel_dtod_copy_thresh;

const char* psmi_oneapi_ze_result_to_string(const ze_result_t result) {
#define ZE_RESULT_CASE(RES) case ZE_RESULT_##RES: return STRINGIFY(RES)

	switch (result) {
	ZE_RESULT_CASE(SUCCESS);
	ZE_RESULT_CASE(NOT_READY);
	ZE_RESULT_CASE(ERROR_UNINITIALIZED);
	ZE_RESULT_CASE(ERROR_DEVICE_LOST);
	ZE_RESULT_CASE(ERROR_INVALID_ARGUMENT);
	ZE_RESULT_CASE(ERROR_OUT_OF_HOST_MEMORY);
	ZE_RESULT_CASE(ERROR_OUT_OF_DEVICE_MEMORY);
	ZE_RESULT_CASE(ERROR_MODULE_BUILD_FAILURE);
	ZE_RESULT_CASE(ERROR_INSUFFICIENT_PERMISSIONS);
	ZE_RESULT_CASE(ERROR_NOT_AVAILABLE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_VERSION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_FEATURE);
	ZE_RESULT_CASE(ERROR_INVALID_NULL_HANDLE);
	ZE_RESULT_CASE(ERROR_HANDLE_OBJECT_IN_USE);
	ZE_RESULT_CASE(ERROR_INVALID_NULL_POINTER);
	ZE_RESULT_CASE(ERROR_INVALID_SIZE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_SIZE);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_ALIGNMENT);
	ZE_RESULT_CASE(ERROR_INVALID_SYNCHRONIZATION_OBJECT);
	ZE_RESULT_CASE(ERROR_INVALID_ENUMERATION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_ENUMERATION);
	ZE_RESULT_CASE(ERROR_UNSUPPORTED_IMAGE_FORMAT);
	ZE_RESULT_CASE(ERROR_INVALID_NATIVE_BINARY);
	ZE_RESULT_CASE(ERROR_INVALID_GLOBAL_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_FUNCTION_NAME);
	ZE_RESULT_CASE(ERROR_INVALID_GROUP_SIZE_DIMENSION);
	ZE_RESULT_CASE(ERROR_INVALID_GLOBAL_WIDTH_DIMENSION);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ARGUMENT_INDEX);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ARGUMENT_SIZE);
	ZE_RESULT_CASE(ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE);
	ZE_RESULT_CASE(ERROR_INVALID_COMMAND_LIST_TYPE);
	ZE_RESULT_CASE(ERROR_OVERLAPPING_REGIONS);
	ZE_RESULT_CASE(ERROR_UNKNOWN);
	default:
		return "Unknown error";
	}

#undef ZE_RESULT_CASE
}

// when allocating bounce buffers either malloc w/Import or
// zeMemAllocHost can be used.  zeMemAllocHost tends to perform
// better in the subsequent GPU copy's AppendMemoryCopy.  However
// zeMemAllocHost results in a GPU-like address which requires dmabuf
// so we can't use zeMemAllocHost for DMA to/from the bounce buffer
// unless rv is available to handle GPU addresses (eg. PSM3_GPUDIRECT=1)

void *psm3_oneapi_ze_host_alloc_malloc(unsigned size)
{
	void *ret_ptr = psmi_malloc(PSMI_EP_NONE, UNDEFINED, size);
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSMI_ONEAPI_ZE_CALL(zexDriverImportExternalPointer, ze_driver, ret_ptr, size);
#endif
	return ret_ptr;
}

void psm3_oneapi_ze_host_free_malloc(void *ptr)
{
#ifndef PSM3_NO_ONEAPI_IMPORT
	PSMI_ONEAPI_ZE_CALL(zexDriverReleaseImportedPointer, ze_driver, ptr);
#endif
	psmi_free(ptr);
}

#ifndef PSM3_USE_ONEAPI_MALLOC
void *psm3_oneapi_ze_host_alloc_zemem(unsigned size)
{
	void *ret_ptr;
	ze_host_mem_alloc_desc_t host_desc = {
		.stype = ZE_STRUCTURE_TYPE_HOST_MEM_ALLOC_DESC,
		.flags = ZE_MEMORY_ACCESS_CAP_FLAG_RW
	};
	PSMI_ONEAPI_ZE_CALL(zeMemAllocHost, ze_context,
						&host_desc, size, 8, &ret_ptr);
	return ret_ptr;
}

void psm3_oneapi_ze_host_free_zemem(void *ptr)
{
	PSMI_ONEAPI_ZE_CALL(zeMemFree, ze_context, ptr);
}

void *(*psm3_oneapi_ze_host_alloc)(unsigned size) = psm3_oneapi_ze_host_alloc_malloc;
void (*psm3_oneapi_ze_host_free)(void *ptr) = psm3_oneapi_ze_host_free_malloc;
int psm3_oneapi_ze_using_zemem_alloc = 0;
#endif /* PSM3_USE_ONEAPI_MALLOC */

// this is only called if GPU Direct is enabled in rv such that
// GDR Copy and/or RDMA MRs can provide GPU-like addresses to rv
void psm3_oneapi_ze_can_use_zemem()
{
#ifndef PSM3_USE_ONEAPI_MALLOC
	psm3_oneapi_ze_host_alloc = psm3_oneapi_ze_host_alloc_zemem;
	psm3_oneapi_ze_host_free = psm3_oneapi_ze_host_free_zemem;
	psm3_oneapi_ze_using_zemem_alloc = 1;
#endif
}

// synchronous GPU memcpy
void psmi_oneapi_ze_memcpy(void *dstptr, const void *srcptr, size_t size)
{
	struct ze_dev_ctxt *ctxt;

	psmi_assert(size > 0);
	ctxt = psmi_oneapi_dev_ctxt_get(dstptr);
	if (!ctxt) {
		ctxt = psmi_oneapi_dev_ctxt_get(srcptr);
		if (!ctxt) {
			_HFI_ERROR("dst %p src %p not GPU buf for copying\n",
				   dstptr, srcptr);
			return;
		}
	}
	if (psm3_oneapi_immed_sync_copy) {
		PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
	} else {
		PSMI_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->cl);
		PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
		PSMI_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->cl);
		PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->cq,
					1, &ctxt->cl, NULL);
		PSMI_ONEAPI_ZE_CALL(zeCommandQueueSynchronize, ctxt->cq, UINT32_MAX);
	}
}

// synchronous GPU memcpy DTOD (xeLink)
void psmi_oneapi_ze_memcpy_DTOD(void *dstptr, const void *srcptr, size_t size)
{
	struct ze_dev_ctxt *ctxt;

	psmi_assert(size > 0);
	ctxt = psmi_oneapi_dev_ctxt_get(dstptr);
	if (!ctxt) {
		_HFI_ERROR("dst %p src %p not GPU buf for copying\n",
			   dstptr, srcptr);
		return;
	}
	if (size <= psm3_oneapi_parallel_dtod_copy_thresh) {
		if (psm3_oneapi_immed_sync_copy) {
			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
		} else {
			PSMI_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->cl);
			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->cl,
					dstptr, srcptr, size, NULL, 0, NULL);
			PSMI_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->cl);
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->cq,
					1, &ctxt->cl, NULL);
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueSynchronize, ctxt->cq, UINT32_MAX);
		}
	} else {
		// for large DTOD copies, start 2 parallel commands
		// then wait for both
		size_t size0 = ROUNDUP64P2(size/2, 64*1024);
		size_t size1 = size - size0;

		if (psm3_oneapi_immed_sync_copy) {
			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl0,
					dstptr, srcptr, size0, ctxt->copy_status0, 0, NULL);

			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl1,
					(void*)((uintptr_t)dstptr+size0),
					(void*)((uintptr_t)srcptr+size0), size1, ctxt->copy_status1,
					0, NULL);
		} else {
			PSMI_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->async_cl0);
			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl0,
					dstptr, srcptr, size0, ctxt->copy_status0, 0, NULL);
			PSMI_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->async_cl0);
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->async_cq0,
					1, &ctxt->async_cl0, NULL);

			PSMI_ONEAPI_ZE_CALL(zeCommandListReset, ctxt->async_cl1);
			PSMI_ONEAPI_ZE_CALL(zeCommandListAppendMemoryCopy, ctxt->async_cl1,
					(void*)((uintptr_t)dstptr+size0),
					(void*)((uintptr_t)srcptr+size0), size1, ctxt->copy_status1,
					0, NULL);
			PSMI_ONEAPI_ZE_CALL(zeCommandListClose, ctxt->async_cl1);
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueExecuteCommandLists, ctxt->async_cq1,
					1, &ctxt->async_cl1, NULL);
		}
		// 2nd copy may be slightly smaller so waity for it first so
		// can potentially hide its Reset latency while 1st copy completes
		PSMI_ONEAPI_ZE_CALL(zeEventHostSynchronize, ctxt->copy_status1, UINT32_MAX);
		PSMI_ONEAPI_ZE_CALL(zeEventHostReset, ctxt->copy_status1);

		PSMI_ONEAPI_ZE_CALL(zeEventHostSynchronize, ctxt->copy_status0, UINT32_MAX);
		PSMI_ONEAPI_ZE_CALL(zeEventHostReset, ctxt->copy_status0);
	}
}

// for pipelined async GPU memcpy
// *p_cq is left as NULL when psm3_oneapi_immed_async_copy enabled
void psmi_oneapi_async_cmd_create(struct ze_dev_ctxt *ctxt,
		ze_command_queue_handle_t *p_cq, ze_command_list_handle_t *p_cl)
{
	psmi_assert(! *p_cl);
	if (psm3_oneapi_immed_async_copy) {
		ze_command_queue_desc_t cq_desc = {
			.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
			.flags = 0,
			.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,
			.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL
		};
		cq_desc.ordinal = ctxt->ordinal;
		cq_desc.index = ctxt->index++;
		ctxt->index %= ctxt->num_queues;
		PSMI_ONEAPI_ZE_CALL(zeCommandListCreateImmediate,
			ze_context, ctxt->dev, &cq_desc, p_cl);
	} else {
		if (! *p_cq) {
			ze_command_queue_desc_t cq_desc = {
				.stype = ZE_STRUCTURE_TYPE_COMMAND_QUEUE_DESC,
				.flags = 0,
				.mode = ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS,
				.priority = ZE_COMMAND_QUEUE_PRIORITY_NORMAL
			};
			cq_desc.ordinal = ctxt->ordinal;
			cq_desc.index = ctxt->index++;
			ctxt->index %= ctxt->num_queues;
			PSMI_ONEAPI_ZE_CALL(zeCommandQueueCreate,
					ze_context, ctxt->dev, &cq_desc, p_cq);
		}
		ze_command_list_desc_t cl_desc = {
			.stype = ZE_STRUCTURE_TYPE_COMMAND_LIST_DESC,
			.flags = 0
		};
		cl_desc.commandQueueGroupOrdinal = ctxt->ordinal;
		PSMI_ONEAPI_ZE_CALL(zeCommandListCreate,
			ze_context, ctxt->dev, &cl_desc, p_cl);
	}
}

#ifndef PSM_HAVE_PIDFD
/*
 * psmi_ze_init_fds - initialize the file descriptors (ze_dev_fds) 
 *
 * Open the file descriptors for our GPUs (psm3_ze_dev_fds[])
 *
 * The file descriptors are used in intra-node communication to pass to peers
 * via socket with sendmsg/recvmsg SCM_RIGHTS message type.
 *
 */

psm2_error_t psm3_ze_init_fds(void)
{
	const char *dev_dir = "/dev/dri/by-path/";
	const char *suffix = "-render";
	DIR *dir;
	struct dirent *ent = NULL;
	char dev_name[NAME_MAX];
	int i = 0, ret;

	if (psm3_num_ze_dev_fds)
		return PSM2_OK;

	dir = opendir(dev_dir);
	if (dir == NULL)
		return PSM2_INTERNAL_ERR;

	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.' ||
		    strstr(ent->d_name, suffix) == NULL)
			continue;

		memset(dev_name, 0, sizeof(dev_name));
		ret = snprintf(dev_name, NAME_MAX, "%s%s", dev_dir, ent->d_name);
		if (ret < 0 || ret >= NAME_MAX) {
			_HFI_INFO("GPU dev name too long: %s%s\n", dev_dir, ent->d_name);
			goto err;
		}

		psm3_ze_dev_fds[i] = open(dev_name, O_RDWR);
		if (psm3_ze_dev_fds[i] == -1) {
			_HFI_INFO("Failed to open %s GPU dev FD: %s\n", dev_name,
					 strerror(errno));
			goto err;
		}
		_HFI_DBG("Opened %s GPU dev FD: %d\n", dev_name,
				psm3_ze_dev_fds[i]);
		i++;
		psm3_num_ze_dev_fds++;
	}
	(void) closedir(dir);
	_HFI_DBG("Opened %d GPU dev FDs\n", psm3_num_ze_dev_fds);
	return PSM2_OK;

err:
	(void) closedir(dir);
	return PSM2_INTERNAL_ERR;
}

/*
 * psmi_ze_get_dev_fds - fetch device file descriptors
 *
 * Returns a pointer to ze_dev_fds while putting the number
 * of fds into the in/out nfds parameter
 *
 */

int *psm3_ze_get_dev_fds(int *nfds)
{
	*nfds = psm3_num_ze_dev_fds;
	return psm3_ze_dev_fds;
}

/*
 * psmi_sendmsg_fds - send device file descriptors over socket w/ sendmsg
 *
 * Prepares message of type SCM_RIGHTS, copies file descriptors as payload,
 * and sends over socket via sendmsg while creating appropriate fd numbers
 * for dest (effectively a dup(2) of our file descriptor)
 *
 * returns -errno on error or number of bytes sent (>0) on success
 */

static int psmi_sendmsg_fds(int sock, int *fds, int nfds, psm2_epid_t epid)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	int64_t peer_id = *(int64_t *)&epid;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = (char *)psmi_calloc(NULL, UNDEFINED, 1, CMSG_SPACE(ctrl_size));
	if (!ctrl_buf)
		return -ENOMEM;

	iov.iov_base = &peer_id;
	iov.iov_len = sizeof(peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);

	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(ctrl_size);
	memcpy(CMSG_DATA(cmsg), fds, ctrl_size);

	ret = sendmsg(sock, &msg, 0);
	if (ret < 0)
		ret = -errno;
	else if (! ret)
		ret = -EAGAIN;

	psmi_free(ctrl_buf);
	return ret;
}

/*
 * psmi_recvmsg_fds - receive device file descriptors from socket w/ recvmsg
 *
 * Prepares message buffer of type SCM_RIGHTS, receives message from socket
 * via recvmsg, and copies device file descriptors to in/out parameter.
 * The received file descriptors are usable in our process and need to
 * be closed when done being used
 *
 * returns -errno on error or number of bytes received (>0) on success
 */

static int psmi_recvmsg_fd(int sock, int *fds, int nfds, psm2_epid_t epid)
{
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	int64_t peer_id = *(int64_t *)&epid;
	char *ctrl_buf;
	size_t ctrl_size;
	int ret;

	ctrl_size = sizeof(*fds) * nfds;
	ctrl_buf = (char *)psmi_calloc(NULL, UNDEFINED, 1, CMSG_SPACE(ctrl_size));
	if (!ctrl_buf)
		return -ENOMEM;

	iov.iov_base = &peer_id;
	iov.iov_len = sizeof(peer_id);

	memset(&msg, 0, sizeof(msg));
	msg.msg_control = ctrl_buf;
	msg.msg_controllen = CMSG_SPACE(ctrl_size);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	ret = recvmsg(sock, &msg, 0);
	if (ret < 0) {
		ret = -errno;
	} else if (ret != sizeof(peer_id)) {
		_HFI_CONNDBG("recvmsg from: %s returns %d expect %u\n",
						psm3_epid_fmt_addr(epid, 0), ret,
						(unsigned)sizeof(peer_id) );
		ret = -EAGAIN;
		goto out;
	}

	psmi_assert(!(msg.msg_flags & (MSG_TRUNC | MSG_CTRUNC)));
	cmsg = CMSG_FIRSTHDR(&msg);
	psmi_assert(cmsg && cmsg->cmsg_len == CMSG_LEN(ctrl_size) &&
	       cmsg->cmsg_level == SOL_SOCKET &&
	       cmsg->cmsg_type == SCM_RIGHTS && CMSG_DATA(cmsg));
	memcpy(fds, CMSG_DATA(cmsg), ctrl_size);
out:
	psmi_free(ctrl_buf);
	return ret;
}

/*
 * psm3_ze_init_ipc_socket - initialize ipc socket in ep
 *
 * Set up the AF_UNIX ipc socket in the ep for listen mode. Name it
 * using our epid, and bind it.
 *
 */

psm2_error_t psm3_ze_init_ipc_socket(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	int ret;
	struct sockaddr_un sockaddr = {0};
	socklen_t len = sizeof(sockaddr);

	if ((ptl->ep->ze_ipc_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		_HFI_ERROR("error creating GPU dev FDs AF_UNIX sock: %s\n",
					strerror(errno));
		err =  PSM2_INTERNAL_ERR;
		goto fail;
	}

	sockaddr.sun_family = AF_UNIX;
	snprintf(sockaddr.sun_path, 108, "/dev/shm/psm3_shm.ze_sock2.%ld.%s",
				(long int) getuid(), psm3_epid_fmt_internal(ptl->epid, 0));
	ptl->ep->listen_sockname = psmi_strdup(NULL, sockaddr.sun_path);
	if (ptl->ep->listen_sockname == NULL) {
		err = PSM2_NO_MEMORY;
		goto fail;
	}

	if ((ret = bind(ptl->ep->ze_ipc_socket, (struct sockaddr *) &sockaddr, len)) < 0) {
		_HFI_ERROR("error binding GPU dev FDs AF_UNIX sock to %s: %s\n",
					sockaddr.sun_path, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	if ((ret = listen(ptl->ep->ze_ipc_socket, 256)) < 0) {
		_HFI_ERROR("error listening on GPU dev FDs AF_UNIX sock %s: %s\n",
					sockaddr.sun_path, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	return PSM2_OK;

fail:
	if (ptl->ep->ze_ipc_socket >= 0)
		close(ptl->ep->ze_ipc_socket);
	ptl->ep->ze_ipc_socket = -1;
	if (ptl->ep->listen_sockname)
		psmi_free(ptl->ep->listen_sockname);
	ptl->ep->listen_sockname = NULL;
	return err;
}

/*
 * psm3_receive_ze_dev_fds - receive the dev fds on the listen socket
 *
 * Set up the listen socket to be polled for POLLIN. When the event is
 * received, accept for the new socket and then read the peer epid,
 * and locate the epaddr for it. Then receive the dev fds to be stored
 * in the am_epaddr.
 *
 * returns:
 *		PSM_OK - GPU dev FDs received from a peer
 *		PSM2_OK_NO_PROGRESS - nothing received
 *		other - error
 */

static psm2_error_t psm3_receive_ze_dev_fds(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK;
	struct pollfd fdset;
	int newsock = -1;

	fdset.fd = ptl->ep->ze_ipc_socket;
	fdset.events = POLLIN;

	if (poll(&fdset, 1, 0) <= 0)
		return PSM2_OK_NO_PROGRESS;

	{
		struct sockaddr_un sockaddr = {0};
		socklen_t len = sizeof(sockaddr);
		int nfds = psm3_num_ze_dev_fds;
		int nread;
		psm2_epid_t epid;
		psm2_epaddr_t epaddr;
		am_epaddr_t *am_epaddr;

		newsock = accept(ptl->ep->ze_ipc_socket, (struct sockaddr *)&sockaddr, &len);
		if (newsock < 0) {
			_HFI_ERROR("GPU dev FDs AF_UNIX accept failed: %s\n",
						strerror(errno));
			err =  PSM2_INTERNAL_ERR;
			goto fail;
		} else {
			int ret;
			// technically we could get less than we asked for and need to
			// call recv again in future but our transfers are small enough
			// we should get it all
			if ((nread = recv(newsock, &epid, sizeof(epid), 0)) < 0) {
				_HFI_ERROR("GPU dev FDs AF_UNIX recv failed: %s\n",
							strerror(errno));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			if (nread != sizeof(epid)) {
				_HFI_ERROR("GPU dev FDs AF_UNIX recv incomplete: %d\n", nread);
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			// we only poll for recv FDs after processing a am_shm connect
			// so the epid should always be known
			if ((epaddr = psm3_epid_lookup(ptl->ep, epid)) == NULL) {
				_HFI_ERROR("Peer Unknown, unable to receive GPU dev FDs from: %s\n",
								psm3_epid_fmt_addr(epid, 0));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			am_epaddr = (am_epaddr_t *)epaddr;
			am_epaddr->num_peer_fds = nfds;
			ret = psmi_recvmsg_fd(newsock, am_epaddr->peer_fds, nfds, ptl->epid);
			if (ret <= 0) {
				_HFI_ERROR("Unable to recvmsg %d GPU dev FDs from: %s: %s\n",
								nfds, psm3_epid_fmt_addr(epid, 0),
								strerror(-ret));
				err =  PSM2_INTERNAL_ERR;
				goto fail;
			}
			_HFI_CONNDBG("%d GPU dev FDs Received from: %s\n",
								nfds, psm3_epid_fmt_addr(epid, 0));
		}
	}

fail:
	if (newsock >= 0)
		close(newsock);
	return err;
}

/*
 * psm3_send_dev_fds - do next step to send the dev fds to the peer's
 *		listen socket
 *
 * Check the connected state and proceed accordingly:
 * - ZE_SOCK_NOT_CONNECTED
 *     We have not done anything yet, so connect and send our epid,
 *     followed by the dev fds. Set state to ZE_SOCK_DEV_FDS_SENT
 * - ZE_SOCK_DEV_FDS_SENT
 *     The dev fds have been sent. Issue ioctl to see if the output
 *     queue has been emptied indicating that the peer has read the data.
 *     If so, set state to ZE_SOCK_DEV_FDS_SENT_AND_RECD.
 * - ZE_SOCK_DEV_FDS_SENT_AND_RECD
 *     We are done, just return.
 *
 * returns:
 *		PSM_OK - next step completed
 *		PSM2_OK_NO_PROGRESS - nothing to do
 *		other - error
 */

psm2_error_t psm3_send_dev_fds(ptl_t *ptl_gen, psm2_epaddr_t epaddr)
{
	am_epaddr_t *am_epaddr = (am_epaddr_t *)epaddr;

	switch (am_epaddr->sock_connected_state) {
		case ZE_SOCK_DEV_FDS_SENT_AND_RECD:
			return PSM2_OK_NO_PROGRESS;
			break;

		case ZE_SOCK_DEV_FDS_SENT:
		{
			int pending;

			psmi_assert(am_epaddr->sock >= 0);
			if_pf (ioctl(am_epaddr->sock, SIOCOUTQ, &pending) != 0) {
				return	psm3_handle_error( PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"error sending dev FDs: %s\n", strerror(errno));
			}
			if (pending == 0) {
				am_epaddr->sock_connected_state = ZE_SOCK_DEV_FDS_SENT_AND_RECD;
				_HFI_CONNDBG("GPU dev FDs Send Completed to: %s\n",
								psm3_epid_fmt_addr(epaddr->epid, 0));
				close(am_epaddr->sock);
				am_epaddr->sock = -1;
				return PSM2_OK;
			}
			// be paranoid just in case 1st call to send_dev_fds for given
			// epaddr gets here
			if (! ((struct ptl_am *)ptl_gen)->ep->need_dev_fds_poll)
				_HFI_CONNDBG("restart GPU dev FDs poll\n");
			((struct ptl_am *)ptl_gen)->ep->need_dev_fds_poll = 1;
			return PSM2_OK_NO_PROGRESS;
			break;
		}

		case ZE_SOCK_NOT_CONNECTED:
		{
			struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
			struct sockaddr_un sockaddr = {0};
			socklen_t len = sizeof(sockaddr);
			psm2_epid_t peer_epid = epaddr->epid;
			int *fds, nfds;

			if (!ptl->ep->need_dev_fds_poll)
				_HFI_CONNDBG("restart GPU dev FDs poll\n");
			ptl->ep->need_dev_fds_poll = 1;

			fds = psm3_ze_get_dev_fds(&nfds);

			if ((am_epaddr->sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
				_HFI_ERROR("error creating GPU dev FDs AF_UNIX sock: %s\n",
							strerror(errno));
				goto fail;
			}

			sockaddr.sun_family = AF_UNIX;
			snprintf(sockaddr.sun_path, 108, "/dev/shm/psm3_shm.ze_sock2.%ld.%s",
				(long int) getuid(), psm3_epid_fmt_internal(peer_epid, 0));

			if (connect(am_epaddr->sock, (struct sockaddr *) &sockaddr, len) < 0) {
				_HFI_ERROR("GPU dev FDs connect to %s (via %s) failed: %s\n",
								psm3_epid_fmt_addr(epaddr->epid, 0),
								sockaddr.sun_path,  strerror(errno));
				goto fail;
			} else {
				int ret;
				ret = send(am_epaddr->sock, &ptl->epid, sizeof(ptl->epid), 0);
				if (ret < 0) {
					_HFI_ERROR("GPU dev FDs send to %s (via %s) failed: %s\n",
							psm3_epid_fmt_addr(epaddr->epid, 0),
							sockaddr.sun_path, strerror(errno));
					goto fail;
				}
		
				ret = psmi_sendmsg_fds(am_epaddr->sock, fds, nfds, peer_epid);
				if (ret <= 0) {
					/* ret is -errno */
					_HFI_ERROR("GPU dev FDs sendmsg to %s (via %s) failed: %s\n",
							psm3_epid_fmt_addr(epaddr->epid, 0),
							sockaddr.sun_path,  strerror(-ret));
					goto fail;
				}
				am_epaddr->sock_connected_state = ZE_SOCK_DEV_FDS_SENT;
				_HFI_CONNDBG("%d GPU dev FDs Posted Send to: %s (via %s)\n",
						nfds, psm3_epid_fmt_addr(epaddr->epid, 0),
						sockaddr.sun_path);
				return PSM2_OK;
			}
			/* NOTREACHED */
			break;
		}

		default:
			return PSM2_INTERNAL_ERR;
			break;
	}
	/* NOTREACHED */
	return PSM2_INTERNAL_ERR;

fail:
	if (am_epaddr->sock >= 0)
		close(am_epaddr->sock);
	am_epaddr->sock = -1;
	return PSM2_INTERNAL_ERR;
}

// simple test if dev_fds bi-dir exchange completed for given epaddr
// 1 = yes, 0 = no
static
int psm3_dev_fds_exchanged(psm2_epaddr_t epaddr)
{
	am_epaddr_t *am_epaddr = (am_epaddr_t *)epaddr;
	return (am_epaddr->sock_connected_state == ZE_SOCK_DEV_FDS_SENT_AND_RECD
			&& am_epaddr->num_peer_fds) ;
}

/*
 * psm3_check_dev_fds_exchanged - check that dev fds have been bi-dir exchanged
 * with given peer. Poll to try and move forward as needed.
 *
 * connect state ZE_SOCK_DEV_FDS_SENT_AND_RECD indicates peer has received
 * our send of dev_fds
 *
 * num_peer_fds indicates if we received peer's fds.
 *
 * if both are satisfied, exchange is complete, return PSM2_OK
 *
 *Returns:
 *   PSM2_OK - both are done
 *   PSM2_OK_NO_PROGRESS - more work needed
 *   other - error
 */
psm2_error_t psm3_check_dev_fds_exchanged(ptl_t *ptl_gen, psm2_epaddr_t epaddr)
{
	psm2_error_t err;
	psm2_error_t ret;
	am_epaddr_t *am_epaddr = (am_epaddr_t *)epaddr;

	psmi_assert(epaddr);
	psmi_assert(! psm3_epid_zero_internal(epaddr->epid));

	if (psm3_dev_fds_exchanged(epaddr))
		return PSM2_OK;

	if (am_epaddr->cstate_outgoing != AMSH_CSTATE_OUTGOING_ESTABLISHED
		&& am_epaddr->cstate_incoming != AMSH_CSTATE_INCOMING_ESTABLISHED)
		return PSM2_OK_NO_PROGRESS;

	// try to move forward 1 step
	err = psm3_send_dev_fds(ptl_gen, epaddr);
	if (am_epaddr->sock_connected_state == ZE_SOCK_DEV_FDS_SENT_AND_RECD)
		err = PSM2_OK;
	else /* err will be NO_PROGRESS or worse */
		err = psm3_error_cmp(err, PSM2_OK_NO_PROGRESS);

	// only poll recv if we need to
	ret = PSM2_OK_NO_PROGRESS;	// keep KW happy
	if (am_epaddr->num_peer_fds == 0) 
		ret = psm3_receive_ze_dev_fds(ptl_gen);
	if (am_epaddr->num_peer_fds) 
		ret = PSM2_OK;

	 /* worst err, NO_PROGRESS is worse than PSM2_OK */
	return psm3_error_cmp(ret, err);
}

/*
 * psm3_poll_dev_fds_exchanged - poll to make forward progress on
 * GPU dev FDs exchange
 *
 * Loop through the epaddrs in am_ep and check_dev_fds_exchanged
 *
 * Returns:
 *		PSM2_OK - we found some work to do and made progress
 *		PSM2_OK_NO_PROGRESS - didn't find anything to do
 *		other - error
 */

psm2_error_t psm3_poll_dev_fds_exchange(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;
	psm2_error_t err = PSM2_OK_NO_PROGRESS;
	psm2_error_t ret;
	int i;
	int num_left = 0;

	err = psm3_receive_ze_dev_fds(ptl_gen);

	for (i = 0; i <= ptl->max_ep_idx; i++) {
		am_epaddr_t *am_epaddr = (am_epaddr_t *)ptl->am_ep[i].epaddr;

		if (!am_epaddr || psm3_epid_zero_internal(ptl->am_ep[i].epid))
			continue;

		if (psm3_dev_fds_exchanged(&am_epaddr->epaddr))
			continue;

		num_left++;	// causes one extra poll if complete now below, but no harm

		// don't try if uni-dir REQ/REP is incomplete
		if (am_epaddr->cstate_outgoing != AMSH_CSTATE_OUTGOING_ESTABLISHED
			&& am_epaddr->cstate_incoming != AMSH_CSTATE_INCOMING_ESTABLISHED)
			continue;

		// try to move forward 1 step
		ret = psm3_send_dev_fds(ptl_gen, &am_epaddr->epaddr);
		if (ret > PSM2_OK_NO_PROGRESS)
			err = psm3_error_cmp(ret, err);
		else if (ret == PSM2_OK && err == PSM2_OK_NO_PROGRESS)
			err = ret;
	}
	if (num_left == 0 && ptl->ep->need_dev_fds_poll)
		_HFI_CONNDBG("stop GPU dev FDs poll\n");
	ptl->ep->need_dev_fds_poll = (num_left != 0);

	return err;
}

psm2_error_t psm3_sock_detach(ptl_t *ptl_gen)
{
	struct ptl_am *ptl = (struct ptl_am *)ptl_gen;

	if (ptl->ep->ze_ipc_socket >= 0)
		close(ptl->ep->ze_ipc_socket);
	ptl->ep->ze_ipc_socket = -1;
	if (ptl->ep->listen_sockname) {
		unlink(ptl->ep->listen_sockname);
		psmi_free(ptl->ep->listen_sockname);
	}
	ptl->ep->listen_sockname = NULL;
	return PSM2_OK;
}
#endif /* not PSM_HAVE_PIDFD */

#ifndef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
static int psm3_ipc_handle_cached(const void *buf,
				ze_ipc_mem_handle_t ipc_handle)
{
	static int first = 1;
	static int cached = 0;
	ze_ipc_mem_handle_t tmp_ipc_handle;
	int tmp_fd;

	/* Only detect the first time */
	if (!first)
		return cached;

	PSMI_ONEAPI_ZE_CALL(zeMemGetIpcHandle, ze_context,
			    buf, &tmp_ipc_handle);
	tmp_fd = *(uint32_t *)tmp_ipc_handle.data;
	if (tmp_fd == *(uint32_t *)ipc_handle.data)
		cached = 1;
	else
		close(tmp_fd);

	first = 0;
	_HFI_VDBG("fd %u tmp_fd %d cached %d\n", *(uint32_t *)ipc_handle.data,
						tmp_fd, cached);

	return cached;
}
#endif

#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
// queue for delayed Put to get better GetIpcHandle performance
// while having an upper bound on number of active Ipc Handles
// sized based on PSM3_ONEAPI_PUTQUEUE_SIZE
struct {
	psmi_lock_t lock;
	struct oneapi_handle_array {
		uint8_t valid;
		ze_ipc_mem_handle_t ipc_handle;
	} *array;
	unsigned index;	// where to add next entry and remove oldest
	int size;	// number of slots in queue, -1 disables put
} psm3_oneapi_putqueue;
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */

psm2_error_t psmi_oneapi_putqueue_alloc(void)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	union psmi_envvar_val env;
	psm3_getenv("PSM3_ONEAPI_PUTQUEUE_SIZE",
				"How many Ipc Handle Puts to queue for shm send and nic Direct GPU Access [-1 disables Put, 0 disables queue]",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)ONEAPI_PUTQUEUE_SIZE, &env);
	_HFI_DBG("OneApi PutQueue Size=%d\n", env.e_int);
	psm3_oneapi_putqueue.size = env.e_int;
	if (env.e_int > 0) {
		psm3_oneapi_putqueue.array = (struct oneapi_handle_array *)psmi_calloc(
										PSMI_EP_NONE, UNDEFINED, env.e_int,
										sizeof(*psm3_oneapi_putqueue.array));
		if (! psm3_oneapi_putqueue.array)
			return PSM2_NO_MEMORY;
		psm3_oneapi_putqueue.index = 0;
		psmi_init_lock(&psm3_oneapi_putqueue.lock);
	}
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
	return PSM2_OK;
}

void psm3_put_ipc_handle(const void *buf, ze_ipc_mem_handle_t ipc_handle)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
	if (! psm3_oneapi_putqueue.array) {	// queue disabled
		if (psm3_oneapi_putqueue.size >= 0)	// negative size disables Put
			PSMI_ONEAPI_ZE_CALL(zeMemPutIpcHandle, ze_context, ipc_handle);
		return;
	}
	PSMI_LOCK(psm3_oneapi_putqueue.lock);
	if (psm3_oneapi_putqueue.array[psm3_oneapi_putqueue.index].valid) {
		// Put the oldest one to make room for new entry
		ze_ipc_mem_handle_t tmp_ipc_handle =
			psm3_oneapi_putqueue.array[psm3_oneapi_putqueue.index].ipc_handle;
		PSMI_ONEAPI_ZE_CALL(zeMemPutIpcHandle, ze_context, tmp_ipc_handle);
	}
	// queue the new one
	psm3_oneapi_putqueue.array[psm3_oneapi_putqueue.index].valid = 1;
	psm3_oneapi_putqueue.array[psm3_oneapi_putqueue.index++].ipc_handle = ipc_handle;
	psm3_oneapi_putqueue.index %= psm3_oneapi_putqueue.size;
	PSMI_UNLOCK(psm3_oneapi_putqueue.lock);
#else /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
	// for older Agama with handle "cache" but no reference counting
	// no way to put handle without affecting all IOs using that buffer
	// on ATS w/o Agama handle cache, no benefit to holding onto fd so close
	if (!psm3_ipc_handle_cached(buf, ipc_handle))
		close(*(uint32_t *)ipc_handle.data);
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
}

void psmi_oneapi_putqueue_free(void)
{
#ifdef PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE
#if 0 // we are shutting down, so don't worry about Putting the queued handles
	int i;

	// no need for lock, destroying object, no more callers
	for (i=0; i < psm3_oneapi_putqueue.size; i++) {
		if (psm3_oneapi_putqueue.array[i].valid) {
			ze_ipc_mem_handle_t ipc_handle = psm3_oneapi_putqueue.array[i].ipc_handle;
			PSMI_ONEAPI_ZE_CALL(zeMemPutIpcHandle, ze_context, ipc_handle);
		}
	}
#endif /* 0 */
	if (psm3_oneapi_putqueue.array) {
		psmi_free(psm3_oneapi_putqueue.array);
		psm3_oneapi_putqueue.array = NULL;
		psmi_destroy_lock(&psm3_oneapi_putqueue.lock);
	}
#endif /* PSM_HAVE_ONEAPI_ZE_PUT_IPCHANDLE */
}

/*
 * get OneAPI alloc_id for a GPU address
 *
 * The address should be part of a buffer allocated from an OneAPI
 * library call (zeMemAllocDevice() or zeMemAllocHost()).
 * The alloc_id changes on each OneAPI allocation call. PSM3/rv uses the
 * alloc_id to determine if a cache hit is a potentially stale entry which
 * should be invalidated.
 */
uint64_t psm3_oneapi_ze_get_alloc_id(void *addr, uint8_t *type)
{
	ze_memory_allocation_properties_t mem_props = {
		.stype = ZE_STRUCTURE_TYPE_MEMORY_ALLOCATION_PROPERTIES
	};
	ze_device_handle_t device;

	PSMI_ONEAPI_ZE_CALL(zeMemGetAllocProperties, ze_context,
			    addr, &mem_props, &device);
	if (type)
		*type = (uint8_t)mem_props.type;
	/*
	 * id is unique across all allocates on all devices within a given
	 * process
	 */
	return mem_props.id;
}

#endif // PSM_ONEAPI
