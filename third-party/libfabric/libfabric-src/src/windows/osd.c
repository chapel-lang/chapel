/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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

#include "config.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <ifaddrs.h>

#include "ofi.h"
#include "ofi_osd.h"
#include "ofi_file.h"
#include "ofi_list.h"
#include "ofi_util.h"
#include "rdma/providers/fi_log.h"

extern struct ofi_common_locks common_locks;
static INIT_ONCE ofi_init_once = INIT_ONCE_STATIC_INIT;

static char ofi_shm_prefix[] = "Local\\";

void fi_fini(void);

int socketpair(int af, int type, int protocol, int socks[2])
{
	struct sockaddr_in in_addr;
	SOCKET lsock;
	int len = sizeof(in_addr);
	int err;

	OFI_UNUSED(protocol);

	if(!socks) {
		WSASetLastError(WSAEINVAL);
		return SOCKET_ERROR;
	}

	socks[0] = socks[1] = INVALID_SOCKET;
	if ((lsock = socket(af == AF_UNIX ? AF_INET : af,
			    type, 0)) == INVALID_SOCKET)
		return SOCKET_ERROR;

	memset(&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;
	in_addr.sin_addr.s_addr = htonl(0x7f000001);

	if (bind(lsock, (struct sockaddr*)&in_addr, sizeof(in_addr))) {
		int err = WSAGetLastError();
		closesocket(lsock);
		WSASetLastError(err);
		return SOCKET_ERROR;
	}

	if (getsockname(lsock, (struct sockaddr*) &in_addr, &len)) {
		int err = WSAGetLastError();
		closesocket(lsock);
		WSASetLastError(err);
		return SOCKET_ERROR;
	}

	if (listen(lsock, 1))
		goto err;

	if ((socks[0] = (int)WSASocketW(af == AF_UNIX ? AF_INET : af,
					type, 0, NULL, 0, 0)) == INVALID_SOCKET)
		goto err;
	if (connect(socks[0], (const struct sockaddr*) &in_addr,
		    sizeof(in_addr)))
		goto err;
	if ((socks[1] = (int)accept(lsock, NULL, NULL)) == INVALID_SOCKET)
		goto err;

	closesocket(lsock);
	return 0;

err:
	err = WSAGetLastError();
	closesocket(lsock);
	closesocket(socks[0]);
	closesocket(socks[1]);
	WSASetLastError(err);
	return SOCKET_ERROR;
}

int ofi_getsockname(SOCKET fd, struct sockaddr *addr, socklen_t *len)
{
	struct sockaddr_storage sock_addr;
	socklen_t sock_addr_len = sizeof(sock_addr);
	int ret;

	ret = getsockname(fd, (struct sockaddr *) &sock_addr, &sock_addr_len);
	if (ret)
		return ret;

	if (addr)
		memcpy(addr, &sock_addr, MIN(*len, sock_addr_len));
	*len = sock_addr_len;

	return FI_SUCCESS;
}

int ofi_getpeername(SOCKET fd, struct sockaddr *addr, socklen_t *len)
{
	struct sockaddr_storage sock_addr;
	socklen_t sock_addr_len = sizeof(sock_addr);
	int ret;

	ret = getpeername(fd, (struct sockaddr *) &sock_addr, &sock_addr_len);
	if (ret)
		return ret;

	if (addr)
		memcpy(addr, &sock_addr, MIN(*len, sock_addr_len));
	*len = sock_addr_len;

	return FI_SUCCESS;
}

int fi_read_file(const char *dir, const char *file, char *buf, size_t size)
{
	char *path = 0;
	int len, lendir, lenfile, pathlen;

	HANDLE fd = INVALID_HANDLE_VALUE;
	DWORD read;

	len = -1;

	lendir = lstrlenA(dir);
	lenfile = lstrlenA(file);

	pathlen = lendir + lenfile + 2; /* dir + '\' + file + '0' */

	path = malloc(pathlen);
	if (!path)
		goto fn_nomem;

	lstrcpyA(path, dir);
	if (lenfile) {
		lstrcatA(path, "\\");
		lstrcatA(path, file);
	}

	fd = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (fd == INVALID_HANDLE_VALUE)
		goto fn_nofile;

	if (!ReadFile(fd, buf, (DWORD)size, &read, 0))
		goto fn_faread;

	len = (int)read;

	if (len > 0 && buf[len - 1] == '\n')
		buf[--len] = '\0';

fn_faread:
	CloseHandle(fd);
fn_nofile:
	free(path);
fn_nomem:
	return len;
}

static BOOL CALLBACK ofi_init_once_cb(PINIT_ONCE once, void* data, void** ctx)
{
	struct ofi_common_locks *locks = (struct ofi_common_locks *)data;

	OFI_UNUSED(once);
	OFI_UNUSED(ctx);

	InitializeCriticalSection(&locks->ini_lock);
	InitializeCriticalSection(&locks->util_fabric_lock);

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	OFI_UNUSED(instance);
	OFI_UNUSED(reserved);

	switch (reason) {
	case DLL_PROCESS_ATTACH:
		InitOnceExecuteOnce(&ofi_init_once, ofi_init_once_cb,
				    &common_locks, 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		fi_fini();
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}

int ofi_shm_map(struct util_shm *shm, const char *name, size_t size,
	int readonly, void **mapped)
{
	int ret = FI_SUCCESS;
	char *fname = 0;
	size_t len = lstrlenA(name) + sizeof(ofi_shm_prefix);
	LARGE_INTEGER large = {.QuadPart = size};
	DWORD access = FILE_MAP_READ | (readonly ? 0 : FILE_MAP_WRITE);

	ZeroMemory(shm, sizeof(*shm));

	fname = malloc(len);
	if (!fname) {
		ret = -FI_ENOMEM;
		goto fn_nomem;
	}
	shm->name = fname;

	lstrcpyA(fname, ofi_shm_prefix);
	lstrcatA(fname, name);

	if (!readonly) {
		shm->shared_fd = CreateFileMappingA(INVALID_HANDLE_VALUE, 0,
			PAGE_READWRITE, large.HighPart, large.LowPart,
			shm->name);
		if (!shm->shared_fd) {
			FI_WARN(&core_prov, FI_LOG_CORE, "CreateFileMapping failed\n");
			ret = -FI_EINVAL;
			goto fn_nofilemap;
		}
	} else { /* readonly */
		shm->shared_fd = OpenFileMappingA(access, FALSE, shm->name);
		if (!shm->shared_fd) {
			FI_WARN(&core_prov, FI_LOG_CORE, "OpenFileMapping failed\n");
			ret = -FI_EINVAL;
			goto fn_nofilemap;
		}
	}

	shm->ptr = MapViewOfFile(shm->shared_fd, access, 0, 0, size);
	if (!shm->ptr) {
		FI_WARN(&core_prov, FI_LOG_CORE, "MapViewOfFile failed\n");
		ret = -FI_EINVAL;
		goto fn_nomap;
	}

	/* size value not really used due to missing remap functionality,
	   but may be useful for debugging */
	shm->size = size;
	*mapped = shm->ptr;

	return FI_SUCCESS;

fn_nomap:
	CloseHandle(shm->shared_fd);
fn_nofilemap:
	free(fname);
fn_nomem:
	ZeroMemory(shm, sizeof(*shm));
	return ret;
}

int ofi_shm_unmap(struct util_shm *shm)
{
	if (shm->name)
		free((void*)shm->name);
	if (shm->ptr)
		UnmapViewOfFile(shm->ptr);
	if (shm->shared_fd)
		CloseHandle(shm->shared_fd);

	ZeroMemory(shm, sizeof(*shm));

	return FI_SUCCESS;
}

/* emulate sendmsg/recvmsg calls using temporary buffer */
ssize_t recvmsg(SOCKET sd, struct msghdr *msg, int flags)
{
	size_t len;
	ssize_t offset;
	size_t i;
	ssize_t read = -1;
	ssize_t received;
	char *buffer;

	assert(msg);
	assert(msg->msg_iov);

	if (msg->msg_iovlen > 1) {
		for (i = 0, len = 0; i < msg->msg_iovlen; i++)
			len += msg->msg_iov[i].iov_len;

		buffer = (char*)malloc(len);
		if (!buffer)
			goto fn_nomem;
	} else {
		buffer = msg->msg_iov[0].iov_base;
		len = msg->msg_iov[0].iov_len;
	}

	received = recvfrom(sd, buffer, (int)len, flags,
		(struct sockaddr *)msg->msg_name, &msg->msg_namelen);

	for(i = 0, offset = 0; i < msg->msg_iovlen && offset < received; i++) {
		ssize_t chunk_len = MIN(received - offset, (ssize_t)msg->msg_iov[i].iov_len);
		assert(msg->msg_iov[i].iov_base);
		memcpy(msg->msg_iov[i].iov_base, buffer + offset, chunk_len);
		offset += chunk_len;
	}
	read = received;

	if (msg->msg_iovlen > 1)
		free(buffer);

fn_complete:
	return read;

fn_nomem:
	read = -1;
	goto fn_complete;
}

ssize_t sendmsg(SOCKET sd, struct msghdr *msg, int flags)
{
	size_t len = 0;
	size_t offset;
	char *buffer;
	ssize_t sent = -1;
	size_t i;

	assert(msg);
	assert(msg->msg_iov);

	if (msg->msg_iovlen > 1) {
		/* calculate common length of data */
		for (i = 0; i < msg->msg_iovlen; i++)
			len += msg->msg_iov[i].iov_len;

		/* allocate temp buffer */
		buffer = (char*)malloc(len);
		if (!buffer)
			goto fn_nomem;
	} else {
		buffer = msg->msg_iov[0].iov_base;
		len = msg->msg_iov[0].iov_len;
	}

	/* copy data to temp buffer */
	for(i = 0, offset = 0; i < msg->msg_iovlen; i++) {
		assert(msg->msg_iov[i].iov_base);
		assert(offset + msg->msg_iov[i].iov_len <= len);
		memcpy(buffer + offset, msg->msg_iov[i].iov_base,
			msg->msg_iov[i].iov_len);
		offset += msg->msg_iov[i].iov_len;
	}

	/* send data */
	sent = sendto(sd, buffer, (int)len, flags,
		(struct sockaddr *)msg->msg_name, msg->msg_namelen);

	if (msg->msg_iovlen > 1)
		free(buffer);

fn_complete:
	return sent;

fn_nomem:
	sent = -1;
	goto fn_complete;
}

int getifaddrs(struct ifaddrs **ifap)
{
	ULONG subnet = 0;
	PULONG mask = &subnet;
	DWORD size, res, i = 0;
	int ret;
	PIP_ADAPTER_ADDRESSES adapter_addresses, aa;
	PIP_ADAPTER_UNICAST_ADDRESS ua;
	struct ifaddrs *head = NULL;
	struct sockaddr_in *pInAddr = NULL;
	SOCKADDR *pSockAddr = NULL;
	struct ifaddrs *fa;

	res = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX,
				   NULL, NULL, &size);
	if (res != ERROR_BUFFER_OVERFLOW)
		return -FI_ENOMEM;

	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);
	res = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX,
				   NULL, adapter_addresses, &size);
	if (res != ERROR_SUCCESS)
		return -FI_ENOMEM;

	for (aa = adapter_addresses; aa != NULL; aa = aa->Next) {
		if (aa->OperStatus != 1)
			continue;

		for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
			pSockAddr = ua->Address.lpSockaddr;
			if (pSockAddr->sa_family != AF_INET &&
				pSockAddr->sa_family != AF_INET6)
				continue;
			fa = calloc(sizeof(*fa), 1);
			if (!fa) {
				ret = -FI_ENOMEM;
				goto out;
			}

			fa->ifa_next = head;
			head = fa;

			fa->ifa_flags = IFF_UP;
			if (aa->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
				fa->ifa_flags |= IFF_LOOPBACK;

			fa->ifa_addr = (struct sockaddr *) &fa->in_addrs;
			fa->ifa_netmask = (struct sockaddr *) &fa->in_netmasks;
			fa->ifa_name = fa->ad_name;

			if (pSockAddr->sa_family == AF_INET) {
				subnet = 0;
				mask = &subnet;
				if (ConvertLengthToIpv4Mask(ua->OnLinkPrefixLength, mask) !=
					NO_ERROR) {
					ret = -FI_ENODATA;
					goto out;
				}
				struct sockaddr_in *addr4 = (struct sockaddr_in *)
							    &fa->in_addrs;
				struct sockaddr_in *netmask4 = (struct sockaddr_in *)
								&fa->in_netmasks;
				netmask4->sin_family = pSockAddr->sa_family;
				addr4->sin_family = pSockAddr->sa_family;
				netmask4->sin_addr.S_un.S_addr = *mask;
				pInAddr = (struct sockaddr_in *) pSockAddr;
				addr4->sin_addr = pInAddr->sin_addr;
			} else {
				struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)
							      &fa->in_addrs;
				(*addr6) = *(struct sockaddr_in6 *) pSockAddr;
			}
			fa->speed = aa->TransmitLinkSpeed;
			/* Generate fake Unix-like device names */
			sprintf_s(fa->ad_name, sizeof(fa->ad_name), "eth%d", i++);
		}
	}
	ret = 0;
