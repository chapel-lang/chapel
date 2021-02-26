/*
 * Copyright (c) 2014-2016 Intel Corporation, Inc.  All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <ofi_util.h>
#include <ofi.h>

static DEFINE_LIST(fabric_list);
extern struct ofi_common_locks common_locks;

void ofi_fabric_insert(struct util_fabric *fabric)
{
	pthread_mutex_lock(&common_locks.util_fabric_lock);
	dlist_insert_tail(&fabric->list_entry, &fabric_list);
	pthread_mutex_unlock(&common_locks.util_fabric_lock);
}

static int util_match_fabric(struct dlist_entry *item, const void *arg)
{
	struct util_fabric *fabric;
	struct util_fabric_info *fabric_info = (struct util_fabric_info *)arg;

	fabric = container_of(item, struct util_fabric, list_entry);
	return (fabric_info->prov == fabric->prov) &&
		!strcmp(fabric->name, fabric_info->name);
}

void ofi_fabric_remove(struct util_fabric *fabric)
{
	pthread_mutex_lock(&common_locks.util_fabric_lock);
	dlist_remove(&fabric->list_entry);
	pthread_mutex_unlock(&common_locks.util_fabric_lock);
}


static int ofi_fid_match(struct dlist_entry *entry, const void *fid)
{
	struct fid_list_entry *item;
	item = container_of(entry, struct fid_list_entry, entry);
	return (item->fid == fid);
}

int fid_list_insert(struct dlist_entry *fid_list, fastlock_t *lock,
		    struct fid *fid)
{
	int ret = 0;
	struct dlist_entry *entry;
	struct fid_list_entry *item;

	fastlock_acquire(lock);
	entry = dlist_find_first_match(fid_list, ofi_fid_match, fid);
	if (entry)
		goto out;

	item = calloc(1, sizeof(*item));
	if (!item) {
		ret = -FI_ENOMEM;
		goto out;
	}

	item->fid = fid;
	dlist_insert_tail(&item->entry, fid_list);
out:
	fastlock_release(lock);
	return ret;
}

void fid_list_remove(struct dlist_entry *fid_list, fastlock_t *lock,
		     struct fid *fid)
{
	struct fid_list_entry *item;
	struct dlist_entry *entry;

	fastlock_acquire(lock);
	entry = dlist_remove_first_match(fid_list, ofi_fid_match, fid);
	fastlock_release(lock);

	if (entry) {
		item = container_of(entry, struct fid_list_entry, entry);
		free(item);
	}
}

static int util_find_domain(struct dlist_entry *item, const void *arg)
{
	const struct util_domain *domain;
	const struct fi_info *info = arg;

	domain = container_of(item, struct util_domain, list_entry);

	return !strcmp(domain->name, info->domain_attr->name) &&
		!((info->caps | info->domain_attr->caps) & ~domain->info_domain_caps) &&
		 (((info->mode | info->domain_attr->mode) &
		   domain->info_domain_mode) == domain->info_domain_mode) &&
		 ((info->domain_attr->mr_mode & domain->mr_mode) == domain->mr_mode);
}

/*
 * Produces 1 fi_info output for each fi_info entry in the provider's base
 * list (stored with util_prov), subject to the base fi_info meeting the
 * user's hints.
 */
