/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

#include <stdlib.h>
#include <unistd.h>
#include "psmi_wrappers.h"
#include <sys/ioctl.h>

/* The following indirection wrappers for external functions
 * are only created if this is a mocking tests build
 */
#ifdef PSM2_MOCK_TESTING

void MOCKABLE(psmi_exit)(int status)
{
	exit(status);
}
MOCK_DEF_EPILOGUE(psmi_exit);

ssize_t MOCKABLE(psmi_write)(int fd, const void *buf, size_t count)
{
	return write(fd, buf, count);
}
MOCK_DEF_EPILOGUE(psmi_write);

int MOCKABLE(psmi_ioctl)(int fd, unsigned int cmd, unsigned long arg)
{
	return ioctl(fd, cmd, arg);
}
MOCK_DEF_EPILOGUE(psmi_ioctl);

int MOCKABLE(psmi_sigaction)(int signum, const struct sigaction *act, struct sigaction *oldact)
{
	return sigaction(signum, act, oldact);
}
MOCK_DEF_EPILOGUE(psmi_sigaction);

void MOCKABLE(psmi_rmb)(void)
{
	return ips_rmb();
}
MOCK_DEF_EPILOGUE(psmi_rmb);

#endif /* def PSM2_MOCK_TESTING */