out:
	free(adapter_addresses);
	if (ret && head) {
		free(head);
		head = NULL;
	}
	*ifap = head;
	return ret;
}

size_t ofi_ifaddr_get_speed(struct ifaddrs *ifa)
{
	return ifa->speed;
}

void freeifaddrs(struct ifaddrs *ifa)
{
	while (ifa) {
		struct ifaddrs *next = ifa->ifa_next;
		free(ifa);
		ifa = next;
	}
}

ssize_t
ofi_sendv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt, int flags)
{
	ssize_t size = 0, ret;
	int i;

	if (iov_cnt == 1) {
		return ofi_send_socket(fd, iovec[0].iov_base,
				       iovec[0].iov_len, flags);
	}

	for (i = 0; i < iov_cnt; i++) {
		ret = ofi_send_socket(fd, iovec[i].iov_base,
				      iovec[i].iov_len, flags);
		if (ret >= 0) {
			size += ret;
			if ((size_t) ret != iovec[i].iov_len)
				return size;
		} else {
			return size ? size : ret;
		}
	}
	return size;
}

ssize_t
ofi_recvv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt, int flags)
{
	ssize_t size = 0, ret;
	int i;

	if (iov_cnt == 1) {
		return ofi_recv_socket(fd, iovec[0].iov_base,
				       iovec[0].iov_len, flags);
	}

	for (i = 0; i < iov_cnt; i++) {
		ret = ofi_recv_socket(fd, iovec[i].iov_base,
				      iovec[i].iov_len, flags);
		if (ret >= 0) {
			size += ret;
			if ((size_t) ret != iovec[i].iov_len)
				return size;
		} else {
			return size ? size : ret;
		}
	}
	return size;
}

