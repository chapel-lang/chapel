/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
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

#include "rdma/fabric.h"
#include "rdma/fi_errno.h"
#include "fi_ext_gni.h"

#include "gnix_auth_key.h"
#include "gnix_hashtable.h"
#include "gnix_shmem.h"
#include "gnix_bitmap.h"
#include "gnix.h"

#define GNIX_AUTH_KEY_HASHSEED 0xdeadbeef

#define GAI_VERSION(major, minor) (((major) << 16) + (minor))
typedef enum gnix_global_auth_info_version {
	GNIX_GAI_VERSION_1 = GAI_VERSION(1, 0), /* initial version */
} gnix_global_auth_info_version_t;

#define MAX_VMDH_TAGS 4
#define MAX_VMDH_REGS 4096
#define BITMAP_ARR_SIZE(nbits) \
	(GNIX_BITMAP_BLOCKS(nbits) * sizeof(gnix_bitmap_block_t))

#define GNIX_DEFAULT_AK_DIR "/tmp"
#define GNIX_DEFAULT_AK_FILENAME "gnix_vmdh_info"
#define GNIX_DEFAULT_AK_PATH \
	GNIX_DEFAULT_AK_DIR "/" GNIX_DEFAULT_AK_FILENAME
#define GNIX_AK_PATH_LEN 256

static char *gnix_default_ak_path = GNIX_DEFAULT_AK_PATH;

uint8_t* gnix_default_auth_key = NULL;

struct gnix_global_ptag_info {
	gnix_bitmap_t prov;
	gnix_bitmap_t user;
	uint8_t arr[BITMAP_ARR_SIZE(MAX_VMDH_REGS)];
};

struct gnix_global_vmdh_info {
	uint32_t version;
	uint32_t size;
	uint32_t table_size;
	fastlock_t lock;
	int ptag_index[MAX_VMDH_TAGS];
	struct gnix_global_ptag_info info[MAX_VMDH_TAGS];
};

/* Global data storage for authorization key information */
/* gnix_vmdh_info is the shared memory synchronization area for ptag info */
static struct gnix_shared_memory __gnix_shmem_region;
static struct gnix_global_vmdh_info *__gnix_vmdh_info;
gnix_hashtable_t __gnix_auth_key_ht;

static int __gnix_global_vmdh_info_init(const char *path,
	uint32_t size, void *buffer)
{
	struct gnix_global_vmdh_info *info =
		(struct gnix_global_vmdh_info *) buffer;
	int i;

	memset(info, 0, sizeof(struct gnix_global_vmdh_info));

	info->version = GNIX_GAI_VERSION_1;
	info->size = size;
	info->table_size = _gnix_bitmap_get_buffer_size(MAX_VMDH_REGS);
	for (i = 0; i < MAX_VMDH_TAGS; i++)
		info->ptag_index[i] = -1;
	fastlock_init(&info->lock);

	return 0;
}

static int _gnix_open_vmdh_info_file(const char *path)
{
	int ret;

	if (!__gnix_vmdh_info) {
		ret = _gnix_shmem_create(path,
			sizeof(struct gnix_global_vmdh_info),
			__gnix_global_vmdh_info_init,
			&__gnix_shmem_region);
		if (ret)
			return ret;

		__gnix_vmdh_info = (struct gnix_global_vmdh_info *)
			__gnix_shmem_region.addr;
	}

	if (__gnix_vmdh_info->version != GNIX_GAI_VERSION_1)
		GNIX_FATAL(FI_LOG_FABRIC,
			"failed to find compatible version of "
			"vmdh information file, expected=%x actual=%x\n",
			GNIX_GAI_VERSION_1, __gnix_vmdh_info->version);

	return 0;
}
int _gnix_get_next_reserved_key(struct gnix_auth_key *info)
{
	int reserved_key;
	int offset = info->attr.user_key_limit;
	int retry_limit = 10; /* randomly picked */
	int ret;

	if (!info) {
		GNIX_WARN(FI_LOG_MR, "bad authorization key, key=%p\n",
			info);
		return -FI_EINVAL;
	}

	do {
		reserved_key = _gnix_find_first_zero_bit(info->prov);
		if (reserved_key >= 0) {
			ret = _gnix_test_and_set_bit(info->prov, reserved_key);
			if (ret)
				reserved_key = -FI_EAGAIN;
		}
		retry_limit--;
	} while (reserved_key < 0 && retry_limit > 0);

	ret = (reserved_key < 0) ? reserved_key : (offset + reserved_key);

	GNIX_INFO(FI_LOG_DOMAIN, "returning key=%d offset=%d\n", ret, offset);

	return ret;
}

