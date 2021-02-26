/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2019      Triad National Security, LLC.
 *                         All rights reserved.
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

//
// Address vector common code
//
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_util.h"
#include "gnix_hashtable.h"
#include "gnix_av.h"
#include "gnix_cm.h"

/*
 * local variables and structs
 */

#define GNIX_AV_ENTRY_VALID		(1ULL)
#define GNIX_AV_ENTRY_CM_NIC_ID		(1ULL << 2)

struct gnix_av_block {
	struct slist_entry        slist;
	struct gnix_av_addr_entry *base;
};

/*******************************************************************************
 * Forward declarations of ops structures.
 ******************************************************************************/
static struct fi_ops_av gnix_av_ops;
static struct fi_ops gnix_fi_av_ops;

/*******************************************************************************
 * Helper functions.
 ******************************************************************************/
static int gnix_verify_av_attr(struct fi_av_attr *attr)
{
	int ret = FI_SUCCESS;

	if (attr->rx_ctx_bits > GNIX_RX_CTX_MAX_BITS) {
		GNIX_WARN(FI_LOG_AV, "rx_ctx_bits too big\n");
		return -FI_EINVAL;
	}

	switch (attr->type) {
	case FI_AV_TABLE:
	case FI_AV_MAP:
	case FI_AV_UNSPEC:
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

	if (attr->name != NULL) {
		ret = -FI_ENOSYS;
	}

	return ret;
}

/*
 * Check the capacity of the internal table used to represent FI_AV_TABLE type
 * address vectors. Initially the table starts with a capacity and count of 0
 * and the capacity increases by roughly double each time a resize is necessary.
 */
static int gnix_check_capacity(struct gnix_fid_av *av, size_t count)
{
	struct gnix_av_addr_entry *addrs = NULL;
	int *valid_entry_vec = NULL;
	size_t capacity = av->capacity;
	size_t prev_capacity;

	/*
	 * av->count + count is the amount of used indices after adding the
	 * count items.
	 */
	prev_capacity = capacity;
	while (capacity < av->count + count) {
		/*
		 * Handle initial capacity of 0, by adding 1.
		 */
		capacity = capacity * 2 + 1;
	}

	/*
	 * Don't need to grow the table.
	 */
	if (capacity == av->capacity) {
		return FI_SUCCESS;
	}

	addrs = realloc(av->table, capacity * sizeof(*addrs));
	if (!addrs) {
		return -FI_ENOMEM;
	}

	memset(&addrs[prev_capacity], 0, (capacity - prev_capacity) *
	       sizeof(*addrs));

	valid_entry_vec = realloc(av->valid_entry_vec, capacity * sizeof(int));
	if (!valid_entry_vec) {
		return -FI_ENOMEM;
	}

	memset(&valid_entry_vec[prev_capacity], 0, (capacity - prev_capacity) *
	       sizeof(int));

	/*
	 * Update table and capacity to reflect new values.
	 */
	av->table = addrs;
	av->valid_entry_vec = valid_entry_vec;
	av->capacity = capacity;

	return FI_SUCCESS;
}

/*******************************************************************************
 * FI_AV_TABLE specific implementations.
 ******************************************************************************/
static int table_insert(struct gnix_fid_av *av_priv, const void *addr,
			size_t count, fi_addr_t *fi_addr, uint64_t flags,
			void *context)
{
	struct gnix_ep_name ep_name;
	int ret = FI_SUCCESS, success_cnt = 0;
	size_t index, i;
	int *entry_err = context;

	if (gnix_check_capacity(av_priv, count)) {
		return -FI_ENOMEM;
	}

	assert(av_priv->table);

	for (index = av_priv->count, i = 0; i < count; index++, i++) {
		ret = _gnix_get_ep_name(addr, i, &ep_name, av_priv->domain);

		/* check if this ep_name fits in the av context bits */
		if ((ret == FI_SUCCESS) &&
			(ep_name.name_type & GNIX_EPN_TYPE_SEP)) {
			if ((1 << av_priv->rx_ctx_bits) < ep_name.rx_ctx_cnt) {
				fprintf(stderr, "rx_ctx_bits %d ep.name.rx_ctx_cnt = %d\n", (1 << av_priv->rx_ctx_bits), ep_name.rx_ctx_cnt);
				ret = -FI_EINVAL;
				GNIX_DEBUG(FI_LOG_AV, "ep_name doesn't fit "
					"into the av context bits\n");
			}
		}

		if (ret != FI_SUCCESS) {
			if (flags & FI_SYNC_ERR) {
				entry_err[i] = ret;
				if (fi_addr)
					fi_addr[i] = FI_ADDR_NOTAVAIL;
				continue;
			} else {
				return -FI_EINVAL;
			}
		} else {
			if (flags & FI_SYNC_ERR)
				entry_err[i] = FI_SUCCESS;
			av_priv->table[index].gnix_addr = ep_name.gnix_addr;
			av_priv->valid_entry_vec[index] = 1;
			av_priv->table[index].name_type = ep_name.name_type;
			av_priv->table[index].cookie = ep_name.cookie;
			av_priv->table[index].rx_ctx_cnt = ep_name.rx_ctx_cnt;
			av_priv->table[index].cm_nic_cdm_id =
				ep_name.cm_nic_cdm_id;
			av_priv->table[index].key_offset = ep_name.key_offset;
			if (fi_addr)
				fi_addr[i] = index;
			success_cnt++;
		}

	}

	av_priv->count += success_cnt;
	ret = success_cnt;

	return ret;
}

/*
 * Currently only marks as 'not valid'. Should actually free memory.
 * If any of the given address fail to be removed (are already marked removed)
 * then the return value will be -FI_EINVAL.
 */
static int table_remove(struct gnix_fid_av *av_priv, fi_addr_t *fi_addr,
			size_t count, uint64_t flags)
{
	int ret = FI_SUCCESS;
	size_t index;
	size_t i;

	for (i = 0; i < count; i++) {
		index = (size_t) fi_addr[i];
		if (index < av_priv->count) {
			if (av_priv->valid_entry_vec[index] == 0) {
				ret = -FI_EINVAL;
				break;
			} else {
				av_priv->valid_entry_vec[index] = 0;
			}
		} else {
			ret = -FI_EINVAL;
			break;
		}
	}

	return ret;
}

/*
 * table_lookup(): Translate fi_addr_t to struct gnix_address.
 */
static int table_lookup(struct gnix_fid_av *av_priv, fi_addr_t fi_addr,
			struct gnix_av_addr_entry *entry_ptr)
{
	size_t index;
	struct gnix_av_addr_entry *entry = NULL;

	index = (size_t)fi_addr;
	if (index >= av_priv->count)
		return -FI_EINVAL;

	assert(av_priv->table);
	entry = &av_priv->table[index];

	if (entry == NULL)
		return -FI_EINVAL;

	if (av_priv->valid_entry_vec[index] == 0)
		return -FI_EINVAL;

	memcpy(entry_ptr, entry, sizeof(*entry));

	return FI_SUCCESS;
}

static int table_reverse_lookup(struct gnix_fid_av *av_priv,
				struct gnix_address gnix_addr,
				fi_addr_t *fi_addr)
{
	struct gnix_av_addr_entry *entry;
	int i;

	for (i = 0; i < av_priv->count; i++) {
		entry = &av_priv->table[i];
		/*
		 * for SEP endpoint entry we may have a delta in the cdm_id
		 * component of the address to process
		 */
		if ((entry->name_type & GNIX_EPN_TYPE_SEP) &&
		    (entry->gnix_addr.device_addr == gnix_addr.device_addr)) {
			int index = gnix_addr.cdm_id - entry->gnix_addr.cdm_id;

			if ((index >= 0) && (index < entry->rx_ctx_cnt)) {
				/* we have a match */
				*fi_addr = fi_rx_addr(i, index,
						      av_priv->rx_ctx_bits);
				return FI_SUCCESS;
			}
		} else if (GNIX_ADDR_EQUAL(entry->gnix_addr, gnix_addr)) {
			*fi_addr = i;
			return FI_SUCCESS;
		}
	}

	return -FI_ENOENT;
}

/*******************************************************************************
 * FI_AV_MAP specific implementations.
 ******************************************************************************/

static int map_insert(struct gnix_fid_av *av_priv, const void *addr,
		      size_t count, fi_addr_t *fi_addr, uint64_t flags,
		      void *context)
{
	int ret;
	struct gnix_ep_name ep_name;
	struct gnix_av_addr_entry *the_entry;
	gnix_ht_key_t key;
	size_t i;
	struct gnix_av_block *blk = NULL;
	int ret_cnt = count;
	int *entry_err = context;

	assert(av_priv->map_ht != NULL);

	if (count == 0)
		return 0;

	blk = calloc(1, sizeof(struct gnix_av_block));
	if (blk == NULL)
		return -FI_ENOMEM;

	blk->base =  calloc(count, sizeof(struct gnix_av_addr_entry));
	if (blk->base == NULL) {
		free(blk);
		return -FI_ENOMEM;
	}

	slist_insert_tail(&blk->slist, &av_priv->block_list);

	for (i = 0; i < count; i++) {
		ret = _gnix_get_ep_name(addr, i, &ep_name, av_priv->domain);
		if (ret != FI_SUCCESS) {
			if (flags & FI_SYNC_ERR) {
				entry_err[i] = -FI_EINVAL;
				fi_addr[i] = FI_ADDR_NOTAVAIL;
				ret_cnt = -FI_EINVAL;
				continue;
			} else {
				return ret;
			}
		}

		/* check if this ep_name fits in the av context bits */
		if (ep_name.name_type & GNIX_EPN_TYPE_SEP) {
			if ((1 << av_priv->rx_ctx_bits) < ep_name.rx_ctx_cnt) {
				if (flags & FI_SYNC_ERR) {
					entry_err[i] = -FI_EINVAL;
					fi_addr[i] = FI_ADDR_NOTAVAIL;
					ret_cnt = -FI_EINVAL;
					continue;
				}
				GNIX_DEBUG(FI_LOG_DEBUG, "ep_name doesn't fit "
					"into the av context bits\n");
				return -FI_EINVAL; /* TODO: should try to do
						      cleanup */
			}
		}

		((struct gnix_address *)fi_addr)[i] = ep_name.gnix_addr;
		the_entry =  &blk->base[i];
		memcpy(&the_entry->gnix_addr, &ep_name.gnix_addr,
		       sizeof(struct gnix_address));
		the_entry->name_type = ep_name.name_type;
		the_entry->cm_nic_cdm_id = ep_name.cm_nic_cdm_id;
		the_entry->cookie = ep_name.cookie;
		the_entry->rx_ctx_cnt = ep_name.rx_ctx_cnt;
		memcpy(&key, &ep_name.gnix_addr, sizeof(gnix_ht_key_t));
		ret = _gnix_ht_insert(av_priv->map_ht,
				      key,
				      the_entry);

		if (flags & FI_SYNC_ERR) {
			entry_err[i] = FI_SUCCESS;
		}

		/*
		 * we are okay with user trying to add more
		 * entries with same key.
		 */
		if ((ret != FI_SUCCESS) && (ret != -FI_ENOSPC)) {
			GNIX_WARN(FI_LOG_AV,
				  "_gnix_ht_insert failed %d\n",
				  ret);
			if (flags & FI_SYNC_ERR) {
				entry_err[i] = ret;
				fi_addr[i] = FI_ADDR_NOTAVAIL;
				ret_cnt = ret;
				continue;
			}
			return ret;
		}
	}

	return ret_cnt;
}

/*
 * TODO: slab should be freed when entries in the slab drop to zero,
 * or as an alternative, have a free list for slabs so they can be
 * reused if new fi_av_insert operations are performed.
 */
static int map_remove(struct gnix_fid_av *av_priv, fi_addr_t *fi_addr,
		      size_t count, uint64_t flags)
{
	int i,ret = FI_SUCCESS;
	struct gnix_av_addr_entry *the_entry = NULL;
	gnix_ht_key_t key;

	for (i = 0; i < count; i++) {

		key = *(gnix_ht_key_t *)&fi_addr[i];

		/*
		 * first see if we have this entry in the hash
		 * TODO: is there a race condition here for multi-threaded?
		 */

		the_entry = _gnix_ht_lookup(av_priv->map_ht, key);
		if (the_entry == NULL)
			return -FI_ENOENT;

		ret = _gnix_ht_remove(av_priv->map_ht, key);

	}

	return ret;
}

static int map_lookup(struct gnix_fid_av *av_priv, fi_addr_t fi_addr,
		      struct gnix_av_addr_entry *entry_ptr)
{
	gnix_ht_key_t *key = (gnix_ht_key_t *)&fi_addr;
	struct gnix_av_addr_entry *entry;

	entry = _gnix_ht_lookup(av_priv->map_ht, *key & av_priv->mask);
	if (entry == NULL)
		return -FI_ENOENT;

	memcpy(entry_ptr, entry, sizeof(*entry));

	return FI_SUCCESS;
}

static int map_reverse_lookup(struct gnix_fid_av *av_priv,
			      struct gnix_address gnix_addr,
			      fi_addr_t *fi_addr)
{
	GNIX_HASHTABLE_ITERATOR(av_priv->map_ht, iter);
	struct gnix_av_addr_entry *entry;
	fi_addr_t rx_addr;

	while ((entry = _gnix_ht_iterator_next(&iter))) {
		/*
		 * for SEP endpoint entry we may have a delta in the cdm_id
		 * component of the address to process
		 */
		if ((entry->name_type & GNIX_EPN_TYPE_SEP) &&
		    (entry->gnix_addr.device_addr == gnix_addr.device_addr)) {
			int index = gnix_addr.cdm_id - entry->gnix_addr.cdm_id;

			if ((index >= 0) && (index < entry->rx_ctx_cnt)) {
				/* we have a match */
				memcpy(&rx_addr, &entry->gnix_addr,
					sizeof(fi_addr_t));
				*fi_addr = fi_rx_addr(rx_addr,
						      index,
						      av_priv->rx_ctx_bits);
				return FI_SUCCESS;
			}
		} else {
			if (GNIX_ADDR_EQUAL(entry->gnix_addr, gnix_addr)) {
				*fi_addr = GNIX_HASHTABLE_ITERATOR_KEY(iter);
				return FI_SUCCESS;
			}
		}
	}

	return -FI_ENOENT;
}

/*******************************************************************************
 * FI_AV API implementations.
 ******************************************************************************/
int _gnix_table_reverse_lookup(struct gnix_fid_av *av_priv,
			       struct gnix_address gnix_addr,
			       fi_addr_t *fi_addr)
{
	return table_reverse_lookup(av_priv, gnix_addr, fi_addr);
}

int _gnix_map_reverse_lookup(struct gnix_fid_av *av_priv,
			     struct gnix_address gnix_addr,
			     fi_addr_t *fi_addr)
{
	return map_reverse_lookup(av_priv, gnix_addr, fi_addr);
}

int _gnix_av_lookup(struct gnix_fid_av *gnix_av, fi_addr_t fi_addr,
		    struct gnix_av_addr_entry *entry_ptr)
{
	int ret = FI_SUCCESS;
	fi_addr_t addr = fi_addr & gnix_av->mask;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!gnix_av) {
		ret = -FI_EINVAL;
		goto err;
	}

	switch (gnix_av->type) {
	case FI_AV_TABLE:
		ret = table_lookup(gnix_av, addr, entry_ptr);
		break;
	case FI_AV_MAP:
		ret = map_lookup(gnix_av, addr, entry_ptr);
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

	if (fi_addr & ~gnix_av->mask) {
		entry_ptr->gnix_addr.cdm_id +=
				fi_addr >> (64 - gnix_av->rx_ctx_bits);
	}

err:
	return ret;
}

int _gnix_av_reverse_lookup(struct gnix_fid_av *gnix_av,
			    struct gnix_address gnix_addr,
			    fi_addr_t *fi_addr)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!gnix_av) {
		ret = -FI_EINVAL;
		goto err;
	}

	switch (gnix_av->type) {
	case FI_AV_TABLE:
		ret = table_reverse_lookup(gnix_av, gnix_addr, fi_addr);
		break;
	case FI_AV_MAP:
		ret = map_reverse_lookup(gnix_av, gnix_addr, fi_addr);
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

err:
	return ret;
}

