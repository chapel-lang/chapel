/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2017  Los Alamos National Security, LLC.
 *                     All rights reserved.
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

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "sock.h"
#include "sock_util.h"

#include "ofi_osd.h"
#include "ofi_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_AV, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_AV, __VA_ARGS__)

#define SOCK_AV_TABLE_SZ(count, av_name) (sizeof(struct sock_av_table_hdr) + \
				SOCK_IS_SHARED_AV(av_name) * count * sizeof(uint64_t) + \
				count * sizeof(struct sock_av_addr))
#define SOCK_IS_SHARED_AV(av_name) ((av_name) ? 1 : 0)

int sock_av_get_addr_index(struct sock_av *av, union ofi_sock_ip *addr)
{
	int i;
	struct sock_av_addr *av_addr;

	fastlock_acquire(&av->table_lock);
	for (i = 0; i < (int)av->table_hdr->size; i++) {
		av_addr = &av->table[i];
		if (!av_addr->valid)
			continue;

		 if (ofi_equals_sockaddr((const struct sockaddr *) addr,
					 (const struct sockaddr *) &av_addr->addr)) {
			fastlock_release(&av->table_lock);
			return i;
		 }
	}
	fastlock_release(&av->table_lock);
	SOCK_LOG_DBG("failed to get index in AV\n");
	return -1;
}

int sock_av_compare_addr(struct sock_av *av,
			 fi_addr_t addr1, fi_addr_t addr2)
{
	int index1, index2;
	struct sock_av_addr *av_addr1, *av_addr2;
	int ret;

	index1 = ((uint64_t)addr1 & av->mask);
	index2 = ((uint64_t)addr2 & av->mask);

	fastlock_acquire(&av->table_lock);
	if (index1 >= (int)av->table_hdr->size || index1 < 0 ||
	    index2 >= (int)av->table_hdr->size || index2 < 0) {
		SOCK_LOG_ERROR("requested rank is larger than av table\n");
		fastlock_release(&av->table_lock);
		return -1;
	}

	av_addr1 = &av->table[index1];
	av_addr2 = &av->table[index2];

	/* Return 0 if the addresses match */
	ret = !ofi_equals_sockaddr(&av_addr1->addr.sa, &av_addr2->addr.sa);
	fastlock_release(&av->table_lock);
	return ret;
}

static inline void sock_av_report_success(struct sock_av *av, void *context,
					  int num_done, uint64_t flags)
{
	struct fi_eq_entry eq_entry;

	if (!av->eq)
		return;

	eq_entry.fid = &av->av_fid.fid;
	eq_entry.context = context;
	eq_entry.data = num_done;
	sock_eq_report_event(av->eq, FI_AV_COMPLETE,
			     &eq_entry, sizeof(eq_entry), flags);
}

static inline void sock_av_report_error(struct sock_av *av,
					void *context, int index, int err)
{
	if (!av->eq)
		return;

	sock_eq_report_error(av->eq, &av->av_fid.fid,
			     context, index, err, -err, NULL, 0);
}

static int sock_av_is_valid_address(const struct sockaddr *addr)
{
	return ofi_sizeofaddr(addr);
}

static void sock_update_av_table(struct sock_av *_av, size_t count)
{
	_av->table = (struct sock_av_addr *)
		((char *)_av->table_hdr +
		SOCK_IS_SHARED_AV(_av->attr.name) * count * sizeof(uint64_t) +
		sizeof(struct sock_av_table_hdr));
}

static int sock_resize_av_table(struct sock_av *av)
{
	void *new_addr;
	size_t new_count, table_sz, old_sz;

	new_count = av->table_hdr->size * 2;
	table_sz = SOCK_AV_TABLE_SZ(new_count, av->attr.name);
	old_sz = SOCK_AV_TABLE_SZ(av->table_hdr->size, av->attr.name);

	if (av->attr.name) {
		new_addr = sock_mremap(av->table_hdr, old_sz, table_sz);
		if (new_addr == MAP_FAILED)
			return -1;

		av->idx_arr[av->table_hdr->stored] = av->table_hdr->stored;
	} else {
		new_addr = realloc(av->table_hdr, table_sz);
		if (!new_addr)
			return -1;
		memset((char *) new_addr + old_sz, 0, table_sz - old_sz);
	}

	av->table_hdr = new_addr;
	av->table_hdr->size = new_count;
	sock_update_av_table(av, new_count);

	return 0;
}