int util_getinfo(const struct util_prov *util_prov, uint32_t version,
		 const char *node, const char *service, uint64_t flags,
		 const struct fi_info *hints, struct fi_info **info)
{
	struct util_fabric *fabric;
	struct util_domain *domain;
	struct dlist_entry *item;
	const struct fi_provider *prov = util_prov->prov;
	struct util_fabric_info fabric_info;
	struct fi_info *saved_info;
	int ret, copy_dest;

	FI_DBG(prov, FI_LOG_CORE, "checking info\n");

	if ((flags & FI_SOURCE) && !node && !service) {
		FI_INFO(prov, FI_LOG_CORE,
			"FI_SOURCE set, but no node or service\n");
		return -FI_EINVAL;
	}

	ret = ofi_prov_check_dup_info(util_prov, version, hints, info);
	if (ret)
		return ret;

	ofi_alter_info(*info, hints, version);

	saved_info = *info;

	for (; *info; *info = (*info)->next) {

		fabric_info.name = (*info)->fabric_attr->name;
		fabric_info.prov = util_prov->prov;

		pthread_mutex_lock(&common_locks.util_fabric_lock);
		item = dlist_find_first_match(&fabric_list, util_match_fabric,
					      &fabric_info);
		if (item) {
			fabric = container_of(item, struct util_fabric, list_entry);
			FI_DBG(prov, FI_LOG_CORE, "Found opened fabric\n");
			(*info)->fabric_attr->fabric = &fabric->fabric_fid;

			fastlock_acquire(&fabric->lock);
			item = dlist_find_first_match(&fabric->domain_list,
						      util_find_domain, *info);
			if (item) {
				FI_DBG(prov, FI_LOG_CORE,
				       "Found open domain\n");
				domain = container_of(item, struct util_domain,
						      list_entry);
				(*info)->domain_attr->domain =
						&domain->domain_fid;
			}
			fastlock_release(&fabric->lock);

		}
		pthread_mutex_unlock(&common_locks.util_fabric_lock);

		if (flags & FI_SOURCE) {
			ret = ofi_get_addr(&(*info)->addr_format, flags,
					  node, service, &(*info)->src_addr,
					  &(*info)->src_addrlen);
			if (ret) {
				FI_INFO(prov, FI_LOG_CORE,
					"source address not available\n");
				goto err;
			}
			copy_dest = (hints && hints->dest_addr);
		} else {
			if (node || service) {
				copy_dest = 0;
				ret = ofi_get_addr(&(*info)->addr_format,
						   flags, node, service,
						   &(*info)->dest_addr,
						   &(*info)->dest_addrlen);
				if (ret) {
					FI_INFO(prov, FI_LOG_CORE,
						"cannot resolve dest address\n");
					goto err;
				}
			} else {
				copy_dest = (hints && hints->dest_addr);
			}

			if (hints && hints->src_addr) {
				(*info)->src_addr = mem_dup(hints->src_addr,
						    hints->src_addrlen);
				if (!(*info)->src_addr) {
					ret = -FI_ENOMEM;
					goto err;
				}
				(*info)->src_addrlen = hints->src_addrlen;
				(*info)->addr_format = hints->addr_format;
			}
		}

		if (copy_dest) {
			(*info)->dest_addr = mem_dup(hints->dest_addr,
						     hints->dest_addrlen);
			if (!(*info)->dest_addr) {
				ret = -FI_ENOMEM;
				goto err;
			}
			(*info)->dest_addrlen = hints->dest_addrlen;
			(*info)->addr_format = hints->addr_format;
		}

		if ((*info)->dest_addr && !(*info)->src_addr) {
			ret = ofi_get_src_addr((*info)->addr_format,
					       (*info)->dest_addr,
					       (*info)->dest_addrlen,
					       &(*info)->src_addr,
					       &(*info)->src_addrlen);
			if (ret) {
				FI_INFO(prov, FI_LOG_CORE,
					"cannot resolve source address\n");
			}
		}
	}

	*info = saved_info;

	return 0;

err:
	fi_freeinfo(*info);
	return ret;
}

static void util_set_netif_names(struct fi_info *info,
				 struct ofi_addr_list_entry *addr_entry)
{
	char *name;

	name = strdup(addr_entry->net_name);
	if (name) {
		free(info->fabric_attr->name);
		info->fabric_attr->name = name;
	}

	name = strdup(addr_entry->ifa_name);
	if (name) {
		free(info->domain_attr->name);
		info->domain_attr->name = name;
	}
}

