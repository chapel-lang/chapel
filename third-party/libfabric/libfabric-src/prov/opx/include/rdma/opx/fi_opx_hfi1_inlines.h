/*
 * Copyright (C) 2022-2023 by Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HFI1_INLINES_H_
#define _FI_PROV_OPX_HFI1_INLINES_H_

#include <assert.h>
#include <stdlib.h>

#include "rdma/opx/fi_opx_endpoint.h"

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_put(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_PUT |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		OPX_HMEM_COPY_FROM((void *)tx_payload,
				   (const void *)*sbuf, payload_bytes,
				   sbuf_iface, sbuf_device);
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}

	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
void opx_hfi1_dput_write_payload_atomic_fetch(
				union fi_opx_hfi1_packet_payload *tx_payload,
				const uint64_t dput_bytes,
				const uint64_t fetch_vaddr,
				const uintptr_t rma_request_vaddr,
				uint64_t bytes_sent,
				uint8_t *sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device)
{
	struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) tx_payload->byte;
	dput_fetch->fetch_rbuf = fetch_vaddr + bytes_sent;
	dput_fetch->rma_request_vaddr = rma_request_vaddr;

	OPX_HMEM_COPY_FROM((void *)&tx_payload->byte[sizeof(*dput_fetch)],
			   (const void *)sbuf, dput_bytes,
			   sbuf_iface, sbuf_device);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_atomic_fetch(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t rma_request_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	size_t dput_bytes = payload_bytes - sizeof(struct fi_opx_hfi1_dput_fetch);

	if (tx_payload) {
		assert(!iov);
		opx_hfi1_dput_write_payload_atomic_fetch(tx_payload, dput_bytes,
					fetch_vaddr, rma_request_vaddr,
					bytes_sent, *sbuf, sbuf_iface, sbuf_device);

		/* Here the source buffer is the data with no fi_opx_hfi1_dput_fetch
		   contained in it, so we only want to advance the pointer by
		   the number of actual data bytes, not including the dput fetch
		   header info that we injected at the beginning in the target buffer. */
		(*sbuf) += dput_bytes;
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;

		/* Here we're simply setting the IOV pointer, so the source buffer
		   must already be a fully built packet payload, so advance the
		   pointer by the full payload amount */
		(*sbuf) += payload_bytes;
	}

	(*rbuf) += dput_bytes;

	return dput_bytes;
}

