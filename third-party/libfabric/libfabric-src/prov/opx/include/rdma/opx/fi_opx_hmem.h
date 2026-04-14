/*
 * Copyright (C) 2023-2025 by Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HMEM_H_
#define _FI_PROV_OPX_HMEM_H_

#include <assert.h>
#include <rdma/hfi/hfi1_user.h>
#include "rdma/opx/fi_opx_compiler.h"
#include "rdma/opx/fi_opx_rma_ops.h"
#include "rdma/opx/opx_tracer.h"
#include "ofi_hmem.h"

#if HAVE_ROCR
#define __HIP_PLATFORM_AMD__ 0
#include <hip/hip_runtime.h>
#endif

#define OPX_HMEM_NO_HANDLE		   (0)
#define OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET (-1L)

enum opx_hmem_return_code {
	OPX_HMEM_ERROR = -1,
	OPX_HMEM_SUCCESS,
	OPX_HMEM_ERROR_NOT_READY,
};

#define OPX_HMEM_MEMCPY_ASYNC_DTOD 1

#ifdef OPX_HMEM
#define OPX_HMEM_DEV_REG_SEND_THRESHOLD (opx_ep->domain->hmem_domain->devreg_copy_from_threshold)
#define OPX_HMEM_DEV_REG_RECV_THRESHOLD (opx_ep->domain->hmem_domain->devreg_copy_to_threshold)
#else
#define OPX_HMEM_DEV_REG_SEND_THRESHOLD (OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET)
#define OPX_HMEM_DEV_REG_RECV_THRESHOLD (OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET)
#endif

struct fi_opx_hmem_info {
	uint64_t	   device;
	uint64_t	   hmem_dev_reg_handle;
	enum fi_hmem_iface iface;
	uint8_t		   is_unified;
	uint8_t		   unused[3];
} __attribute__((__packed__)) __attribute__((aligned(8)));

OPX_COMPILE_TIME_ASSERT((sizeof(struct fi_opx_hmem_info) & 0x7) == 0,
			"sizeof(fi_opx_hmem_info) should be a multiple of 8");

__OPX_FORCE_INLINE__
enum fi_hmem_iface opx_hmem_get_ptr_iface(const void *ptr, uint64_t *device, uint64_t *is_unified)
{
#ifdef OPX_HMEM
#if HAVE_CUDA
	unsigned mem_type;
	unsigned is_managed;
	unsigned device_ordinal;

	/* Each pointer in 'data' needs to have the same array index
	   as the corresponding attribute in 'cuda_attributes' */
	void *data[] = {&mem_type, &is_managed, &device_ordinal};

	enum CUpointer_attribute_enum cuda_attributes[] = {
		CU_POINTER_ATTRIBUTE_MEMORY_TYPE, CU_POINTER_ATTRIBUTE_IS_MANAGED, CU_POINTER_ATTRIBUTE_DEVICE_ORDINAL};

	CUresult cuda_rc =
		cuPointerGetAttributes(ARRAY_SIZE(cuda_attributes), cuda_attributes, data, (CUdeviceptr) ptr);

	if (cuda_rc == CUDA_SUCCESS) {
		*is_unified = is_managed;
		if (mem_type == CU_MEMORYTYPE_DEVICE && !is_managed) {
			*device = device_ordinal;
			return FI_HMEM_CUDA;
		} else {
			*device = 0UL;
			return FI_HMEM_SYSTEM;
		}
	} else if (cuda_rc != CUDA_ERROR_INVALID_CONTEXT) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Bad return code %hu from cuPointerGetAttributes()",
			     cuda_rc);
	}
#else
	*is_unified		 = 0UL;
	enum fi_hmem_iface iface = ofi_get_hmem_iface(ptr, device, NULL);
	return iface;
#endif
#endif

	*is_unified = 0UL;
	*device	    = 0UL;
	return FI_HMEM_SYSTEM;
}