/*
 * Note: this function (according to WG), is not intended to
 * typically be used in the critical path for messaging/rma/amo
 * requests
 */
DIRECT_FN STATIC int gnix_av_lookup(struct fid_av *av, fi_addr_t fi_addr,
				    void *addr, size_t *addrlen)
{
	struct gnix_fid_av *gnix_av;
	struct gnix_ep_name ep_name = { {0} };
	struct gnix_av_addr_entry entry;
	int ret;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!av || !addrlen)
		return -FI_EINVAL;

	gnix_av = container_of(av, struct gnix_fid_av, av_fid);

	if (gnix_av->domain->addr_format == FI_ADDR_STR) {
		if (*addrlen < GNIX_FI_ADDR_STR_LEN) {
			*addrlen = GNIX_FI_ADDR_STR_LEN;
			return -FI_ETOOSMALL;
		}
	} else {
		if (*addrlen < sizeof(ep_name)) {
			*addrlen = sizeof(ep_name);
			return -FI_ETOOSMALL;
		}
	}

	/*
	 * user better have provided a buffer since the
	 * value stored in addrlen is big enough to return ep_name
	 */

	if (!addr)
		return -FI_EINVAL;

	ret = _gnix_av_lookup(gnix_av, fi_addr, &entry);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_AV, "_gnix_av_lookup failed: %d\n", ret);
		return ret;
	}

	ep_name.gnix_addr = entry.gnix_addr;
	ep_name.name_type = entry.name_type;
	ep_name.cm_nic_cdm_id = entry.cm_nic_cdm_id;
	ep_name.cookie = entry.cookie;

	if (gnix_av->domain->addr_format == FI_ADDR_STR) {
		ret = _gnix_ep_name_to_str(&ep_name, (char **)&addr);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_AV, "_gnix_resolve_str_ep_name failed: %d %s\n",
				  ret, fi_strerror(-ret));
			return ret;
		}
		*addrlen = GNIX_FI_ADDR_STR_LEN;
	} else {
		memcpy(addr, (void *)&ep_name, MIN(*addrlen, sizeof(ep_name)));
		*addrlen = sizeof(ep_name);
	}

	return FI_SUCCESS;
}