static int sock_av_get_next_index(struct sock_av *av)
{
	uint64_t i;

	for (i = 0; i < av->table_hdr->size; i++) {
		if (!av->table[i].valid)
			return i;
	}

	return -1;
}

static int sock_check_table_in(struct sock_av *_av, const struct sockaddr *addr,
			       fi_addr_t *fi_addr, int count, uint64_t flags,
			       void *context)
{
	int i, ret = 0;
	uint64_t j;
	char sa_ip[INET6_ADDRSTRLEN];
	struct sock_av_addr *av_addr;
	int index;

	if ((_av->attr.flags & FI_EVENT) && !_av->eq)
		return -FI_ENOEQ;

	if (_av->attr.flags & FI_READ) {
		for (i = 0; i < count; i++) {
			for (j = 0; j < _av->table_hdr->size; j++) {
				if (_av->table[j].valid &&
				     !sock_av_is_valid_address(&addr[i])) {
					if (fi_addr)
						fi_addr[i] = FI_ADDR_NOTAVAIL;
					sock_av_report_error(_av, context, i,
								FI_EINVAL);
					continue;
				}

				av_addr = &_av->table[j];
				if (memcmp(&av_addr->addr, &addr[i],
					   ofi_sizeofaddr(&addr[i])) == 0) {
					SOCK_LOG_DBG("Found addr in shared av\n");
					if (fi_addr)
						fi_addr[i] = (fi_addr_t)j;
					ret++;
				}
			}
		}
		sock_av_report_success(_av, context, ret, flags);
		return (_av->attr.flags & FI_EVENT) ? 0 : ret;
	}

	for (i = 0, ret = 0; i < count; i++) {
		if (!sock_av_is_valid_address(&addr[i])) {
			if (fi_addr)
				fi_addr[i] = FI_ADDR_NOTAVAIL;
			sock_av_report_error(_av, context, i, FI_EINVAL);
			continue;
		}
		if (_av->table_hdr->stored == _av->table_hdr->size) {
			index = sock_av_get_next_index(_av);
			if (index < 0) {
				if (sock_resize_av_table(_av)) {
					if (fi_addr)
						fi_addr[i] = FI_ADDR_NOTAVAIL;
					sock_av_report_error(_av, context, i, FI_ENOMEM);
					continue;
				}
				index = _av->table_hdr->stored++;
			}
		} else {
			index = _av->table_hdr->stored++;
		}

		av_addr = &_av->table[index];
		inet_ntop(addr[i].sa_family, ofi_get_ipaddr(&addr[i]),
			  sa_ip, sizeof sa_ip);
		SOCK_LOG_DBG("AV-INSERT: dst_addr family: %d, IP %s, port: %d\n",
			      (&addr[i])->sa_family, sa_ip,
			      ofi_addr_get_port(&addr[i]));

		memcpy(&av_addr->addr, &addr[i], ofi_sizeofaddr(&addr[i]));
		if (fi_addr)
			fi_addr[i] = (fi_addr_t)index;

		av_addr->valid = 1;
		ret++;
	}
	sock_av_report_success(_av, context, ret, flags);
	return (_av->attr.flags & FI_EVENT) ? 0 : ret;
}

static int sock_av_insert(struct fid_av *av, const void *addr, size_t count,
			  fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct sock_av *_av;
	int ret = 0;

	_av = container_of(av, struct sock_av, av_fid);

	fastlock_acquire(&_av->table_lock);
	ret = sock_check_table_in(_av, (const struct sockaddr *) addr,
				   fi_addr, count, flags, context);
	fastlock_release(&_av->table_lock);
	return ret;
}