__OPX_FORCE_INLINE__
enum fi_hmem_iface opx_hmem_get_mr_iface(const struct fi_opx_mr *desc, uint64_t *device, uint64_t *handle)
{
#ifdef OPX_HMEM
	if (desc && !desc->hmem_unified) {
		switch (desc->attr.iface) {
		case FI_HMEM_CUDA:
			*device = desc->attr.device.cuda;
			break;
		case FI_HMEM_ZE:
			*device = desc->attr.device.ze;
			break;
		default:
			*device = 0ul;
		}
		*handle = desc->hmem_dev_reg_handle;
		return desc->attr.iface;
	}
#endif
	*device = 0ul;
	*handle = OPX_HMEM_NO_HANDLE;
	return FI_HMEM_SYSTEM;
}

__OPX_FORCE_INLINE__
int opx_copy_to_hmem(enum fi_hmem_iface iface, uint64_t device, uint64_t hmem_handle, void *dest, const void *src,
		     size_t len, int64_t threshold)
{
	// These functions should never be called for regular host memory.
	// Calling this function directly should only ever be done in code
	// paths where we know iface != FI_HMEM_SYSTEM. Otherwise, the
	// OPX_HMEM_COPY_* macros should be used
	assert(iface != FI_HMEM_SYSTEM);

	int ret;

	assert(hmem_handle == OPX_HMEM_NO_HANDLE || threshold != OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "COPY-TO-HMEM");
	switch (iface) {
#if HAVE_CUDA
	case FI_HMEM_CUDA:
		if ((hmem_handle != 0) && (len <= threshold)) {
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "GDRCOPY-TO-DEV");
			cuda_gdrcopy_to_dev(hmem_handle, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "GDRCOPY-TO-DEV");
			ret = 0;
		} else {
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "CUDAMEMCPY-TO-HMEM");
			ret = (int) cudaMemcpy(dest, src, len, cudaMemcpyHostToDevice);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "CUDAMEMCPY-TO-HMEM");
		}
		break;
#endif

#if HAVE_ROCR
	case FI_HMEM_ROCR:
		if ((hmem_handle != 0) && (len <= threshold)) {
			/* Perform a device registered copy */
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "AMD-DEV-REG-COPY-TO-DEV");
			ret = rocr_dev_reg_copy_to_hmem(hmem_handle, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "AMD-DEV-REG-COPY-TO-DEV");
		} else {
			/* Perform standard rocr_memcopy*/
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "AMD-ROCR-MEMCOPY-TO-HMEM");
			ret = rocr_copy_to_dev(device, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "AMD-ROCR-MEMCOPY-TO-HMEM");
		}
		break;
#endif

	default:
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "OFI-COPY-TO-HMEM");
		ret = ofi_copy_to_hmem(iface, device, dest, src, len);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "OFI-COPY-TO-HMEM");
		break;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "COPY-TO-HMEM");
	return ret;
}

__OPX_FORCE_INLINE__
int opx_copy_from_hmem(enum fi_hmem_iface iface, uint64_t device, uint64_t hmem_handle, void *dest, const void *src,
		       size_t len, int64_t threshold)
{
	// These functions should never be called for regular host memory.
	// Calling this function directly should only ever be done in code
	// paths where we know iface != FI_HMEM_SYSTEM. Otherwise, the
	// OPX_HMEM_COPY_* macros should be used
	assert(iface != FI_HMEM_SYSTEM);

	int ret;

	assert(hmem_handle == OPX_HMEM_NO_HANDLE || threshold != OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "COPY-FROM-HMEM");
	switch (iface) {
#if HAVE_CUDA
	case FI_HMEM_CUDA:
		if ((hmem_handle != 0) && (len <= threshold)) {
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "GDRCOPY-FROM-DEV");
			cuda_gdrcopy_from_dev(hmem_handle, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "GDRCOPY-FROM-DEV");
			ret = 0;
		} else {
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "CUDAMEMCPY-FROM-HMEM");
			ret = (int) cudaMemcpy(dest, src, len, cudaMemcpyDeviceToHost);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "CUDAMEMCPY-FROM-HMEM");
		}
		break;
#endif

#if HAVE_ROCR
	case FI_HMEM_ROCR:
		if ((hmem_handle != 0) && (len <= threshold)) {
			/* Perform a device registered copy */
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "AMD-DEV-REG-COPY-FROM-DEV");
			ret = rocr_dev_reg_copy_from_hmem(hmem_handle, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "AMD-DEV-REG-COPY-FROM-DEV");
		} else {
			/* Perform standard rocr_memcopy*/
			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "AMD-ROCR-MEMCOPY-FROM-HMEM");
			ret = rocr_copy_from_dev(device, dest, src, len);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "AMD-ROCR-MEMCOPY-FROM-HMEM");
		}
		break;
