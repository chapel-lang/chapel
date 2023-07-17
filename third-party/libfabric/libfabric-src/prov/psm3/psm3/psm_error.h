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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */
#include "psm2_mock_testing.h"

#ifndef _PSMI_IN_USER_H
#error psm_error.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_ERROR_H
#define _PSMI_ERROR_H

#define PSMI_EP_NONE		    (NULL)
#define PSMI_EP_NORETURN	    ((psm2_ep_t) -2)
#define PSMI_EP_LOGEVENT	    ((psm2_ep_t) -3)

extern psm2_ep_errhandler_t psm3_errhandler_global;

//
// psm3_handle_error has a 1st argument which controls how it behaves.
// PSMI_EP_NO_RETURN – unconditionally outputs message and exits or aborts
//		process.
// other values – behavior is controlled by how psm2 error handler has been set
//		via PSM API.  OFI/psm3 provider disables error handler so these will
//		be silent
//
// to have PSM stop immediately with a message. use
//		psm3_handle_error(PSMI_EP_NO_RETURN
// all other uses of psm3_handle_error are under the control of the middleware
// or OFI provider.  The OFI provider turns all of them off.
//
// To have a a message be unconditionally output for all builds, regardless of
// env variables, use _HFI_ERROR or _HFI_UNIT_ERROR
// All other logging macros are under the control of the user via env variables
// and build options can disable them
//
// Other logging calls are only enabled if _HFI_DEBUGGING is defined,
// in which case _HFI_INFO is also enabled by default (but env can disable it).
// All others controlled by env variable.
//
// Currently utils_debug.h always defines _HFI_DEBUGGING 
// so logging is presently enabled in all builds.  At some point
// may want to explore a performance optimization and disable logging macros
// for lower level debug messages in non-debug builds.

psm2_error_t MOCKABLE(psm3_handle_error)(psm2_ep_t ep, psm2_error_t error,
			      const char *buf, ...)
			      __attribute__((format(printf, 3, 4)));
MOCK_DCL_EPILOGUE(psm3_handle_error);

psm2_error_t psm3_error_cmp(psm2_error_t errA, psm2_error_t errB);
int psm3_error_syslog_level(psm2_error_t error);

#endif /* _PSMI_ERROR_H */
