/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2017 Intel Corporation.

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

#include "psm_hal_gen1.h"

extern size_t arrsz[MAPSIZE_MAX];

static inline struct _hfp_gen1 *get_psm_gen1_hi(void)
{
	return (struct _hfp_gen1*) psmi_hal_current_hal_instance;
}

/* hfp_gen1_initialize */
static PSMI_HAL_INLINE int hfp_gen1_initialize(psmi_hal_instance_t *phi)
{
	return 0;
}

/* hfp_gen1_finalize_ */
static PSMI_HAL_INLINE int hfp_gen1_finalize_(void)
{
	return 0;
}

/* hfp_gen1_get_num_units */
static PSMI_HAL_INLINE int hfp_gen1_get_num_units(void)
{
	return hfi_get_num_units();
}

/* hfp_gen1_get_num_ports */
static PSMI_HAL_INLINE int hfp_gen1_get_num_ports(void)
{
	return HFI_NUM_PORTS_GEN1;
}

/* hfp_gen1_get_unit_active */
static PSMI_HAL_INLINE int hfp_gen1_get_unit_active(int unit)
{
	return hfi_get_unit_active(unit);
}

/* hfp_gen1_get_port_active */
static PSMI_HAL_INLINE int hfp_gen1_get_port_active(int unit, int port)
{
	return hfi_get_port_active(unit, port);
}

// Most of these defines are in opa_service_gen1.c, but there are no
// include files common to that file and this one
#define HFI_UD_NUM_CTXTS   1024

/* hfp_gen1_get_num_contexts */
static PSMI_HAL_INLINE int hfp_gen1_get_num_contexts(int unit)
{
	return HFI_UD_NUM_CTXTS;
}

// Most of these defines are in opa_service_gen1.c, but there are no
// include files common to that file and this one
#define HFI_UD_NUM_FREE_CTXTS   1024

/* hfp_gen1_get_num_free_contexts */
static PSMI_HAL_INLINE int hfp_gen1_get_num_free_contexts(int unit)
{
	return HFI_UD_NUM_FREE_CTXTS;
}



/* hfp_gen1_close_context */
static PSMI_HAL_INLINE int hfp_gen1_close_context(psmi_hal_hw_context *ctxtp)
{
	hfp_gen1_pc_private *psm_hw_ctxt;

	if (!ctxtp || !*ctxtp)
		return PSM_HAL_ERROR_OK;

	psm_hw_ctxt = (hfp_gen1_pc_private *)(*ctxtp);
	psmi_free(psm_hw_ctxt);

	return PSM_HAL_ERROR_OK;
}

/* Moved from psm_context.c */






static inline char * _dump_cpu_affinity(char *buf, size_t buf_size, cpu_set_t * cpuset) {
	int i;
	int isfirst = 1;
	char tmp[25]; //%d = 10 :: 10 + '-' + 10 + ',' + '\0' = 23
	int first = -1, last = -1;

	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, cpuset)) {
			if (first == -1) {
				first = last = i;
			} else if ((last+1) == i) {
				last = i;
			}
		} else if (first != -1) {
			if (first == last) {
				snprintf(tmp, sizeof(tmp), "%d,", first);
			} else {
				snprintf(tmp, sizeof(tmp), "%d-%d,", first, last);
			}
			first = last = -1;

			if (isfirst) {
				strncpy(buf, tmp, buf_size-1);
				isfirst=0;
			} else {
				strncat(buf, tmp, buf_size-1);
			}
			buf[buf_size-1] = '\0';
		}
	}

	if (first != -1) {
		if (first == last) {
			snprintf(tmp, sizeof(tmp), "%d,", first);
		} else {
			snprintf(tmp, sizeof(tmp), "%d-%d,", first, last);
		}
		if (isfirst) {
			strncpy(buf, tmp, buf_size-1);
		} else {
			strncat(buf, tmp, buf_size-1);
		}
		buf[buf_size-1] = '\0';
	}
	char *comma = strrchr(buf, ',');
	if (comma) comma[0] = '\0';

	return buf;
} //	pthread_getaffinity_np