static int sock_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			  size_t *addrlen)
{
	int index;
	struct sock_av *_av;
	struct sock_av_addr *av_addr;

	_av = container_of(av, struct sock_av, av_fid);
	index = ((uint64_t)fi_addr & _av->mask);

	fastlock_acquire(&_av->table_lock);
	if (index >= (int)_av->table_hdr->size || index < 0) {
		SOCK_LOG_ERROR("requested address not inserted\n");
		fastlock_release(&_av->table_lock);
		return -EINVAL;
	}

	av_addr = &_av->table[index];
	memcpy(addr, &av_addr->addr, MIN(*addrlen, (size_t)_av->addrlen));
	fastlock_release(&_av->table_lock);
	*addrlen = _av->addrlen;
	return 0;
}

static int _sock_av_insertsvc(struct fid_av *av, const char *node,
			      const char *service, fi_addr_t *fi_addr,
			      uint64_t flags, void *context)
{
	int ret;
	struct addrinfo sock_hints;
	struct addrinfo *result = NULL;
	struct sock_av *_av;

	_av = container_of(av, struct sock_av, av_fid);
	memset(&sock_hints, 0, sizeof(struct addrinfo));
	/* Map all services to IPv4 addresses -- for compatibility */
	sock_hints.ai_family = AF_INET;
	sock_hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(node, service, &sock_hints, &result);
	if (ret) {
		if (_av->eq) {
			sock_av_report_error(_av, context, 0, FI_EINVAL);
			sock_av_report_success(_av, context, 0, flags);
		}
		return -ret;
	}

	fastlock_acquire(&_av->table_lock);
	ret = sock_check_table_in(_av, result->ai_addr,
				  fi_addr, 1, flags, context);
	fastlock_release(&_av->table_lock);
	freeaddrinfo(result);
	return ret;
}

static int sock_av_insertsvc(struct fid_av *av, const char *node,
		   const char *service, fi_addr_t *fi_addr,
		   uint64_t flags, void *context)
{
	if (!service) {
		SOCK_LOG_ERROR("Port not provided\n");
		return -FI_EINVAL;
	}

	return _sock_av_insertsvc(av, node, service, fi_addr, flags, context);
}

static int sock_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		      const char *service, size_t svccnt, fi_addr_t *fi_addr,
		      uint64_t flags, void *context)
{
	int ret = 0, success = 0, err_code = 0, len1, len2;
	int var_port, var_host;
	char base_host[FI_NAME_MAX] = {0};
	char tmp_host[FI_NAME_MAX] = {0};
	char tmp_port[FI_NAME_MAX] = {0};
	int hostlen, offset = 0, fmt;
	size_t i, j;

	if (!node || !service || node[0] == '\0') {
		SOCK_LOG_ERROR("Node/service not provided\n");
		return -FI_EINVAL;
	}

	hostlen = strlen(node);
	while (isdigit(*(node + hostlen - (offset + 1))))
		offset++;

	if (*(node + hostlen - offset) == '.')
		fmt = 0;
	else
		fmt = offset;

	if (hostlen - offset >= FI_NAME_MAX)
		return -FI_ETOOSMALL;
	memcpy(base_host, node, hostlen - offset);
	var_port = atoi(service);
	var_host = atoi(node + hostlen - offset);

	for (i = 0; i < nodecnt; i++) {
		for (j = 0; j < svccnt; j++) {
			len1 = snprintf(tmp_host, FI_NAME_MAX, "%s%0*d",
					base_host, fmt, var_host + (int)i);
			len2 = snprintf(tmp_port, FI_NAME_MAX,  "%d",
					var_port + (int)j);
			if (len1 > 0 && len1 < FI_NAME_MAX && len2 > 0 && len2 < FI_NAME_MAX) {
				ret = _sock_av_insertsvc(av, tmp_host, tmp_port, fi_addr, flags, context);
				if (ret == 1)
					success++;
				else
					err_code = ret;
			} else {
				SOCK_LOG_ERROR("Node/service value is not valid\n");
				err_code = -FI_ETOOSMALL;
			}
		}
	}
	return success > 0 ? success : err_code;
}


