/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_LOG_H_
#define _FI_NETDIR_LOG_H_

#include <windows.h>

#include "rdma/providers/fi_log.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern struct fi_provider ofi_nd_prov;

#define ND_LOG_WARN(subsystem, ...) FI_WARN(&ofi_nd_prov, subsystem, __VA_ARGS__)
#define ND_LOG_INFO(subsystem, ...) FI_INFO(&ofi_nd_prov, subsystem, __VA_ARGS__)
#define ND_LOG_DEBUG(subsystem, ...) FI_DBG(&ofi_nd_prov, subsystem, __VA_ARGS__)

#if ENABLE_DEBUG
# define ND_LOG_EVENT_INFO(entry)							\
	ND_LOG_DEBUG(FI_LOG_EP_DATA, "\nflags: req_ack - %d, ack - %d, empty - %d\n"	\
		     "common: state - %d, event - %d\n",				\
		     (entry)->flow_cntrl_flags.req_ack,					\
		     (entry)->flow_cntrl_flags.ack,					\
		     (entry)->flow_cntrl_flags.empty,					\
		     (entry)->state,							\
		     (entry)->event)
#else
# define ND_LOG_EVENT_INFO(entry)
#endif

#define FI_ND_GUID_FORMAT "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"
#define FI_ND_GUID_ARG(guid)					\
	(guid).Data1, (guid).Data2, (guid).Data3,		\
	(guid).Data4[0], (guid).Data4[1], (guid).Data4[2],	\
	(guid).Data4[3], (guid).Data4[4], (guid).Data4[5],	\
	(guid).Data4[6], (guid).Data4[7]

/* ofi_nd_strerror generates string message based on err value (GetLastError)
   returned string is valid till next call of ofi_nd_strerror
*/
static inline char *ofi_nd_strerror(DWORD err, HMODULE module)
{
	static char *message = NULL;
	size_t size;

	/* if message is allocated - free it */
	if (message)
		LocalFree(message);

	size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			      FORMAT_MESSAGE_FROM_SYSTEM |
			      FORMAT_MESSAGE_IGNORE_INSERTS |
			      (module ? FORMAT_MESSAGE_FROM_HMODULE : 0),
			      module, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			      (LPSTR)&message, 0, NULL);

	return size ? message : (char*)"";
}
static inline char *
ofi_nd_get_last_error_str(HRESULT hr, char *errmsg, SIZE_T max_msg_len)
{
	LPVOID lpMsgBuf;
	DWORD dw = (DWORD)hr;
	errno_t rc;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	strcpy_s(errmsg, max_msg_len, "NTStatus: ");
	rc = strncat_s(errmsg, max_msg_len - strlen(errmsg),
		lpMsgBuf, _TRUNCATE);

	LocalFree(lpMsgBuf);

	return errmsg;
}

#define OFI_NDERR(err, str) 	\
	case err:		\
	  str = #err ;		\
	  break

#define ND_FLUSHED 0x10000L	/* undocumented ND error code */
#define ND_DISCONNECTED 0xc000020C 

static char *ofi_nd_error_str(HRESULT hr)
{
	static char lerr[128];
	char *err_str = NULL;

	switch (hr) {
	OFI_NDERR(ND_SUCCESS, err_str);
	OFI_NDERR(ND_FLUSHED, err_str);
	OFI_NDERR(ND_TIMEOUT, err_str);
	OFI_NDERR(ND_PENDING, err_str);
	OFI_NDERR(ND_BUFFER_OVERFLOW, err_str);
	OFI_NDERR(ND_DEVICE_BUSY, err_str);
	OFI_NDERR(ND_NO_MORE_ENTRIES, err_str);
	OFI_NDERR(ND_UNSUCCESSFUL, err_str);
	OFI_NDERR(ND_ACCESS_VIOLATION, err_str);
	OFI_NDERR(ND_INVALID_HANDLE, err_str);
	OFI_NDERR(ND_INVALID_DEVICE_REQUEST, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER, err_str);
	OFI_NDERR(ND_NO_MEMORY, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_MIX, err_str);
	OFI_NDERR(ND_DATA_OVERRUN, err_str);
	OFI_NDERR(ND_SHARING_VIOLATION, err_str);
	OFI_NDERR(ND_INSUFFICIENT_RESOURCES, err_str);
	OFI_NDERR(ND_DEVICE_NOT_READY, err_str);
	OFI_NDERR(ND_IO_TIMEOUT, err_str);
	OFI_NDERR(ND_NOT_SUPPORTED, err_str);
	OFI_NDERR(ND_INTERNAL_ERROR, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_1, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_2, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_3, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_4, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_5, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_6, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_7, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_8, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_9, err_str);
	OFI_NDERR(ND_INVALID_PARAMETER_10, err_str);
	OFI_NDERR(ND_CANCELED, err_str);
	OFI_NDERR(ND_REMOTE_ERROR, err_str);
	OFI_NDERR(ND_INVALID_ADDRESS, err_str);
	OFI_NDERR(ND_INVALID_DEVICE_STATE, err_str);
	OFI_NDERR(ND_INVALID_BUFFER_SIZE, err_str);
	OFI_NDERR(ND_TOO_MANY_ADDRESSES, err_str);
	OFI_NDERR(ND_ADDRESS_ALREADY_EXISTS, err_str);
	OFI_NDERR(ND_CONNECTION_REFUSED, err_str);
	OFI_NDERR(ND_CONNECTION_INVALID, err_str);
	OFI_NDERR(ND_CONNECTION_ACTIVE, err_str);
	OFI_NDERR(ND_HOST_UNREACHABLE, err_str);
	OFI_NDERR(ND_CONNECTION_ABORTED, err_str);
	OFI_NDERR(ND_DEVICE_REMOVED, err_str);
	OFI_NDERR(ND_DISCONNECTED, err_str);
	default:
		err_str = ofi_nd_get_last_error_str(hr, lerr, sizeof(lerr));
		if (err_str == NULL) {
			_snprintf(lerr, sizeof(lerr), "Unknown ND error %#08ld", hr);
			err_str = lerr;
		}
		break;
	}
	return err_str;
}
#undef NDERR

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_LOG_H_ */

