/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021 Cornelis Networks.

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
  Copyright(c) 2021 Cornelis Networks.

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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef OPA_UDEBUG_H
#define OPA_UDEBUG_H

#include <stdio.h>
#include "rdma/opx/fi_opx.h"

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} else { \
		fprintf(stderr, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_ERROR(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} else { \
		fprintf(stderr, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_INFO(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_INFO(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_DBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_PDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_EPDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_PRDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_ENVDBG(lev, fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_VDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_MMDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_CCADBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#endif /* OPA_UDEBUG_H */