/*
 * Produces 1 fi_info output for each usable IP address in the system for the
 * given fi_info input.
 */
#if HAVE_GETIFADDRS
static void util_getinfo_ifs(const struct util_prov *prov, struct fi_info *src_info,
			     struct fi_info **head, struct fi_info **tail)
{
	struct fi_info *cur;
	struct slist addr_list;
	size_t addrlen;
	uint32_t addr_format;
	struct slist_entry *entry, *prev;
	struct ofi_addr_list_entry *addr_entry;

	*head = *tail = NULL;
	slist_init(&addr_list);

	ofi_get_list_of_addr(prov->prov, "iface", &addr_list);

	(void) prev; /* Makes compiler happy */
	slist_foreach(&addr_list, entry, prev) {
		addr_entry = container_of(entry, struct ofi_addr_list_entry, entry);

		cur = fi_dupinfo(src_info);
		if (!cur)
			break;

		if (!*head) {
			*head = cur;
			FI_INFO(prov->prov, FI_LOG_CORE, "Chosen addr for using: %s,"
				" speed %zu\n", addr_entry->ipstr, addr_entry->speed);
		} else {
			(*tail)->next = cur;
		}
		*tail = cur;

		switch (addr_entry->ipaddr.sin.sin_family) {
		case AF_INET:
			addrlen = sizeof(struct sockaddr_in);
			addr_format = FI_SOCKADDR_IN;
			break;
		case AF_INET6:
			addrlen = sizeof(struct sockaddr_in6);
			addr_format = FI_SOCKADDR_IN6;
			break;
		default:
			continue;
		}

		cur->src_addr = mem_dup(&addr_entry->ipaddr, addrlen);
		if (cur->src_addr) {
			cur->src_addrlen = addrlen;
			cur->addr_format = addr_format;
		}
		util_set_netif_names(cur, addr_entry);
	}

	ofi_free_list_of_addr(&addr_list);
	if (!*head) {
		*head = src_info;
		*tail = src_info;
	}
}
#else
static void util_getinfo_ifs(const struct util_prov *prov, struct fi_info *src_info,
			     struct fi_info **head, struct fi_info **tail)
{
	*head = src_info;
	*tail = src_info;
}
#endif

static int util_match_addr(struct slist_entry *entry, const void *addr)
{
	struct ofi_addr_list_entry *addr_entry;

	addr_entry = container_of(entry, struct ofi_addr_list_entry, entry);
	return ofi_equals_ipaddr(&addr_entry->ipaddr.sa, addr);
}

int ofi_ip_getinfo(const struct util_prov *prov, uint32_t version,
		   const char *node, const char *service, uint64_t flags,
		   const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *head, *tail, *cur, **prev;
	struct ofi_addr_list_entry *addr_entry;
	struct slist addr_list;
	struct slist_entry *entry;
	int ret;

	ret = util_getinfo(prov, version, node, service, flags,
			   hints, info);
	if (ret)
		return ret;

	prev = info;
	for (cur = *info; cur; cur = cur->next) {
		if (!cur->src_addr && !cur->dest_addr) {
			util_getinfo_ifs(prov, cur, &head, &tail);
			if (head != cur) {
				tail->next = (*prev)->next;
				*prev = head;

				cur->next = NULL;
				fi_freeinfo(cur);
				cur = tail;
			}
		} else if (cur->src_addr) {
			slist_init(&addr_list);
			ofi_get_list_of_addr(prov->prov, "iface", &addr_list);

			entry = slist_find_first_match(&addr_list, util_match_addr,
						(*info)->src_addr);
			if (entry) {
				addr_entry = container_of(entry,
						struct ofi_addr_list_entry, entry);
				util_set_netif_names(cur, addr_entry);
			}
			ofi_free_list_of_addr(&addr_list);
		}
		prev = &cur->next;
	}

	return 0;
}