int _gnix_release_reserved_key(struct gnix_auth_key *info, int reserved_key)
{
	int offset = info->attr.user_key_limit;
	int ret;

	if (!info || reserved_key < 0) {
		GNIX_WARN(FI_LOG_MR, "bad authorization key or reserved key,"
			" auth_key=%p requested_key=%d\n",
			info, reserved_key);
		return -FI_EINVAL;
	}

	ret = _gnix_test_and_clear_bit(info->prov, reserved_key - offset);
	assert(ret == 1);

	return (ret == 1) ? FI_SUCCESS : -FI_EBUSY;
}

static inline int __gnix_auth_key_enable_vmdh(struct gnix_auth_key *info)
{
	int i, ret;
	void *buffer;

	fastlock_acquire(&__gnix_vmdh_info->lock);
	/* Find ptag in node-local info structure */
	for (i = 0; i < MAX_VMDH_TAGS; i++)
		if (__gnix_vmdh_info->ptag_index[i] == info->ptag)
			break;

	if (i == MAX_VMDH_TAGS) { /* didn't find it */
		/* find first empty region */
		for (i = 0; i < MAX_VMDH_TAGS; i++)
			if (__gnix_vmdh_info->ptag_index[i] == -1)
				break;

		/* if no space ... */
		if (i == MAX_VMDH_TAGS) {
			fastlock_release(&__gnix_vmdh_info->lock);
			GNIX_WARN(FI_LOG_FABRIC,
				"application is attempting to use too many keys "
				"with scalable memory registration, "
				"ret=-FI_ENOSPC\n");
			return -FI_ENOSPC;
		}

		/* set index entry to ptag ID */
		__gnix_vmdh_info->ptag_index[i] = info->ptag;

		/* setup provider key space */
		buffer = (void *) __gnix_vmdh_info->info[i].arr;
		ret = _gnix_alloc_bitmap(&__gnix_vmdh_info->info[i].prov,
			info->attr.prov_key_limit, buffer);
		if (ret) {
			fastlock_release(&__gnix_vmdh_info->lock);
			GNIX_WARN(FI_LOG_FABRIC,
				"failed to allocate bitmap on mmap backed page, ret=%d\n",
				ret);
			return ret;
		}

		/* advance buffer and setup user key space */
		buffer = (void *) ((uint64_t) (buffer) +
			 _gnix_bitmap_get_buffer_size(info->attr.prov_key_limit));

		ret = _gnix_alloc_bitmap(&__gnix_vmdh_info->info[i].user,
			info->attr.user_key_limit, buffer);
		if (ret) {
			fastlock_release(&__gnix_vmdh_info->lock);
			GNIX_WARN(FI_LOG_FABRIC,
				"failed to allocate bitmap on mmap backed page, ret=%d\n",
				ret);
			return ret;
		}

		GNIX_INFO(FI_LOG_FABRIC,
			"set resource limits: pkey=%08x ptag=%d "
			"reserved=%d registration_limit=%d "
			"reserved_keys=%d-%d\n",
			info->cookie,
			info->ptag,
			info->attr.prov_key_limit,
			info->attr.user_key_limit,
			info->attr.user_key_limit,
			(info->attr.prov_key_limit +
			info->attr.user_key_limit - 1));
	}
	info->prov = &__gnix_vmdh_info->info[i].prov;
	info->user = &__gnix_vmdh_info->info[i].user;
	fastlock_release(&__gnix_vmdh_info->lock);

	return FI_SUCCESS;
}