DIRECT_FN STATIC int gnix_av_insert(struct fid_av *av, const void *addr,
				    size_t count, fi_addr_t *fi_addr,
				    uint64_t flags, void *context)
{
	struct gnix_fid_av *av_priv = NULL;
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!av)
		return -FI_EINVAL;

	av_priv = container_of(av, struct gnix_fid_av, av_fid);

	if (!av_priv)
		return -FI_EINVAL;

	if ((av_priv->type == FI_AV_MAP) && (fi_addr == NULL))
		return -FI_EINVAL;

	if ((flags & FI_SYNC_ERR) && (context == NULL)) {
		GNIX_WARN(FI_LOG_AV, "FI_SYNC_ERR requires context\n");
		return -FI_EINVAL;
	}

	switch (av_priv->type) {
	case FI_AV_TABLE:
		ret =
		    table_insert(av_priv, addr, count, fi_addr, flags, context);
		break;
	case FI_AV_MAP:
		ret = map_insert(av_priv, addr, count, fi_addr, flags, context);
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

	return ret;
}

DIRECT_FN STATIC int gnix_av_insertsvc(struct fid_av *av, const char *node,
				       const char *service, fi_addr_t *fi_addr,
				       uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

DIRECT_FN STATIC int gnix_av_insertsym(struct fid_av *av, const char *node,
				       size_t nodecnt, const char *service,
				       size_t svccnt, fi_addr_t *fi_addr,
				       uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

DIRECT_FN STATIC int gnix_av_remove(struct fid_av *av, fi_addr_t *fi_addr,
				    size_t count, uint64_t flags)
{
	struct gnix_fid_av *av_priv = NULL;
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!av) {
		ret = -FI_EINVAL;
		goto err;
	}

	av_priv = container_of(av, struct gnix_fid_av, av_fid);

	if (!av_priv) {
		ret = -FI_EINVAL;
		goto err;
	}

	switch (av_priv->type) {
	case FI_AV_TABLE:
		ret = table_remove(av_priv, fi_addr, count, flags);
		break;
	case FI_AV_MAP:
		ret = map_remove(av_priv, fi_addr, count, flags);
		break;
	default:
		ret = -FI_EINVAL;
		break;
	}

err:
	return ret;
}

/*
 * Given an address pointed to by addr, stuff a string into buf representing:
 * device_addr:cdm_id:name_type:cm_nic_cdm_id:cookie
 * where device_addr, cdm_id, cm_nic_cdm_id and cookie are represented in
 * hexadecimal. And name_type is represented as an integer.
 */
DIRECT_FN const char *gnix_av_straddr(struct fid_av *av,
		const void *addr, char *buf,
		size_t *len)
{
	char int_buf[GNIX_AV_MAX_STR_ADDR_LEN];
	int size;
	struct gnix_ep_name ep_name;
	struct gnix_fid_av *av_priv;

	if (!av || !addr || !buf || !len) {
		GNIX_DEBUG(FI_LOG_DEBUG, "NULL parameter in gnix_av_straddr\n");
		return NULL;
	}

	av_priv = container_of(av, struct gnix_fid_av, av_fid);

	if (av_priv->domain->addr_format == FI_ADDR_STR)
		_gnix_resolve_str_ep_name(addr, 0, &ep_name);
	else
		ep_name = ((struct gnix_ep_name *) addr)[0];

	/*
	 * if additional information is added to this string, then
	 * you will need to update in gnix.h:
	 *   GNIX_AV_STR_ADDR_VERSION, increment this value
	 *   GNIX_AV_MAX_STR_ADDR_LEN, to be the number of characters printed
	 */
	size = snprintf(int_buf, sizeof(int_buf), "%04i:0x%08" PRIx32 ":0x%08"
			PRIx32 ":%02i:0x%06" PRIx32 ":0x%08" PRIx32
			":%02i", GNIX_AV_STR_ADDR_VERSION,
			ep_name.gnix_addr.device_addr,
			ep_name.gnix_addr.cdm_id,
			ep_name.name_type,
			ep_name.cm_nic_cdm_id,
			ep_name.cookie,
			ep_name.rx_ctx_cnt);

	/*
	 * snprintf returns the number of character written
	 * without the terminating character.
	 */
	if ((size + 1) < *len) {
		/*
		 * size needs to be all the characters plus the terminating
		 * character.  Otherwise, we could lose information.
		 */
		size = size + 1;
	} else {
		/* Do not overwrite the buffer. */
		size = *len;
	}

	snprintf(buf, size, "%s", int_buf);
	*len = size;

	return buf;
}

static void __av_destruct(void *obj)
{
	int ret;
	struct gnix_fid_av *av = (struct gnix_fid_av *) obj;
	struct slist_entry *blk_entry;
	struct gnix_av_block *temp;


	if (av->type == FI_AV_TABLE) {
		if (av->table) {
			free(av->table);
		}
	} else if (av->type == FI_AV_MAP) {

		while (!slist_empty(&av->block_list)) {
			blk_entry = slist_remove_head(&av->block_list);
			temp = container_of(blk_entry,
					struct gnix_av_block, slist);
			free(temp->base);
			free(temp);
		}

		ret = _gnix_ht_destroy(av->map_ht);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_AV,
				  "_gnix_ht_destroy failed %d\n",
				  ret);
		free(av->map_ht);
	}
	if (av->valid_entry_vec) {
		free(av->valid_entry_vec);
	} else {
		GNIX_WARN(FI_LOG_AV, "valid_entry_vec is NULL\n");
	}

	free(av);
}

