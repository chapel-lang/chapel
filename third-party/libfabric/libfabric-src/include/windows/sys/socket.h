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

#ifndef _FI_WIN_SYS_SOCKET_H_
#define _FI_WIN_SYS_SOCKET_H_

#include "ofi_osd.h"
#include "rdma/fabric.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct msghdr {
	void * msg_name; /* optional address */
	socklen_t msg_namelen; /* size of address */
	struct iovec * msg_iov; /* scatter/gather array */
	size_t msg_iovlen; /* # elements in msg_iov */
	void * msg_control; /* ancillary data, see below */
	socklen_t msg_controllen; /* ancillary data buffer len */
	int msg_flags; /* flags on received message */
};

ssize_t sendmsg(SOCKET sd, struct msghdr *msg, int flags);
ssize_t recvmsg(SOCKET sd, struct msghdr *msg, int flags);

#ifdef __cplusplus
extern }
#endif /* __cplusplus */

#endif /* _FI_WIN_SYS_SOCKET_H_ */