/* hfp_gen1_context_open */
static PSMI_HAL_INLINE int hfp_gen1_context_open(int unit,
				 int port,
				 uint64_t open_timeout,
				 psm2_ep_t ep,
				 psm2_uuid_t const job_key,
				 psmi_context_t *psm_ctxt,
				 uint32_t cap_mask,
				 unsigned retryCnt)
{
	psm2_error_t err = PSM2_OK;
	hfp_gen1_pc_private *pc_private = psmi_malloc(ep, UNDEFINED, sizeof(hfp_gen1_pc_private));

	if_pf (!pc_private) {
		//err = -PSM_HAL_ERROR_CANNOT_OPEN_CONTEXT;
		goto bail;
	}

	memset(pc_private, 0, sizeof(hfp_gen1_pc_private));


	// open verbs 1st so psmi_context_open can get pkey, lid, etc
	if ((err = __psm2_ep_open_verbs(ep, unit, port, job_key)) != PSM2_OK) {
		_HFI_ERROR( "Unable to initialize verbs\n");
		err = -PSM_HAL_ERROR_CANNOT_OPEN_CONTEXT;
		goto bail;
	}

	pthread_t mythread = pthread_self();
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
	if (s != 0) {
		psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
			"Can't get CPU affinity: %s\n", strerror(errno));
		goto bail;
	}

	if (_HFI_DBG_ON) {
		char cpu_buf[128] = {0};
		_HFI_DBG( "CPU affinity Before set: %s\n", _dump_cpu_affinity(cpu_buf, 128, &cpuset));
	}

	if (getenv("PSM3_FORCE_CPUAFFINITY") ||
		!(getenv("PSM3_NO_CPUAFFINITY") || ep->skip_affinity))
	{
		cpu_set_t mycpuset, andcpuset;

		if (hfi_get_unit_cpumask(unit, &mycpuset)) {
			_HFI_ERROR( "Failed to get unit %d's cpu set\n", unit);
			//err = -PSM_HAL_ERROR_GENERAL_ERROR;
			goto bail;
		}

		int cpu_count = CPU_COUNT(&cpuset);
		int my_count = CPU_COUNT(&mycpuset);
		if (cpu_count > my_count) {
			andcpuset = cpuset;
		} else {
			CPU_AND(&andcpuset, &cpuset, &mycpuset);
		}
		int cpu_and_count = CPU_COUNT(&andcpuset);

		if (cpu_and_count > 0 && pthread_setaffinity_np(mythread, sizeof(andcpuset), &andcpuset)) {
			_HFI_ERROR( "Failed to set unit %d's cpu set: %s\n", unit, strerror(errno));
			//err = -PSM_HAL_ERROR_GENERAL_ERROR;
			goto bail;
		} else if (cpu_and_count == 0 && _HFI_DBG_ON) {
			char buf1[128] = {0};
			char buf2[128] = {0};
			_HFI_DBG( "CPU affinity not set, NIC selected is not on the same socket as thread (\"%s\" & \"%s\" == 0).\n",
				_dump_cpu_affinity(buf1, 128, &mycpuset), _dump_cpu_affinity(buf2, 128, &cpuset));
		}
	}
	if (_HFI_DBG_ON) {
		CPU_ZERO(&cpuset);
		int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
		if (s != 0) {
			psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				"Can't get CPU affinity: %s\n", strerror(errno));
			goto bail;
		}
		char cpu_buf[128] = {0};
		_HFI_DBG( "CPU affinity After set: %s\n", _dump_cpu_affinity(cpu_buf, 128, &cpuset));
	}

// TBD - inside hfi_userinit_internal we would find CPU
// which HFI is closest to and set affinity.  Need a way to do that for UD
// we would also wash jkey through driver and stash it in _hfi_ctrl
// but because we disable this we won't have an _hfi_ctrl structure

		psm_ctxt->psm_hw_ctxt = pc_private;
	return PSM_HAL_ERROR_OK;

bail:
	if (pc_private) {
		psmi_free(pc_private);
	}

	return -PSM_HAL_ERROR_GENERAL_ERROR;
}





static PSMI_HAL_INLINE int hfp_gen1_get_port_rate(int unit, int port)
{
	return hfi_get_port_rate(unit, port);
}





static PSMI_HAL_INLINE int hfp_gen1_get_port_lid(int unit, int port)
{
	return hfi_get_port_lid(unit, port);
}

static PSMI_HAL_INLINE int hfp_gen1_get_port_subnet(int unit, int port,
	uint64_t *subnet, uint64_t *addr, uint32_t *ip_addr, uint32_t *netmask,
	int *idx, uint64_t *hi, uint64_t *lo)
{
	return hfi_get_port_subnet(unit, port, subnet, addr, ip_addr, netmask,
								idx, hi, lo);
}






















































static PSMI_HAL_INLINE int hfp_gen1_get_default_pkey(void)
{
	return HFI_DEFAULT_P_KEY;
}

#include "psm_hal_gen1_spio.c"



static PSMI_HAL_INLINE int hfp_gen1_spio_transfer_frame(struct ips_proto *proto,
					struct ips_flow *flow, struct ips_scb *scb,
					uint32_t *payload, uint32_t length,
					uint32_t isCtrlMsg, uint32_t cksum_valid,
					uint32_t cksum, psmi_hal_hw_context ctxt
#ifdef PSM_CUDA
				, uint32_t is_cuda_payload
#endif
	)
{
	return ips_spio_transfer_frame(proto, flow, scb,
					 payload, length, isCtrlMsg,
					 cksum_valid, cksum
#ifdef PSM_CUDA
				, is_cuda_payload
#endif
	);
}

static PSMI_HAL_INLINE int hfp_gen1_spio_process_events(const struct ptl *ptl)
{
	return ips_spio_process_events(ptl);
}

static PSMI_HAL_INLINE int hfp_gen1_get_node_id(int unit, int *nodep)
{
	int64_t node_id = hfi_sysfs_unit_read_node_s64(unit);
	*nodep = (int)node_id;
	if (node_id != -1)
		return PSM_HAL_ERROR_OK;
	else
		return -PSM_HAL_ERROR_GENERAL_ERROR;
}




static PSMI_HAL_INLINE int      hfp_gen1_get_jkey(psmi_hal_hw_context ctxt)
{
	return 0;	// TBD - washed through driver - see HED-542
}