static int gnix_av_close(fid_t fid)
{
	struct gnix_fid_av *av = NULL;
	int ret = FI_SUCCESS;
	int references_held;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!fid) {
		ret = -FI_EINVAL;
		goto err;
	}
	av = container_of(fid, struct gnix_fid_av, av_fid.fid);

	references_held = _gnix_ref_put(av);
	if (references_held) {
		GNIX_INFO(FI_LOG_AV, "failed to fully close av due to lingering "
				"references. references=%i av=%p\n",
				references_held, av);
	}

err:
	return ret;
}

DIRECT_FN int gnix_av_bind(struct fid_av *av, struct fid *fid, uint64_t flags)
{
	return -FI_ENOSYS;
}

/*
 * TODO: Support shared named AVs.
 */
DIRECT_FN int gnix_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
			   struct fid_av **av, void *context)
{
	struct gnix_fid_domain *int_dom = NULL;
	struct gnix_fid_av *av_priv = NULL;
	struct gnix_hashtable_attr ht_attr;

	enum fi_av_type type = FI_AV_TABLE;
	size_t count = 128;
	int rx_ctx_bits = 0;
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_AV, "\n");

	if (!domain) {
		ret = -FI_EINVAL;
		goto err;
	}

	int_dom = container_of(domain, struct gnix_fid_domain, domain_fid);
	if (!int_dom) {
		ret = -FI_EINVAL;
		goto err;
	}

	av_priv = calloc(1, sizeof(*av_priv));
	if (!av_priv) {
		ret = -FI_ENOMEM;
		goto err;
	}

	if (attr) {
		ret = gnix_verify_av_attr(attr);
		if (ret) {
			goto cleanup;
		}

		if (attr->type != FI_AV_UNSPEC) {
			type = attr->type;
		}
		count = attr->count;
		rx_ctx_bits = attr->rx_ctx_bits;
	}

	av_priv->domain = int_dom;
	av_priv->type = type;
	av_priv->addrlen = sizeof(struct gnix_address);
	av_priv->rx_ctx_bits = rx_ctx_bits;
	av_priv->mask = rx_ctx_bits ?
			((uint64_t)1 << (64 - attr->rx_ctx_bits)) - 1 : ~0;

	av_priv->capacity = count;
	if (type == FI_AV_TABLE) {
		av_priv->table = calloc(count,
				       sizeof(struct gnix_av_addr_entry));
		if (!av_priv->table) {
			ret = -FI_ENOMEM;
			goto cleanup;
		}
	}

	av_priv->valid_entry_vec = calloc(count, sizeof(int));
	if (!av_priv->valid_entry_vec) {
		ret = -FI_ENOMEM;
		goto cleanup;
	}

	av_priv->av_fid.fid.fclass = FI_CLASS_AV;
	av_priv->av_fid.fid.context = context;
	av_priv->av_fid.fid.ops = &gnix_fi_av_ops;
	av_priv->av_fid.ops = &gnix_av_ops;

	if (type == FI_AV_MAP) {
		av_priv->map_ht = calloc(1, sizeof(struct gnix_hashtable));
		if (av_priv->map_ht == NULL)
			goto cleanup;

		/*
		 * use same parameters as used for ep vc hash
		 */

		ht_attr.ht_initial_size = int_dom->params.ct_init_size;
		ht_attr.ht_maximum_size = int_dom->params.ct_max_size;
		ht_attr.ht_increase_step = int_dom->params.ct_step;
		ht_attr.ht_increase_type = GNIX_HT_INCREASE_MULT;
		ht_attr.ht_collision_thresh = 500;
		ht_attr.ht_hash_seed = 0xdeadbeefbeefdead;
		ht_attr.ht_internal_locking = 1;
		ht_attr.destructor = NULL;

		ret = _gnix_ht_init(av_priv->map_ht,
				    &ht_attr);
		slist_init(&av_priv->block_list);
	}
	_gnix_ref_init(&av_priv->ref_cnt, 1, __av_destruct);

	*av = &av_priv->av_fid;

	return ret;

cleanup:
	if (av_priv->table != NULL)
		free(av_priv->table);
	if (av_priv->valid_entry_vec != NULL)
		free(av_priv->valid_entry_vec);
	free(av_priv);
err:
	return ret;
}

/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/
static struct fi_ops_av gnix_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = gnix_av_insert,
	.insertsvc = gnix_av_insertsvc,
	.insertsym = gnix_av_insertsym,
	.remove = gnix_av_remove,
	.lookup = gnix_av_lookup,
	.straddr = gnix_av_straddr
};

static struct fi_ops gnix_fi_av_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open
};