static int sock_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			  uint64_t flags)
{
	size_t i;
	struct sock_av *_av;
	struct sock_av_addr *av_addr;
	struct dlist_entry *item;
	struct fid_list_entry *fid_entry;
	struct sock_ep *sock_ep;
	struct sock_conn *conn;
	uint16_t idx;

	_av = container_of(av, struct sock_av, av_fid);
	fastlock_acquire(&_av->list_lock);
	dlist_foreach(&_av->ep_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		sock_ep = container_of(fid_entry->fid, struct sock_ep, ep.fid);
		fastlock_acquire(&sock_ep->attr->cmap.lock);
		for (i = 0; i < count; i++) {
			idx = fi_addr[i] & sock_ep->attr->av->mask;
			conn = ofi_idm_lookup(&sock_ep->attr->av_idm, idx);
			if (conn) {
				/* A peer may be using the connection, so leave
				 * it operational, just dissociate it from AV.
				 */
				if (conn->av_index == idx)
					conn->av_index = FI_ADDR_NOTAVAIL;
				ofi_idm_clear(&sock_ep->attr->av_idm, idx);
			}
		}
		fastlock_release(&sock_ep->attr->cmap.lock);
	}
	fastlock_release(&_av->list_lock);

	for (i = 0; i < count; i++) {
		av_addr = &_av->table[fi_addr[i]];
		av_addr->valid = 0;
	}

	return 0;
}

static const char *sock_av_straddr(struct fid_av *av, const void *addr,
				   char *buf, size_t *len)
{
	const struct sockaddr *sa = addr;
	char straddr[OFI_ADDRSTRLEN];
	char ipaddr[INET6_ADDRSTRLEN];
	int size;

	if (!inet_ntop(sa->sa_family, ofi_get_ipaddr(sa), ipaddr, sizeof(ipaddr)))
		return NULL;

	size = snprintf(straddr, sizeof(straddr), "%s:%d",
			ipaddr, ofi_addr_get_port(sa));
	snprintf(buf, *len, "%s", straddr);
	*len = size + 1;
	return buf;
}

static int sock_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct sock_av *av;
	struct sock_eq *eq;

	if (bfid->fclass != FI_CLASS_EQ)
		return -FI_EINVAL;

	av = container_of(fid, struct sock_av, av_fid.fid);
	eq = container_of(bfid, struct sock_eq, eq.fid);
	av->eq = eq;
	return 0;
}

static int sock_av_close(struct fid *fid)
{
	struct sock_av *av;
	int ret = 0;
	av = container_of(fid, struct sock_av, av_fid.fid);
	if (ofi_atomic_get32(&av->ref))
		return -FI_EBUSY;

	if (!av->shared) {
		free(av->table_hdr);
	} else {
		ret = ofi_shm_unmap(&av->shm);
		if (ret)
			SOCK_LOG_ERROR("unmap failed: %s\n",
				       strerror(ofi_syserr()));
	}

	ofi_atomic_dec32(&av->domain->ref);
	fastlock_destroy(&av->list_lock);
	fastlock_destroy(&av->table_lock);
	free(av);
	return 0;
}

static struct fi_ops sock_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_av_close,
	.bind = sock_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av sock_am_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = sock_av_insert,
	.insertsvc = sock_av_insertsvc,
	.insertsym = sock_av_insertsym,
	.remove = sock_av_remove,
	.lookup = sock_av_lookup,
	.straddr = sock_av_straddr
};

static struct fi_ops_av sock_at_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = sock_av_insert,
	.insertsvc = sock_av_insertsvc,
	.insertsym = sock_av_insertsym,
	.remove = sock_av_remove,
	.lookup = sock_av_lookup,
	.straddr = sock_av_straddr
};

