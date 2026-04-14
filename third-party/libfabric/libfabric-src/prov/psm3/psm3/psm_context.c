/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include "psm_user.h"
#include "psm2_hal.h"

static psm2_error_t
psm3_ep_verify_pkey(psm2_ep_t ep, uint16_t pkey, uint16_t *opkey, uint16_t* oindex);

// enable or disable interrupts for urgent PSM protocol packets
psm2_error_t psm3_context_interrupt_set(psm2_ep_t ep, int enable)
{
	int poll_type;
	int ret;

	if (!enable == !psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED))
		return PSM2_OK;

	if (enable)
		poll_type = PSMI_HAL_POLL_TYPE_URGENT;
	else
		poll_type = PSMI_HAL_POLL_TYPE_NONE;

	ret = psmi_hal_poll_type(poll_type, ep);

	if (ret != 0) return PSM2_EP_NO_RESOURCES;

	if (enable)
		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
	else
		psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
	return PSM2_OK;
}

int psm3_context_interrupt_isenabled(psm2_ep_t ep)
{
	return psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
}

static int psmi_hash_addr_index(long unit, long port, long addr_index)
{
	/* if the user did not set addr_index, then use a hash */
	if (addr_index == PSM3_ADDR_INDEX_ANY) {
		addr_index = (psm3_get_mylocalrank() + psm3_opened_endpoint_count) % psm3_addr_per_nic;
		if (psmi_hal_get_port_lid(unit, port?port:1, addr_index) <= 0)
			return 0;
	}
	return addr_index;
}

// Open a single NIC.
// if unit_param is PSM3_NIC_ANY, the chosen PSM3_NIC_SELECTION_ALG will be
// used to pick a single active NIC
psm2_error_t
psm3_context_open(const psm2_ep_t ep, long unit_param, long port, long addr_index,
		  psm2_uuid_t const job_key, uint16_t network_pkey,
		  int64_t timeout_ns)
{
	long open_timeout = 0;
	psm2_error_t err = PSM2_OK;
	int nunits = psmi_hal_get_num_units();
	union psmi_envvar_val env_rcvthread;
	static int norcvthread;	/* only for first rail */

	if (nunits <= 0)
	{
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 no nic units are available");
		goto ret;
	}

	if (timeout_ns > 0)
		open_timeout = (long)(timeout_ns / NSEC_PER_MSEC);

	if (unit_param == PSM3_NIC_ANY) {
		/* user did not set PSM3_NIC and not PSM3_MULTIRAIL */
		unit_param = psm3_autoselect_one(addr_index, nunits, job_key);
		if (unit_param < 0) {
			err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 no nic units are active");
			goto ret;
		}
	} else {
		_HFI_DBG("Caller selected NIC %ld.\n", unit_param);
		psmi_assert(unit_param >= 0);	// caller checked valid
	}

	/* open this unit. */
	if (psmi_hal_get_unit_active(unit_param) <= 0
	    || psmi_hal_context_open(unit_param, port,
			psmi_hash_addr_index(unit_param, port, addr_index),
			open_timeout,
			ep, job_key, HAL_CONTEXT_OPEN_RETRY_MAX)) {
		// in modes where we refcount NIC use,
		// psm3_autoselect_one refcount'ed it, so we dec refcount here
		psm3_dec_nic_refcount(unit_param);
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				"PSM3 can't open nic unit: %ld",unit_param);
		goto bail;
	}
	// HAL context_open has initialized:
	// ep->unit_id, ep->portnum, ep->addr_index,
	// ep->dev_name, ep->subnet, ep->addr, ep->gid, ep->wiremode,
	// ep->epid and
	// HAL specific ep fields (context, verbs_ep or sockets_ep)
	psmi_assert_always(! psm3_epid_zero_internal(ep->epid));

	_HFI_VDBG("hal_context_open() passed.\n");

	psmi_assert_always(PSMI_IPS_ADDR_FMT_IS_VALID(ep->addr.fmt));
	psmi_assert_always(ep->addr.fmt == ep->subnet.fmt);
	psmi_assert_always(ep->addr.prefix_len == ep->subnet.prefix_len);
	ep->addr_fmt = ep->addr.fmt;

	_HFI_DBG("[%d]use unit %d port %d addr %d\n", getpid(), ep->unit_id, 1, ep->addr_index);

	/* device is opened, make sure we can find a valid desirable pkey */
	if ((err =
	     psm3_ep_verify_pkey(ep, network_pkey,
			&ep->network_pkey, &ep->network_pkey_index)) != PSM2_OK)
		goto close;

	/* See if we want to activate support for receive thread */
	psm3_getenv("PSM3_RCVTHREAD",
		    "Enable Recv thread (0 disables thread)",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			// default to 0 for all but 1st rail
		    (union psmi_envvar_val)(norcvthread++ ? 0 :
					    PSMI_RCVTHREAD_FLAGS),
		    &env_rcvthread);

	/* If enabled, use the polling capability to implement a receive
	 * interrupt thread that can handle urg packets */
	if (env_rcvthread.e_uint) {
		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD);