ssize_t ofi_writev_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt)
{
	return ofi_sendv_socket(fd, iovec, iov_cnt, 0);
}

ssize_t ofi_readv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt)
{
	return ofi_recvv_socket(fd, iovec, iov_cnt, 0);
}

ssize_t ofi_sendmsg_tcp(SOCKET fd, const struct msghdr *msg, int flags)
{
	return ofi_sendv_socket(fd, msg->msg_iov, msg->msg_iovlen, flags);
}

ssize_t ofi_recvmsg_tcp(SOCKET fd, struct msghdr *msg, int flags)
{
	return ofi_recvv_socket(fd, msg->msg_iov, msg->msg_iovlen, flags);
}

/*
 * We assume that the same WSARecvMsg pointer will work for all UDP sockets.
 */
ssize_t ofi_recvmsg_udp(SOCKET fd, struct msghdr *msg, int flags)
{
	static LPFN_WSARECVMSG WSARecvMsg = NULL;
	GUID guid = WSAID_WSARECVMSG;
	DWORD bytes;
	int ret;

	if (!WSARecvMsg) {
		pthread_mutex_lock(&common_locks.ini_lock);
		if (!WSARecvMsg) {
			ret = WSAIoctl(fd, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid,
					sizeof(guid), &WSARecvMsg, sizeof(WSARecvMsg),
					&bytes, NULL, NULL);
		} else {
			ret = 0;
		}
		pthread_mutex_unlock(&common_locks.ini_lock);
		if (ret)
			return ret;
	}

	ret = WSARecvMsg(fd, (LPWSAMSG) msg, &bytes, NULL, NULL);
	return ret ? ret : bytes;
}


