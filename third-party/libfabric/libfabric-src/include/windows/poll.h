/*
* Copyright (c) 2016 Intel Corporation.  All rights reserved.
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

#pragma once

#include <Winsock2.h>

static inline int poll(struct pollfd *fds, int nfds, int timeout)
{
	/* if none of fds is passed, returns 0 instead of error
	 * to allign Windows and Linux poll system call */
	if (!nfds) {
		Sleep(timeout);
		return 0;
	} else {
		return WSAPoll(fds, nfds, timeout);
	}
}

