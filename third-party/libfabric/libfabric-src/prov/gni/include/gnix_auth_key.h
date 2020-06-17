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

#ifndef PROV_GNI_INCLUDE_GNIX_AUTH_KEY_H_
#define PROV_GNI_INCLUDE_GNIX_AUTH_KEY_H_

#include <ofi_lock.h>

#include "fi_ext_gni.h"
#include "gnix_bitmap.h"

/*
 * GNIX Authorization keys are directly associated with a specific GNI network
 * key. There are some limitations to GNI network keys that should be noted.
 *
 * GNI network keys are directly associated with memory registrations, and
 * can only support a single type of memory mode at a time. This means that
 * the memory mode must be tracked with the authorization key, and must exist
 * as globally known information. Since references to objects may still exist
 * after the fabric is closed, this information must persist unless the
 * application has promised not to open any more GNI fabric instances.
 * See fi_gni man page for guidance on GNI_DEALLOC_AKI_ON_FABRIC_CLOSE.
 */

/**
 * GNIX authorization key construct
 *
 * @var lock        lock for data structure
 * @var attr        authorization key attributes
 * @var enabled     Is this authorization key live? If so, refuse changes to limits
 * @var using_vmdh  Is this authorization key associated with a domain using
 *                  VMDH?
 * @var prov        bitmap for detecting provider key usage
 * @var user        bitmap for detecting user key usage
 */
struct gnix_auth_key {
	fastlock_t lock;
	struct gnix_auth_key_attr attr;
	int enabled;
	uint8_t ptag;
	uint32_t cookie;
	int using_vmdh;
	uint32_t key_partition_size;
	uint32_t key_offset;
	gnix_bitmap_t *prov;
	gnix_bitmap_t *user;
};

/**
 * Allocates an authorization key structure, if possible
 *
 * @return  non-NULL pointer to initialized memory on success
 *          NULL on failure
 */
struct gnix_auth_key *_gnix_auth_key_alloc();

/**
 * Frees an authorization key structure
 *
 * @param key    A GNI authorization key structure to free
 * @return       0 on success
 *               -FI_EINVAL, if invalid parameter passed as key
 */
int _gnix_auth_key_free(struct gnix_auth_key *key);

/**
 * Lookup an authorization key in global data storage
 *
 * @param auth_key     authorization key
 * @param auth_key_size  length of authorization key in bytes
 * @return             non-NULL pointer on success
 *                     NULL pointer if not found
 */
struct gnix_auth_key *_gnix_auth_key_lookup(
		uint8_t *auth_key,
		size_t auth_key_size);

/**
 * Enables and prevents further limit modifications for an authorization key
 *
 * @param key  GNI authorization key
 * @return     FI_SUCCESS on success
 *             -FI_EINVAL, if bad parameters were passed
 *             -FI_EBUSY, if already enabled
 */

int _gnix_auth_key_enable(struct gnix_auth_key *key);

/**
 * Retrieves the next available provider-reserved key for a given
 * authorization key
 *
 * @param info  A GNI authorization key
 * @return      FI_SUCCESS on success
 *              -FI_EINVAL, if bad parameters were passed
 *              -FI_EAGAIN, if no available key could be foundi
 */
int _gnix_get_next_reserved_key(struct gnix_auth_key *info);

/**
 * Releases a reserved key back to the bitset to be reused
 *
 * @param info          A GNI authorization key
 * @param reserved_key  index of the reserved key
 * @return              FI_SUCCESS on success
 *                      -FI_EINVAL, if invalid parameters were passed
 *                      -FI_EBUSY, if reserved key was already released
 */
int _gnix_release_reserved_key(struct gnix_auth_key *info, int reserved_key);

/**
 * Creates an authorization key from default configuration
 *
 * @param auth_key     authorization key
 * @param auth_key_size  length of authorization key in bytes
 * @return             non-NULL pointer on success
 *                     NULL pointer on failure
 */
struct gnix_auth_key *_gnix_auth_key_create(
		uint8_t *auth_key,
		size_t auth_key_size);

/**
 * Inserts an authorization key into global data storage
 *
 * @param auth_key     authorization key
 * @param auth_key_size  length of authorization key in bytes
 * @param to_insert    GNI authorization key structure to insert
 * @return             FI_SUCCESS on success
 *                     -FI_EINVAL, if to_insert is NULL or global data
 *                                 storage is destroyed
 *                     -FI_ENOSPC, if auth key exists in global data
 *                                 storage
 */
int _gnix_auth_key_insert(
		uint8_t *auth_key,
		size_t auth_key_size,
		struct gnix_auth_key *to_insert);

#define GNIX_GET_AUTH_KEY(auth_key, auth_key_size, requested_mode) \
	({ \
		struct gnix_auth_key *_tmp; \
		_tmp  = _gnix_auth_key_lookup((auth_key), (auth_key_size)); \
		int _tmp_ret; \
		if (!_tmp) { \
			GNIX_INFO(FI_LOG_FABRIC, \
				"failed to find authorization " \
				"key, creating new authorization key\n"); \
			_tmp = _gnix_auth_key_create( \
				(auth_key), (auth_key_size)); \
			if (!_tmp) { \
				GNIX_DEBUG(FI_LOG_FABRIC, \
					"failed to create new " \
					"authorization key, "\
					"another thread beat us to the insert " \
					"- searching again\n"); \
				_tmp = _gnix_auth_key_lookup((auth_key), \
					(auth_key_size)); \
				assert(_tmp); \
			} \
			_tmp->using_vmdh = (requested_mode); \
			_tmp_ret = _gnix_auth_key_enable(_tmp); \
			if (_tmp_ret) { \
				GNIX_WARN(FI_LOG_FABRIC, \
					"failed to enable new " \
					"authorization key\n"); \
			} \
		} \
		_tmp; \
	})

/* provider subsystem initialization and teardown functions */
int _gnix_auth_key_subsys_init(void);
int _gnix_auth_key_subsys_fini(void);

#endif /* PROV_GNI_INCLUDE_GNIX_AUTH_KEY_H_ */