#endif

	default:
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "OFI-COPY-FROM-HMEM");
		ret = ofi_copy_from_hmem(iface, device, dest, src, len);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "OFI-COPY-FROM-HMEM");
		break;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "COPY-FROM-HMEM");
	return ret;
}

__OPX_FORCE_INLINE__
unsigned opx_hmem_iov_init(const void *buf, const size_t len, const void *desc, struct fi_opx_hmem_iov *iov,
			   uint64_t *handle)
{
	iov->buf = (uintptr_t) buf;
	iov->len = len;
#ifdef OPX_HMEM
	uint64_t	   hmem_device;
	enum fi_hmem_iface hmem_iface;
	if (desc) {
		hmem_iface = opx_hmem_get_mr_iface(desc, &hmem_device, handle);
	} else {
		uint64_t is_unified __attribute__((__unused__));
		hmem_iface = opx_hmem_get_ptr_iface(buf, &hmem_device, &is_unified);
		*handle	   = OPX_HMEM_NO_HANDLE;
	}
	iov->iface  = hmem_iface;
	iov->device = hmem_device;
	return (hmem_iface != FI_HMEM_SYSTEM);
#else
	iov->iface  = FI_HMEM_SYSTEM;
	iov->device = 0ul;
	*handle	    = OPX_HMEM_NO_HANDLE;
	return 0;
#endif
}

static const unsigned OPX_HMEM_KERN_MEM_TYPE[4] = {
#ifdef OPX_HMEM
	HFI1_MEMINFO_TYPE_SYSTEM, HFI1_MEMINFO_TYPE_NVIDIA, 2, /* HFI1_MEMINFO_TYPE_AMD */
	1						       /* HFI1_MEMINFO_TYPE_DMABUF */
#endif
};

static const unsigned OPX_HMEM_OFI_MEM_TYPE[4] = {
#ifdef OPX_HMEM
	FI_HMEM_SYSTEM, /* HFI1_MEMINFO_TYPE_SYSTEM */
	FI_HMEM_ZE,	/* HFI1_MEMINFO_TYPE_DMABUF */
	FI_HMEM_ROCR,	/* HFI1_MEMINFO_TYPE_AMD    */
	FI_HMEM_CUDA	/* HFI1_MEMINFO_TYPE_NVIDIA */
#endif
};

#ifdef OPX_HMEM
#define OPX_HMEM_COPY_FROM(dst, src, len, handle, threshold, src_iface, src_device)                  \
	do {                                                                                         \
		if (src_iface == FI_HMEM_SYSTEM) {                                                   \
			memcpy(dst, src, len);                                                       \
		} else {                                                                             \
			opx_copy_from_hmem(src_iface, src_device, handle, dst, src, len, threshold); \
		}                                                                                    \
	} while (0)

#define OPX_HMEM_COPY_TO(dst, src, len, handle, threshold, dst_iface, dst_device)                  \
	do {                                                                                       \
		if (dst_iface == FI_HMEM_SYSTEM) {                                                 \
			memcpy(dst, src, len);                                                     \
		} else {                                                                           \
			opx_copy_to_hmem(dst_iface, dst_device, handle, dst, src, len, threshold); \
		}                                                                                  \
	} while (0)

#define OPX_HMEM_ATOMIC_DISPATCH(src, dst, len, dt, op, dst_iface, dst_device)                            \
	do {                                                                                              \
		if (dst_iface == FI_HMEM_SYSTEM) {                                                        \
			fi_opx_rx_atomic_dispatch(src, dst, len, dt, op);                                 \
		} else {                                                                                  \
			uint8_t hmem_buf[OPX_HFI1_MAX_PKT_SIZE];                                          \
			opx_copy_from_hmem(dst_iface, dst_device, OPX_HMEM_NO_HANDLE, hmem_buf, dst, len, \
					   OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);                           \
			fi_opx_rx_atomic_dispatch(src, hmem_buf, len, dt, op);                            \
			opx_copy_to_hmem(dst_iface, dst_device, OPX_HMEM_NO_HANDLE, dst, hmem_buf, len,   \
					 OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);                             \
		}                                                                                         \
	} while (0)