static int sock_verify_av_attr(struct fi_av_attr *attr)
{
	switch (attr->type) {
	case FI_AV_MAP:
	case FI_AV_TABLE:
	case FI_AV_UNSPEC:
		break;
	default:
		return -FI_EINVAL;
	}

	if (attr->flags & FI_READ && !attr->name)
		return -FI_EINVAL;

	if (attr->rx_ctx_bits > SOCK_EP_MAX_CTX_BITS) {
		SOCK_LOG_ERROR("Invalid rx_ctx_bits\n");
		return -FI_EINVAL;
	}
	return 0;
}

int sock_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context)
{
	int ret = 0;
	struct sock_domain *dom;
	struct sock_av *_av;
	size_t table_sz;

	if (!attr || sock_verify_av_attr(attr))
		return -FI_EINVAL;

	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	dom = container_of(domain, struct sock_domain, dom_fid);
	if (dom->attr.av_type != FI_AV_UNSPEC &&
	    dom->attr.av_type != attr->type)
		return -FI_EINVAL;

	_av = calloc(1, sizeof(*_av));
	if (!_av)
		return -FI_ENOMEM;

	_av->attr = *attr;
	_av->attr.count = (attr->count) ? attr->count : sock_av_def_sz;
	table_sz = SOCK_AV_TABLE_SZ(_av->attr.count, attr->name);

	if (attr->name) {
		ret = ofi_shm_map(&_av->shm, attr->name, table_sz,
				attr->flags & FI_READ, (void**)&_av->table_hdr);

		if (ret || _av->table_hdr == MAP_FAILED) {
			SOCK_LOG_ERROR("map failed\n");
			ret = -FI_EINVAL;
			goto err;
		}

		_av->idx_arr = (uint64_t *)(_av->table_hdr + 1);
		_av->attr.map_addr = _av->idx_arr;
		attr->map_addr = _av->attr.map_addr;
		SOCK_LOG_DBG("Updating map_addr: %p\n", _av->attr.map_addr);

		if (attr->flags & FI_READ) {
			if (_av->table_hdr->size != _av->attr.count) {
				ret = -FI_EINVAL;
				goto err2;
			}
		} else {
			_av->table_hdr->size = _av->attr.count;
			_av->table_hdr->stored = 0;
		}
		_av->shared = 1;
	} else {
		_av->table_hdr = calloc(1, table_sz);
		if (!_av->table_hdr) {
			ret = -FI_ENOMEM;
			goto err;
		}
		_av->table_hdr->size = _av->attr.count;
	}
	sock_update_av_table(_av, _av->attr.count);

	_av->av_fid.fid.fclass = FI_CLASS_AV;
	_av->av_fid.fid.context = context;
	_av->av_fid.fid.ops = &sock_av_fi_ops;

	switch (attr->type) {
	case FI_AV_MAP:
		_av->av_fid.ops = &sock_am_ops;
		break;
	case FI_AV_TABLE:
		_av->av_fid.ops = &sock_at_ops;
		break;
	default:
		ret = -FI_EINVAL;
		goto err2;
	}

	ofi_atomic_initialize32(&_av->ref, 0);
	ofi_atomic_inc32(&dom->ref);
	_av->domain = dom;
	switch (dom->info.addr_format) {
	case FI_SOCKADDR_IN:
		_av->addrlen = sizeof(struct sockaddr_in);
		break;
	case FI_SOCKADDR_IN6:
		_av->addrlen = sizeof(struct sockaddr_in6);
		break;
	default:
		SOCK_LOG_ERROR("Invalid address format: only IP supported\n");
		ret = -FI_EINVAL;
		goto err2;
	}
	dlist_init(&_av->ep_list);
	fastlock_init(&_av->list_lock);
	fastlock_init(&_av->table_lock);
	_av->rx_ctx_bits = attr->rx_ctx_bits;
	_av->mask = attr->rx_ctx_bits ?
		((uint64_t)1 << (64 - attr->rx_ctx_bits)) - 1 : ~0;
	*av = &_av->av_fid;
	return 0;

err2:
	if(attr->name) {
		ofi_shm_unmap(&_av->shm);
	} else {
		if(_av->table_hdr != MAP_FAILED)
			free(_av->table_hdr);
	}
err:
	free(_av);
	return ret;
}