#ifdef PSMI_PLOCK_IS_NOLOCK
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "#define PSMI_PLOCK_IS_NOLOCK not functional yet "
				  "with RCVTHREAD on");
#endif
	}
	_HFI_PRDBG("Opened unit %ld port %ld: EPID=%s %s\n", unit_param, port,
		psm3_epid_fmt_internal(ep->epid, 0), psm3_epid_fmt_addr(ep->epid, 1));

	goto ret;

close:
	psmi_hal_close_context(ep);
	psm3_dec_nic_refcount(ep->unit_id);
bail:
	_HFI_PRDBG("open failed: unit_id: %ld, err: %d (%s)\n", unit_param, err, strerror(errno));
ret:

	_HFI_VDBG("psm3_context_open() return %d\n", err);
	return err;
}

psm2_error_t psm3_context_close(psm2_ep_t ep)
{
	psmi_hal_close_context(ep);
	psm3_dec_nic_refcount(ep->unit_id);

	return PSM2_OK;
}

// up to 4 digits per CPU number, plus a coma or dash
#define MAX_CPU_AFFINITY_STRING (CPU_SETSIZE * 5)

static inline char * _dump_cpu_affinity(char *buf, size_t buf_size, cpu_set_t * cpuset) {
	int i;
	char tmp[25]; //%d, = 10+','+\0 or %d-%d, = 10 + '-' + 10 + ',' + '\0' = 23
	int first = -1, last = -1;
	int len = 0;

	*buf = '\0';
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

			snprintf(&buf[len], buf_size-len,"%s", tmp);
			len = strlen(buf);
		}
	}

	if (first != -1) {
		if (first == last) {
			snprintf(tmp, sizeof(tmp), "%d,", first);
		} else {
			snprintf(tmp, sizeof(tmp), "%d-%d,", first, last);
		}
		snprintf(&buf[len], buf_size-len,"%s", tmp);
		len = strlen(buf);
	}
	if (len)
		buf[len-1] = '\0';	// elimate trailing coma

	return buf;
}