#else

#define OPX_HMEM_COPY_FROM(dst, src, len, handle, threshold, src_iface, src_device) \
	do {                                                                        \
		memcpy(dst, src, len);                                              \
		(void) src_iface;                                                   \
	} while (0)

#define OPX_HMEM_COPY_TO(dst, src, len, handle, threshold, dst_iface, dst_device) \
	do {                                                                      \
		memcpy(dst, src, len);                                            \
		(void) dst_iface;                                                 \
	} while (0)

#define OPX_HMEM_ATOMIC_DISPATCH(src, dst, len, dt, op, dst_iface, dst_device) \
	do {                                                                   \
		fi_opx_rx_atomic_dispatch(src, dst, len, dt, op);              \
	} while (0)

#endif // OPX_HMEM

union opx_hmem_stream {
	uint64_t *handle;
#if HAVE_CUDA
	CUstream cu_stream;
#endif
#if HAVE_ROCR
	hipStream_t hip_stream;
#endif
};

union opx_hmem_event {
	uint64_t *handle;
#if HAVE_CUDA
	CUevent cu_event;
#endif
#if HAVE_ROCR
	hipEvent_t hip_event;
#endif
};

__OPX_FORCE_INLINE__
void opx_hmem_dbg_trace(enum fi_hmem_iface iface, char *string, int result)
{
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		const char *error_string = NULL;
		cuGetErrorString(result, &error_string);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s CUresult=%d (%s)\n", string, result,
			     error_string ? error_string : "unknown error");
		return;
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s hipError_t=%d (%s)\n", string, result,
			     hipGetErrorString(result));
		return;
	}
#endif
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s result=%d with invalid iface (%d)\n", string, result,
		     iface);
}

__OPX_FORCE_INLINE__
void opx_hmem_warn_trace(enum fi_hmem_iface iface, char *string, int result)
{
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		const char *error_string = NULL;
		cuGetErrorString(result, &error_string);
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "%s CUresult=%d (%s)\n", string, result,
			error_string ? error_string : "unknown error");
		return;
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "%s hipError_t=%d (%s)\n", string, result,
			hipGetErrorString(result));
		return;
	}
#endif
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "%s result=%d with invalid iface (%d)\n", string, result, iface);
}

/**
 * @brief Create a stream for GPU operations to complete
 *
 * @param stream Pointer to the opx stream that was created
 * @return 0 on success, -1 on failure.
 */
__OPX_FORCE_INLINE__
int opx_hmem_stream_create(enum fi_hmem_iface iface, union opx_hmem_stream **stream)
{
	int		       result	  = OPX_HMEM_ERROR;
	union opx_hmem_stream *new_stream = (union opx_hmem_stream *) calloc(1, sizeof(*new_stream));
	if (OFI_UNLIKELY(!new_stream)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "Error allocating memory for HMEM stream\n");
		return OPX_HMEM_ERROR;
	}
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuStreamCreate(&new_stream->cu_stream, CU_STREAM_DEFAULT);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipStreamCreate(&new_stream->hip_stream);
	}
#endif
	if (result) {
		opx_hmem_dbg_trace(iface, "Error creating the HMEM stream", result);
		free(new_stream);
		return OPX_HMEM_ERROR;
	}
	*stream = new_stream;
	return OPX_HMEM_SUCCESS;
}

/**
 * @brief Destroy the stream used for GPU operations
 *
 * @param stream The opx stream to destroy
 */
__OPX_FORCE_INLINE__
void opx_hmem_stream_destroy(enum fi_hmem_iface iface, union opx_hmem_stream *stream)
{
	if (stream) {
#if HAVE_CUDA
		if (iface == FI_HMEM_CUDA) {
			cuStreamDestroy(stream->cu_stream);
		}
#endif
#if HAVE_ROCR
		if (iface == FI_HMEM_ROCR) {
			hipStreamDestroy(stream->hip_stream);
		}
#endif
		free(stream);
	}
}

/**
 * @brief Wait for the oustanding GPU operations on the stream to complete
 *
 * @param stream The opx stream to be synchronized
 */