struct ofi_pollfds_ctx *ofi_pollfds_get_ctx(struct ofi_pollfds *pfds, int fd)
{
	struct ofi_pollfds_ctx *ctx = NULL;
	int i;

	assert(ofi_genlock_held(&pfds->lock));

	/* 0 is signaling fd */
	for (i = 1; i < pfds->size; i++) {
		ctx = &pfds->ctx[i];
		if (ctx->index >= 0 && ctx->index < pfds->nfds &&
		    pfds->fds[ctx->index].fd == fd)
			return ctx;
	}

	return NULL;
}

struct ofi_pollfds_ctx *ofi_pollfds_alloc_ctx(struct ofi_pollfds *pfds, int fd)
{
	struct ofi_pollfds_ctx *ctx;
	int i;

	assert(ofi_genlock_held(&pfds->lock));
	assert(!ofi_pollfds_get_ctx(pfds, fd));
	/* 0 is signaling fd */
	for (i = 1; i < pfds->size; i++) {
		if (pfds->ctx[i].index == -1) {
			ctx = &pfds->ctx[i];
			goto insert;
		}
	}

	if (ofi_pollfds_grow(pfds, pfds->size + 1))
		return NULL;

	ctx = &pfds->ctx[pfds->nfds];
insert:
	assert(ctx->index < 0);
	ctx->index = pfds->nfds++;
	return ctx;
}
