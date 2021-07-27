/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2018 Intel Corporation.

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

  Copyright(c) 2018 Intel Corporation.

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
#ifdef PSM_CUDA
#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_gdrcpy.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "ptl_ips/ips_tid.h"
#include "ptl_ips/ips_expected_proto.h"
#include "opa_user_gen1.h"

static int gdr_fd;

int get_gdr_fd(){
	return gdr_fd;
}

#define GPU_PAGE_OFFSET_MASK (PSMI_GPU_PAGESIZE -1)
#define GPU_PAGE_MASK ~GPU_PAGE_OFFSET_MASK





void *
gdr_convert_gpu_to_host_addr(int gdr_fd, unsigned long buf,
							 size_t size, int flags,
							 struct ips_proto* proto)
{
	void *host_addr_buf;

	uintptr_t pageaddr = buf & GPU_PAGE_MASK;
// TBD - is this comment correct?  Callers may be calling for a whole
// app buffer, especially when RECV RDMA is disabled
	/* As size is guarenteed to be in the range of 0-8kB
	 * there is a guarentee that buf+size-1 does not overflow
	 * 64 bits.
	 */
	uint32_t pagelen = (uint32_t) (PSMI_GPU_PAGESIZE +
					   ((buf + size - 1) & GPU_PAGE_MASK) -
					   pageaddr);

	_HFI_VDBG("buf=%p size=%zu pageaddr=%p pagelen=%u flags=0x%x proto=%p\n",
		(void *)buf, size, (void *)pageaddr, pagelen, flags, proto);
#ifdef RNDV_MOD
	host_addr_buf = __psm2_rv_pin_and_mmap(proto->ep->verbs_ep.rv, pageaddr, pagelen);
	if (! host_addr_buf) {
		if (errno == ENOMEM || errno == EINVAL) {
			/* Fatal error */
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "Unable to PIN GPU pages(Out of BAR1 space) (errno: %d)\n", errno);
			return NULL;
		} else {
			/* Fatal error */
			psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
							  "PIN/MMAP ioctl failed errno %d\n",
							  errno);
			return NULL;
		}
	}
#else
	psmi_assert_always(0);	// unimplemented, should not get here
	host_addr_buf = NULL;
#endif /* RNDV_MOD */
	return host_addr_buf + (buf & GPU_PAGE_OFFSET_MASK);
}

// keep this symmetrical with other functions, even though gdr_fd not used
int
gdr_unmap_gpu_host_addr(int gdr_fd, const void *buf,
							 size_t size, struct ips_proto* proto)
{
#ifdef RNDV_MOD
	// TBD - will we need to round size up to pagelen?
	if (0 != __psm2_rv_munmap_and_unpin(proto->ep->verbs_ep.rv, buf, size)) {
		/* Fatal error */
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
						  "UNMMAP/UNPIN ioctl failed errno %d\n",
						  errno);
		return -1;
	}
	return 0;
#else
	psmi_assert_always(0);	// unimplemented, should not get here
	errno = EINVAL;
	return -1;
#endif
}


void hfi_gdr_open(){
	return;
}

void hfi_gdr_close()
{
}

#endif