__OPX_FORCE_INLINE__
void opx_hmem_stream_synchronize(enum fi_hmem_iface iface, union opx_hmem_stream *stream)
{
	int result = OPX_HMEM_ERROR;
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuStreamSynchronize(stream->cu_stream);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipStreamSynchronize(stream->hip_stream);
	}
#endif
	if (result) {
		opx_hmem_warn_trace(iface, "Error synchronizing the stream", result);
		abort();
	}
}

#ifdef OPX_HMEM
/**
 * @brief Create an event to record in order to sync GPU operations on a stream
 *
 * @param domain Pointer to the opx domain containing the event pool for allocation
 * @param event Pointer to the opx event that was created
 * @return 0 on success, -1 on failure.
 */
__OPX_FORCE_INLINE__
int opx_hmem_event_create(enum fi_hmem_iface iface, struct opx_hmem_domain *domain, union opx_hmem_event **event)
{
	int		      result	= OPX_HMEM_ERROR;
	union opx_hmem_event *new_event = ofi_buf_alloc(domain->hmem_stream.event_pool);
	if (OFI_UNLIKELY(!new_event)) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "Error allocating memory for HMEM event\n");
		return OPX_HMEM_ERROR;
	}
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuEventCreate(&new_event->cu_event, CU_EVENT_DEFAULT);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipEventCreate(&new_event->hip_event);
	}
#endif
	if (result) {
		opx_hmem_dbg_trace(iface, "Error creating the event", result);
		free(new_event);
		return OPX_HMEM_ERROR;
	}
	*event = new_event;
	return OPX_HMEM_SUCCESS;
}
#endif

/**
 * @brief Destroy an event
 *
 * @param event The opx event to be destroyed
 */
__OPX_FORCE_INLINE__
void opx_hmem_event_destroy(enum fi_hmem_iface iface, union opx_hmem_event **event)
{
	if (*event) {
#if HAVE_CUDA
		if (iface == FI_HMEM_CUDA) {
			cuEventDestroy((*event)->cu_event);
		}
#endif
#if HAVE_ROCR
		if (iface == FI_HMEM_ROCR) {
			hipEventDestroy((*event)->hip_event);
		}
#endif
		ofi_buf_free(*event);
		*event = NULL;
	}
}

/**
 * @brief Create an event to record in order to sync GPU operations on a stream
 *
 * @param event The opx event to record for later synchronization
 * @param stream The opx stream on which to record the event
 * @return 0 on success, -1 on failure.
 */
__OPX_FORCE_INLINE__
int opx_hmem_event_record(enum fi_hmem_iface iface, union opx_hmem_event *event, union opx_hmem_stream *stream)
{
	int result = OPX_HMEM_ERROR;
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuEventRecord(event->cu_event, stream->cu_stream);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipEventRecord(event->hip_event, stream->hip_stream);
	}
#endif
	if (result) {
		opx_hmem_dbg_trace(iface, "Error recording an event on the steam", result);
		return OPX_HMEM_ERROR;
	}
	return OPX_HMEM_SUCCESS;
}

/**
 * @brief Synchronize the recorded HMEM event
 *
 * @param event The opx event on which to wait for synchronization
 * @return 0 on success, -1 on failure.
 */
__OPX_FORCE_INLINE__
int opx_hmem_event_synchronize(enum fi_hmem_iface iface, union opx_hmem_event **event)
{
	int result = OPX_HMEM_ERROR;
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuEventSynchronize((*event)->cu_event);
		opx_hmem_event_destroy(FI_HMEM_CUDA, event);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipEventSynchronize((*event)->hip_event);
		opx_hmem_event_destroy(FI_HMEM_ROCR, event);
	}
#endif

	if (result) {
		opx_hmem_dbg_trace(iface, "Error on event synchronize", result);
		abort();
	}
	return OPX_HMEM_SUCCESS;
}

/**
 * @brief Check the status of a recorded event
 *
 * @param event Pointer to the opx stream that was created
 * @return 0 event completion, 1 the event is still pending, -1 error
 */