int _gnix_auth_key_enable(struct gnix_auth_key *info)
{
	int ret = -FI_EBUSY;
	uint32_t pes_on_node;
	int logical_rank;

	if (!info) {
		GNIX_WARN(FI_LOG_MR, "bad authorization key, key=%p\n",
			info);
		return -FI_EINVAL;
	}

	fastlock_acquire(&info->lock);
	if (!info->enabled) {
		info->enabled = 1;

		if (info->using_vmdh) {
			if (!__gnix_vmdh_info) {
				char *dir = getenv("TMPDIR");
				char *filename = getenv("GNIX_AK_FILENAME");
				char path[GNIX_AK_PATH_LEN];
				int sz, i;

				if (dir || filename) {
					GNIX_DEBUG(FI_LOG_FABRIC,
						"using non-default directory or file name, "
						"errors may occur\n");
					if (!dir)
						sz = snprintf(path, GNIX_AK_PATH_LEN,
							"%s/", GNIX_DEFAULT_AK_DIR);
					else
						sz = snprintf(path, GNIX_AK_PATH_LEN,
							"%s/", dir);

					if (sz < 0)
						return -FI_EINVAL;

					i = sz;
					if (!filename)
						sz = snprintf(&path[i], GNIX_AK_PATH_LEN - i,
							"%s", GNIX_DEFAULT_AK_FILENAME);
					else
						sz = snprintf(&path[i], GNIX_AK_PATH_LEN - i,
							"%s", filename);

					if (sz < 0)
						return -FI_EINVAL;
					sz += i;
				} else {
					sz = snprintf(path, GNIX_AK_PATH_LEN, "%s",
						gnix_default_ak_path);
				}

				path[(sz == GNIX_AK_PATH_LEN) ?
					GNIX_AK_PATH_LEN : sz + 1] = '\0';
				if (sz == GNIX_AK_PATH_LEN) {
					GNIX_WARN(FI_LOG_FABRIC,
						"file path maximum length exceeded, "
						"truncating name to 256 characters path=%s\n",
						path);
				}

				GNIX_INFO(FI_LOG_FABRIC,
					"opening auth key file at %s\n", path);

				ret = _gnix_open_vmdh_info_file(path);
				if (ret) {
					info->enabled = 0;
					fastlock_release(&info->lock);
					return ret;
				}
			}

			ret = __gnix_auth_key_enable_vmdh(info);
			if (ret) {
				info->enabled = 0;
				fastlock_release(&info->lock);
				return ret;
			}

			ret = _gnix_pes_on_node(&pes_on_node);
			if (ret)
				GNIX_FATAL(FI_LOG_DOMAIN,
					"failed to get count of pes on node, rc=%d\n", ret);

			ret = _gnix_pe_node_rank(&logical_rank);
			if (ret)
				GNIX_FATAL(FI_LOG_DOMAIN,
					"failed to get logical node rank, rc=%d\n", ret);

			info->key_partition_size = info->attr.user_key_limit / pes_on_node;
			info->key_offset = logical_rank * info->key_partition_size;
		}
		GNIX_INFO(FI_LOG_DOMAIN,
			"pkey=%08x ptag=%d key_partition_size=%d key_offset=%d enabled\n",
			info->cookie, info->ptag, info->key_partition_size, info->key_offset);
		ret = FI_SUCCESS;
	}
	fastlock_release(&info->lock);

	if (ret == -FI_EBUSY) {
		GNIX_DEBUG(FI_LOG_MR, "authorization key already enabled, "
			"auth_key=%p\n", info);
	}

	return ret;
}

struct gnix_auth_key *_gnix_auth_key_alloc()
{
	struct gnix_auth_key *auth_key = NULL;

	auth_key = calloc(1, sizeof(*auth_key));
	if (auth_key) {
		fastlock_init(&auth_key->lock);
	} else {
		GNIX_WARN(FI_LOG_MR, "failed to allocate memory for "
			"authorization key\n");
	}

	return auth_key;
}

int _gnix_auth_key_insert(
		uint8_t *auth_key,
		size_t auth_key_size,
		struct gnix_auth_key *to_insert)
{
	int ret;
	gnix_ht_key_t key;
	struct fi_gni_auth_key *gni_auth_key =
		(struct fi_gni_auth_key *) auth_key;

	if (!to_insert) {
		GNIX_WARN(FI_LOG_MR, "bad parameters, to_insert=%p\n",
			to_insert);
		return -FI_EINVAL;
	}

	if (!auth_key) {
		GNIX_INFO(FI_LOG_FABRIC, "auth key is null\n");
		return -FI_EINVAL;
	}

	switch (gni_auth_key->type) {
	case GNIX_AKT_RAW:
		key = (gnix_ht_key_t) gni_auth_key->raw.protection_key;
		break;
	default:
		GNIX_INFO(FI_LOG_FABRIC, "unrecognized auth key "
			"type, type=%d\n",
			gni_auth_key->type);
		return -FI_EINVAL;
	}

	ret = _gnix_ht_insert(&__gnix_auth_key_ht, key, to_insert);
	if (ret) {
		GNIX_WARN(FI_LOG_MR, "failed to insert entry, ret=%d\n",
			ret);
	}

	return ret;
}

