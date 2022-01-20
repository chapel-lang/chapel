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

#include "psm_user.h"
#include "psm2_hal.h"

#if PSMI_HAL_INST_CNT > 1
#define PSMI_HAL_CAT_INL_SYM(KERNEL) hfp_gen1_ ## KERNEL
#include "psm2_hal_inline_t.h"
#include "psm_hal_inline_i.h"
#endif

/* define the singleton that implements hal for gen1 */
static hfp_gen1_t psm_gen1_hi = {
	/* start of public psmi_hal_instance_t data */
	.phi = {
		.type					  = PSM_HAL_INSTANCE_GEN1,
		.description				  = "PSM3 HAL instance for GEN1"
#ifdef PSM_CUDA
								" (cuda)"
#endif
									,
		.hfi_name				  = "hfi1",
		.hfi_sys_class_path			  = "/sys/class/infiniband/hfi1",
		.params					  = {0},

		/* The following methods are alphabetized */
#if PSMI_HAL_INST_CNT > 1

		.hfp_close_context			  = hfp_gen1_close_context,
		.hfp_context_open			  = hfp_gen1_context_open,
		.hfp_context_initstats			  = hfp_gen1_context_initstats,


		.hfp_finalize_				  = hfp_gen1_finalize_,


		.hfp_get_jkey				  = hfp_gen1_get_jkey,


		.hfp_get_node_id			  = hfp_gen1_get_node_id,



		.hfp_get_port_lid			  = hfp_gen1_get_port_lid,


		.hfp_get_port_rate			  = hfp_gen1_get_port_rate,


		.hfp_spio_process_events		  = hfp_gen1_spio_process_events,
		.hfp_spio_transfer_frame		  = hfp_gen1_spio_transfer_frame,


#endif // PSMI_HAL_INST_CNT > 1
		.hfp_get_port_subnet		  = hfp_gen1_get_port_subnet,
		.hfp_get_default_pkey			  = hfp_gen1_get_default_pkey,
		.hfp_get_num_contexts			  = hfp_gen1_get_num_contexts,
		.hfp_get_num_free_contexts		  = hfp_gen1_get_num_free_contexts,
		.hfp_get_num_units			  = hfp_gen1_get_num_units,
		.hfp_get_num_ports			  = hfp_gen1_get_num_ports,
		.hfp_get_port_active			  = hfp_gen1_get_port_active,
		.hfp_get_unit_active			  = hfp_gen1_get_unit_active,
		.hfp_initialize				  = hfp_gen1_initialize,
	},
};

/* __psmi_hal_gen1_constructor */
static void __attribute__ ((constructor)) __psmi_hal_gen1_constructor(void)
{
	psmi_hal_register_instance((psmi_hal_instance_t*)&psm_gen1_hi);
}