__OPX_FORCE_INLINE__
enum opx_hmem_return_code opx_hmem_event_query(enum fi_hmem_iface iface, union opx_hmem_event *event)
{
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		CUresult result = cuEventQuery(event->cu_event);
		if (result == CUDA_SUCCESS) {
			return OPX_HMEM_SUCCESS;
		} else if (result == CUDA_ERROR_NOT_READY) {
			return OPX_HMEM_ERROR_NOT_READY;
		} else {
			return OPX_HMEM_ERROR;
		}
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		hipError_t result = hipEventQuery(event->hip_event);
		if (result == hipSuccess) {
			return OPX_HMEM_SUCCESS;
		} else if (result == hipErrorNotReady) {
			return OPX_HMEM_ERROR_NOT_READY;
		} else {
			return OPX_HMEM_ERROR;
		}
	}
#endif
	return OPX_HMEM_ERROR;
}

/**
 * @brief Issue an asynchronous HMEM DtoD memcpy on the given stream
 *
 * @param iface HMEM interface type
 * @param dst Pointer to the destination device
 * @param src Pointer to the source device
 * @param size Length to be copied
 * @param stream The opx HMEM stream on which the copy should be started
 * @return  0 on success, -1 on failure.
 */
__OPX_FORCE_INLINE__
int opx_hmem_memcpy_async_DtoD(enum fi_hmem_iface iface, void *dst, const void *src, size_t size,
			       union opx_hmem_stream *stream)
{
	int result = OPX_HMEM_ERROR;
#if HAVE_CUDA
	if (iface == FI_HMEM_CUDA) {
		result = cuMemcpyDtoDAsync((CUdeviceptr) dst, (CUdeviceptr) src, size, stream->cu_stream);
	}
#endif
#if HAVE_ROCR
	if (iface == FI_HMEM_ROCR) {
		result = hipMemcpyAsync(dst, src, size, hipMemcpyDeviceToDevice, stream->hip_stream);
	}
#endif
	if (result) {
		opx_hmem_dbg_trace(iface, "Error on the asynchronous copy", result);
		return OPX_HMEM_ERROR;
	}
	return OPX_HMEM_SUCCESS;
}

#ifdef OPX_HMEM
/**
 * @brief Kick off an asynchronous HMEM memcpy
 *
 * This function starts an asynchronous HMEM memcpy and creates the event to query for completion.
 * If one has not already been created, create a new HMEM stream and save it on the HMEM domain.
 * After it determines a stream is available, it creates an event to record after the memcpy is started.
 * If the asynchronous copy has any failure, it tries to complete a synchronous copy.  If the synchronous
 * copy also fails, the function will abort.
 *
 * @param iface HMEM interface type
 * @param dst Pointer to the destination device
 * @param src Pointer to the source device
 * @param size Length to be copied
 * @param domain Pointer to the opx domain containing the HMEM stream and event pool
 * @param event Pointer to the opx event that was created
 */
__OPX_FORCE_INLINE__
void opx_hmem_memcpy_async(enum fi_hmem_iface iface, uint64_t device, void *dst, const void *src, size_t size,
			   struct opx_hmem_domain *domain, union opx_hmem_event **event, int copy_type)
{
	union opx_hmem_event *new_event;
	int		      ret;

	if (domain->hmem_stream.stream == NULL) {
		ret = opx_hmem_stream_create(iface, &domain->hmem_stream.stream);
		if (ret) {
			goto err;
		}
		domain->hmem_stream.type = iface;
	}

	ret = opx_hmem_event_create(iface, domain, &new_event);
	if (ret) {
		goto err;
	}

	assert(copy_type == OPX_HMEM_MEMCPY_ASYNC_DTOD);
	ret = opx_hmem_memcpy_async_DtoD(iface, dst, src, size, domain->hmem_stream.stream);
	if (ret) {
		opx_hmem_event_destroy(iface, &new_event);
		goto err;
	}

	ret = opx_hmem_event_record(iface, new_event, domain->hmem_stream.stream);
	if (ret) {
		opx_hmem_stream_synchronize(iface, domain->hmem_stream.stream);
		opx_hmem_event_destroy(iface, &new_event);
	}

	*event = new_event;
	return;

err:
	ret = ofi_copy_to_hmem(iface, device, dst, src, size);
	if (ret) {
		opx_hmem_dbg_trace(iface, "Error trying to synchronously copy", ret);
		abort();
	}
}
#endif

#endif