__OPX_FORCE_INLINE__
void opx_hfi1_dput_write_payload_atomic_compare_fetch(
				union fi_opx_hfi1_packet_payload *tx_payload,
				const size_t dput_bytes_half,
				const uint64_t fetch_vaddr,
				const uintptr_t rma_request_vaddr,
				uint64_t bytes_sent,
				uint8_t *sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uint8_t *cbuf,
				const enum fi_hmem_iface cbuf_iface,
				const uint64_t cbuf_device)
{
	struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) tx_payload->byte;
	dput_fetch->fetch_rbuf = fetch_vaddr + bytes_sent;
	dput_fetch->rma_request_vaddr = rma_request_vaddr;

	/* The first 1/2 of the actual payload bytes contains the data for the elements
	   we want to write to memory at the destination. The second 1/2 contains the
	   data for the elements to use as compare values against the elements currently
	   in the destination's memory, to see if a swap should take place. */
	OPX_HMEM_COPY_FROM((void *)&tx_payload->byte[sizeof(*dput_fetch)],
			   (const void *)sbuf, dput_bytes_half,
			   sbuf_iface, sbuf_device);
	OPX_HMEM_COPY_FROM((void *)&tx_payload->byte[sizeof(*dput_fetch) + dput_bytes_half],
			   (const void *)cbuf, dput_bytes_half,
			   cbuf_iface, cbuf_device);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_atomic_compare_fetch(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t rma_request_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uint8_t **cbuf,
				const enum fi_hmem_iface cbuf_iface,
				const uint64_t cbuf_device,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	size_t dput_bytes = payload_bytes - sizeof(struct fi_opx_hfi1_dput_fetch);
	size_t dput_bytes_half = dput_bytes >> 1;

	if (tx_payload) {
		assert(!iov);
		opx_hfi1_dput_write_payload_atomic_compare_fetch(tx_payload, dput_bytes_half,
					fetch_vaddr, rma_request_vaddr,
					bytes_sent, *sbuf, sbuf_iface, sbuf_device,
					*cbuf, cbuf_iface, cbuf_device);

		/* Here the source buffer is the data with no fi_opx_hfi1_dput_fetch
		   contained in it, so we only want to advance the pointer by
		   the number of actual data bytes, not including the dput fetch
		   header info that we injected at the beginning in the target buffer. */
		(*sbuf) += dput_bytes_half;
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;

		/* Here we're simply setting the IOV pointer, so the source buffer
		   must already be a fully built packet payload, so advance the
		   pointer by the full payload amount */
		(*sbuf) += payload_bytes;
	}

	(*rbuf) += dput_bytes_half;
	(*cbuf) += dput_bytes_half;

	return dput_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_get(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uintptr_t rma_request_vaddr,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_GET |
			(dt64 << 16) | (payload_bytes << 48);
	tx_hdr->qw[5] = rma_request_vaddr;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		if (dt64 == (FI_VOID - 1)) {
			OPX_HMEM_COPY_FROM((void *)tx_payload,
					   (const void *)*sbuf, payload_bytes,
					   sbuf_iface, sbuf_device);
		} else {
			OPX_HMEM_ATOMIC_DISPATCH((void *)*sbuf,
						  (void *)tx_payload,
						  payload_bytes,
						  dt64,
						  FI_ATOMIC_WRITE,
						  sbuf_iface, sbuf_device);
		}
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}

	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_rzv(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint32_t opcode,
				const uintptr_t target_byte_counter_vaddr,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | (opcode) | (payload_bytes << 48);
	tx_hdr->qw[5] = target_byte_counter_vaddr;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		OPX_HMEM_COPY_FROM((void *)tx_payload, (const void *)*sbuf,
				   payload_bytes, sbuf_iface, sbuf_device);
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}
	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_packet(struct fi_opx_ep *opx_ep,
				  union fi_opx_hfi1_packet_hdr *tx_hdr,
				  union fi_opx_hfi1_packet_payload *tx_payload,
				  struct iovec *iov,
				  const uint32_t opcode,
				  const int64_t psn_orig,
				  const uint16_t lrh_dws,
				  const uint64_t op64,
				  const uint64_t dt64,
				  const uint64_t lrh_dlid,
				  const uint64_t bth_rx,
				  const size_t payload_bytes,
				  const uint64_t key,
				  const uint64_t fetch_vaddr,
				  const uintptr_t target_byte_counter_vaddr,
				  const uintptr_t rma_request_vaddr,
				  uint64_t bytes_sent,
				  uint8_t **sbuf,
				  const enum fi_hmem_iface sbuf_iface,
				  const uint64_t sbuf_device,
				  uint8_t **cbuf,
				  const enum fi_hmem_iface cbuf_iface,
				  const uint64_t cbuf_device,
				  uintptr_t *rbuf)
{
	uint64_t psn = (uint64_t) htonl((uint32_t)psn_orig);

	tx_hdr->qw[0] = opx_ep->rx->tx.dput.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	tx_hdr->qw[1] = opx_ep->rx->tx.dput.hdr.qw[1] | bth_rx;
	tx_hdr->qw[2] = opx_ep->rx->tx.dput.hdr.qw[2] | psn;
	tx_hdr->qw[3] = opx_ep->rx->tx.dput.hdr.qw[3];

	switch(opcode) {
	case FI_OPX_HFI_DPUT_OPCODE_RZV:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_TID:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG:
		return opx_hfi1_dput_write_header_and_payload_rzv(
				opx_ep, tx_hdr, tx_payload, iov,
				op64, dt64, payload_bytes, opcode,
				target_byte_counter_vaddr, sbuf,
				sbuf_iface, sbuf_device, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_GET:
		return opx_hfi1_dput_write_header_and_payload_get(
				opx_ep, tx_hdr, tx_payload, iov,
				dt64, payload_bytes, rma_request_vaddr,
				sbuf, sbuf_iface, sbuf_device, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_PUT:
		return opx_hfi1_dput_write_header_and_payload_put(
				opx_ep, tx_hdr, tx_payload,
				iov, op64, dt64, payload_bytes,
				key, sbuf, sbuf_iface, sbuf_device, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH:
		return opx_hfi1_dput_write_header_and_payload_atomic_fetch(
				opx_ep, tx_hdr, tx_payload, iov, op64, dt64,
				payload_bytes, key, fetch_vaddr,
				rma_request_vaddr, bytes_sent, sbuf,
				sbuf_iface, sbuf_device, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH:
		return opx_hfi1_dput_write_header_and_payload_atomic_compare_fetch(
				opx_ep, tx_hdr, tx_payload, iov, op64, dt64,
				payload_bytes, key, fetch_vaddr,
				rma_request_vaddr, bytes_sent, sbuf, sbuf_iface,
				sbuf_device, cbuf, cbuf_iface, cbuf_device, rbuf);
		break;
	default:
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"Invalid opcode %0X; abort\n", opcode);
		abort();
	}
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				const uint32_t opcode,
				const int64_t psn_orig,
				const uint16_t lrh_dws,
				const uint64_t op64,
				const uint64_t dt64,
				const uint64_t lrh_dlid,
				const uint64_t bth_rx,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				const uintptr_t rma_request_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				const enum fi_hmem_iface sbuf_iface,
				const uint64_t sbuf_device,
				uint8_t **cbuf,
				const enum fi_hmem_iface cbuf_iface,
				const uint64_t cbuf_device,
				uintptr_t *rbuf)
{
	return opx_hfi1_dput_write_packet(opx_ep, tx_hdr, tx_payload, NULL,
					  opcode, psn_orig, lrh_dws, op64,
					  dt64, lrh_dlid, bth_rx, payload_bytes,
					  key, fetch_vaddr, target_byte_counter_vaddr,
					  rma_request_vaddr, bytes_sent,
					  sbuf, sbuf_iface, sbuf_device,
					  cbuf, cbuf_iface, cbuf_device, rbuf);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_iov(struct fi_opx_ep *opx_ep,
					  union fi_opx_hfi1_packet_hdr *tx_hdr,
					  struct iovec *iov,
					  const uint32_t opcode,
					  const uint16_t lrh_dws,
					  const uint64_t op64,
					  const uint64_t dt64,
					  const uint64_t lrh_dlid,
					  const uint64_t bth_rx,
					  const size_t payload_bytes,
					  const uint64_t key,
					  const uint64_t fetch_vaddr,
					  const uintptr_t target_byte_counter_vaddr,
					  const uintptr_t rma_request_vaddr,
					  uint64_t bytes_sent,
					  uint8_t **sbuf,
					  uint8_t **cbuf,
					  uintptr_t *rbuf)
{
	/* When we're just setting the IOV
	 * 1. Use a PSN of 0, because the caller will set that later
	 * 2. The sbuf/cbuf iface and device are not used, so just pass in system/0
	 */
	return opx_hfi1_dput_write_packet(opx_ep, tx_hdr, NULL, iov, opcode, 0,
					  lrh_dws, op64, dt64, lrh_dlid, bth_rx,
					  payload_bytes, key, fetch_vaddr,
					  target_byte_counter_vaddr,
					  rma_request_vaddr, bytes_sent,
					  sbuf, FI_HMEM_SYSTEM, 0ul,
					  cbuf, FI_HMEM_SYSTEM, 0ul,
					  rbuf);
}
#endif