int _gnix_auth_key_free(struct gnix_auth_key *key)
{
	if (!key) {
		GNIX_WARN(FI_LOG_MR, "bad parameters, key=%p\n", key);
		return -FI_EINVAL;
	}

	fastlock_destroy(&key->lock);

	key->enabled = 0;

	free(key);

	return FI_SUCCESS;
}

struct gnix_auth_key *
_gnix_auth_key_lookup(uint8_t *auth_key, size_t auth_key_size)
{
	gnix_ht_key_t key;
	struct gnix_auth_key *ptr = NULL;
	struct fi_gni_auth_key *gni_auth_key = NULL;

	if (auth_key == NULL || auth_key_size == 0) {
		auth_key = gnix_default_auth_key;
	}

	gni_auth_key = (struct fi_gni_auth_key *) auth_key;
	switch (gni_auth_key->type) {
	case GNIX_AKT_RAW:
		key = (gnix_ht_key_t) gni_auth_key->raw.protection_key;
		break;
	default:
		GNIX_INFO(FI_LOG_FABRIC, "unrecognized auth key type, "
			"type=%d\n", gni_auth_key->type);
		return NULL;
	}

	ptr = (struct gnix_auth_key *) _gnix_ht_lookup(
		&__gnix_auth_key_ht, key);

	return ptr;
}

int _gnix_auth_key_subsys_init(void)
{
	int ret = FI_SUCCESS;

	gnix_hashtable_attr_t attr = {
			.ht_initial_size     = 8,
			.ht_maximum_size     = 256,
			.ht_increase_step    = 2,
			.ht_increase_type    = GNIX_HT_INCREASE_MULT,
			.ht_collision_thresh = 400,
			.ht_hash_seed        = 0xcafed00d,
			.ht_internal_locking = 1,
			.destructor          = NULL
	};

	ret = _gnix_ht_init(&__gnix_auth_key_ht, &attr);
	assert(ret == FI_SUCCESS);

	struct fi_gni_auth_key *gni_auth_key = calloc(1, sizeof(*gni_auth_key));
	gni_auth_key->type = GNIX_AKT_RAW;
	gni_auth_key->raw.protection_key = 0;
	gnix_default_auth_key = (uint8_t *) gni_auth_key;

	return ret;
}

int _gnix_auth_key_subsys_fini(void)
{
	free(gnix_default_auth_key);

	return FI_SUCCESS;
}

struct gnix_auth_key *_gnix_auth_key_create(
		uint8_t *auth_key,
		size_t auth_key_size)
{
	struct gnix_auth_key *to_insert;
	struct fi_gni_auth_key *gni_auth_key;
	int ret;
	gni_return_t grc;
	uint8_t ptag;
	uint32_t cookie;

	if (auth_key == NULL || auth_key_size == 0) {
		auth_key = gnix_default_auth_key;
	}

	gni_auth_key = (struct fi_gni_auth_key *) auth_key;
	if (auth_key == gnix_default_auth_key) {
		gnixu_get_rdma_credentials(NULL, &ptag, &cookie);
		gni_auth_key->raw.protection_key = cookie;
	} else {
		switch (gni_auth_key->type) {
		case GNIX_AKT_RAW:
			cookie = gni_auth_key->raw.protection_key;
			break;
		default:
			GNIX_WARN(FI_LOG_FABRIC,
				"unrecognized auth key type, type=%d\n",
				gni_auth_key->type);
			return NULL;
		}

		grc = GNI_GetPtag(0, cookie, &ptag);
		if (grc) {
			GNIX_WARN(FI_LOG_FABRIC,
				"could not retrieve ptag, "
				"cookie=%d ret=%d\n", cookie, grc);
			return NULL;
		}
	}

	to_insert = _gnix_auth_key_alloc();
	if (!to_insert) {
		GNIX_WARN(FI_LOG_MR, "failed to allocate memory for "
			"auth key\n");
		return NULL;
	}

	to_insert->attr.prov_key_limit = gnix_default_prov_registration_limit;
	to_insert->attr.user_key_limit = gnix_default_user_registration_limit;
	to_insert->ptag = ptag;
	to_insert->cookie = cookie;

	ret = _gnix_auth_key_insert(auth_key, auth_key_size, to_insert);
	if (ret) {
		GNIX_INFO(FI_LOG_MR, "failed to insert authorization key, "
			"key=%p len=%d to_insert=%p ret=%d\n",
			auth_key, auth_key_size, to_insert, ret);
		_gnix_auth_key_free(to_insert);
		to_insert = NULL;
	}

	return to_insert;
}