// called by HAL context_open to set CPU affinity narrower consistent with
// NIC NUMA location
// Intel MPI sets PSM3_NO_CPUAFFINITY to disable this function
// Suspect this is not effective or has bugs.  For Omni-Path the NIC
// driver set affinity before this was called, and this was thus likely a noop.
// This is a noop if:
//     - if NIC is not NUMA local to any of CPUs in existing affinity
//     - if existing affinity selects more cores than those local to NIC
//		even if that set incompletely overlaps the NIC local core set
//		suspect this is a bug and test should be opposity or just test
//		for overlap.
// if NIC is NUMA local to CPU, and NIC core list is larger than existing
// affinity, will limit scope of affinity to cores NUMA local to NIC
//  - does not consider the full set of selected NICs when multirail enabled
//  - may only provide value if CPU set from caller is small but > 1 CPU NUMA
//    domain in which case this will reduce it to a single CPU NUMA domain
//    matching the NIC's NUMA location.
//
// By default this is enabled, but two undocumented variables
// PSM3_FORCE_CPUAFFINITY and PSM3_NO_CPUAFFINITY can control this
// as well as the ep_open skip_affinity flag.
//
// May be better if we analyzed NIC NUMA location and various other
// process and thread locations when NIC NUMA is a subset of CPU affinity
// and guide a good choice for CPU affinity, but that would require
// intra-node process coordination to avoid duplicate CPU selections
//
// TBD for GPU affinity this may not make sense.  Also PSM3 can't force a GPU
// selection for an app.
//
// TBD when PSM3 is using multiple NICs (PSM3_MULTIRAIL > 0) this should
// be enhanced to attempt to select a CPU based on location of all NICs being
// used, not just a single NIC.
int
psm3_context_set_affinity(psm2_ep_t ep, int unit)
{
	pthread_t mythread = pthread_self();
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
	if (s != 0) {
		psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
			"Can't get CPU affinity: %s\n", strerror(errno));
		goto bail;
	}

	if (_HFI_DBG_ON) {
		char cpu_buf[MAX_CPU_AFFINITY_STRING] = {0};
		_HFI_DBG_ALWAYS( "CPU affinity Before set: %s\n",
				_dump_cpu_affinity(cpu_buf, MAX_CPU_AFFINITY_STRING, &cpuset));
	}

	/*
	 * PSM3_FORCE_CPUAFFINITY forces affinity setting
	 * Otherwise, skip affinity if:
	 * 1. User explicitly sets PSM3_NO_AFFINITY in environment.
	 * 2. User doesn't set affinity in environment and PSM is opened with
	 *    option affinity skip.
	 */
	//if (psm3_env_get("PSM3_FORCE_CPUAFFINITY") ||
	//	!(psm3_env_get("PSM3_NO_CPUAFFINITY") || ep->skip_affinity))
	if (psm3_env_psm_sets_cpuaffinity(ep->skip_affinity))
	{
		cpu_set_t nic_cpuset;
		cpu_set_t andcpuset;

		if (psm3_sysfs_get_unit_cpumask(unit, &nic_cpuset)) {
			//_HFI_INFO( "Failed to get %s (unit %d) cpu set\n", ep->dev_name, unit);
			////err = -PSM_HAL_ERROR_GENERAL_ERROR;
			//goto bail;
			goto skip_affinity;
		}

		int cpu_count = CPU_COUNT(&cpuset);
		int nic_count = CPU_COUNT(&nic_cpuset);
		if (cpu_count > nic_count) {
			andcpuset = cpuset;
		} else {
			CPU_AND(&andcpuset, &cpuset, &nic_cpuset);
		}
		int cpu_and_count = CPU_COUNT(&andcpuset);

		if (cpu_and_count > 0 && pthread_setaffinity_np(mythread, sizeof(andcpuset), &andcpuset)) {
			_HFI_ERROR( "Failed to set %s (unit %d) cpu set: %s\n", ep->dev_name,  unit, strerror(errno));
			//err = -PSM_HAL_ERROR_GENERAL_ERROR;
			goto bail;
		} else if (cpu_and_count == 0 && _HFI_DBG_ON) {
			char buf1[MAX_CPU_AFFINITY_STRING] = {0};
			char buf2[MAX_CPU_AFFINITY_STRING] = {0};
			_HFI_DBG_ALWAYS( "CPU affinity not set, NIC selected is not on the same socket as thread (\"%s\" & \"%s\" == 0).\n",
				_dump_cpu_affinity(buf1, MAX_CPU_AFFINITY_STRING, &nic_cpuset),
				_dump_cpu_affinity(buf2, MAX_CPU_AFFINITY_STRING, &cpuset));
		}
	}
skip_affinity:
	if (_HFI_DBG_ON) {
		CPU_ZERO(&cpuset);
		int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
		if (s != 0) {
			psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				"Can't get CPU affinity: %s\n", strerror(errno));
			goto bail;
		}
		char cpu_buf[MAX_CPU_AFFINITY_STRING] = {0};
		_HFI_DBG_ALWAYS( "CPU affinity After set: %s\n",
				_dump_cpu_affinity(cpu_buf, MAX_CPU_AFFINITY_STRING, &cpuset));
	}
	return 0;

bail:
	return -1;
}

static psm2_error_t
psm3_ep_verify_pkey(psm2_ep_t ep, uint16_t pkey, uint16_t *opkey, uint16_t* oindex)
{
	int i, ret;
	psm2_error_t err;

	_HFI_PRDBG("looking for pkey 0x%x\n", pkey);
	for (i = 0; i < 16; i++) {
		ret = psmi_hal_get_port_index2pkey(ep, i);
		if (ret < 0) {
			err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"Can't get a valid pkey value from pkey table on %s port %u\n", ep->dev_name, ep->portnum);
			return err;
		}
		// pkey == 0 means just get slot 0
		if (! pkey && ! i)
			break;
		if ((pkey & 0x7fff) == (uint16_t)(ret & 0x7fff)) {
			break;
		}
	}

	/* if pkey does not match */
	if (i == 16) {
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Wrong pkey 0x%x on %s port %u, please use PSM3_PKEY to specify a valid pkey\n",
					pkey, ep->dev_name, ep->portnum);
		return err;
	}

	if (((uint16_t)ret & 0x8000) == 0) {
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Limited Member pkey 0x%x on %s port %u, please use PSM3_PKEY to specify a valid pkey\n",
					(uint16_t)ret, ep->dev_name, ep->portnum);
		return err;
	}

	/* return the final pkey */
	*opkey = (uint16_t)ret;
	*oindex = (uint16_t)i;

	return PSM2_OK;
}
