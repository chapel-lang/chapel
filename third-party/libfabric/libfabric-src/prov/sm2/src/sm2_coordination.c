/*
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "sm2_coordination.h"
#include "sm2.h"
#include "sm2_atom.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NEXT_MULTIPLE_OF(x, mod) x % mod ? ((x / mod) + 1) * mod : x
#define ZOMBIE_ALLOCATION_NAME	 "ZOMBIE"
#define SM2_COORDINATION_DIR	 "/dev/shm"
#define SM2_COORDINATION_FILE	 SM2_COORDINATION_DIR "/fi_sm2_mmaps"
#define SM2_STARTUP_MAX_TRIES	 1000

static void sm2_file_attempt_shrink(struct sm2_mmap *map);
int sm2_entry_lookup(const char *name, struct sm2_mmap *map);

/*
 * Sends signal 0 to the pid, if the call succeeds, it means the pid exists.
 * Signal 0 doesn't actually kill the process.
 */
static inline bool pid_lives(int pid)
{
	int err = kill(pid, 0);
	return err == 0;
}

static inline int
sm2_mmap_check_version(struct sm2_coord_file_header *tmp_header)
{
	if (tmp_header->file_version == SM2_VERSION)
		return 0;

	FI_WARN(&sm2_prov, FI_LOG_AV,
		"Cannot open the sm2 coordination file because the existing "
		"file with version (%d) is not compatible with this library's "
		"version (%d).\n Consider removing the existing file: %s\n",
		tmp_header->file_version, SM2_VERSION, SM2_COORDINATION_FILE);
	return -FI_EAVAIL;
}

/*
 * Take an open file, and mmap its contents
 */
void *sm2_mmap_map(int fd, struct sm2_mmap *map)
{
	struct stat st;

	if (fstat(fd, &st)) {
		FI_WARN(&sm2_prov, FI_LOG_AV, "Failed fstat");
		goto out;
	}

	map->base =
		mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map->base == MAP_FAILED) {
		FI_WARN(&sm2_prov, FI_LOG_AV, "Failed mmap, st.st_size=%ld\n",
			st.st_size);
		goto out;
	}
	map->fd = fd;
	map->size = st.st_size;
	return map->base;
out:
	map->base = NULL;
	return NULL;
}

/*
 * Take a sm2_mmap, and re-map if necessary, ensuring size of at_least
 *
 * If the size of the current map or the size of the file are not sufficient
 * to address "at_least" bytes, then the file will be truncated() (extended)
 * to the required size and the memory munmap()ed and re mmap()'ed
 */
static inline int sm2_mmap_remap(struct sm2_mmap *map, size_t at_least)
{
	struct stat st;

	assert(at_least > 0);
	if (map->size >= at_least)
		return 0;

	if (fstat(map->fd, &st)) {
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Failed fstat of sm2_mmaps file: %s, at_least: %zu\n",
			strerror(errno), at_least);
		return -FI_EOTHER;
	}

	if (st.st_size < at_least) {
		if (ftruncate(map->fd, at_least)) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"Failed ftruncate of sm2_mmaps file: %s, "
				"at_least: %zu\n",
				strerror(errno), at_least);
			return -FI_ENOMEM;
		}
	} else if (st.st_size >= map->size) {
		/* file has been extended by another process since
		   last we checked.  Re-map the entire file. */
		at_least = st.st_size;
	} else {
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Shm file has shrunk, re-mapping!\n");
		at_least = st.st_size;
	}

	if (map->size != at_least) {
		if (munmap(map->base, map->size)) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"Failed unmap of sm2_mmaps file: %s, at_least: "
				"%zu\n",
				strerror(errno), at_least);
			return -FI_EOTHER;
			;
		}

		map->base = mmap(0, at_least, PROT_READ | PROT_WRITE,
				 MAP_SHARED, map->fd, 0);
		if (map->base == MAP_FAILED) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"Failed to remap sm2_maps file when increasing "
				"size to st.st_size=%ld, at_least, %zu, error: "
				"%s\n",
				st.st_size, at_least, strerror(errno));
			return -FI_ENOMEM;
		}
		map->size = at_least;
	}

	return 0;
}

ssize_t sm2_mmap_cleanup(struct sm2_mmap *map)
{
	int err1, err2;

	err1 = close(map->fd);
	if (err1)
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Failed to close fd %d with error code %d", map->fd,
			err1);

	err2 = munmap(map->base, map->size);
	if (err2)
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Failed unmap shared memory region with error code %d",
			err2);

	return err1 ? err1 : err2;
}

ssize_t sm2_file_open_or_create(struct sm2_mmap *map_shared)
{
	pthread_mutexattr_t att;
	struct sm2_mmap map_ours;
	static const int template_len = sizeof(SM2_COORDINATION_DIR) +
					sizeof("/fi_sm2_pid1234567_XXXXXX") + 1;
	char template[template_len];
	struct sm2_coord_file_header *header, *tmp_header;
	struct sm2_ep_allocation_entry *entries;
	int fd, common_fd, err, tries, item;
	bool have_file_lock = false;
	long int page_size;
	long int max_file_size;

	page_size = ofi_get_page_size();
	if (page_size <= 0) {
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Unable to determine page size %ld\n", page_size);
		return -FI_EINVAL;
	}
	sprintf(template, "%s/fi_sm2_pid%7d_XXXXXX", SM2_COORDINATION_DIR,
		getpid());

	/* Assume we are the first process here.
	 * Try to open a tmpfile file in the shm directory
	 */
	fd = mkostemp(template, O_RDWR);
	if (fd < 0)
		goto early_exit;
	err = ftruncate(fd, sizeof(*header));
	if (err)
		goto early_exit;
	header = sm2_mmap_map(fd, &map_ours);
	if (!header)
		goto early_exit;
	err = pthread_mutexattr_init(&att);
	if (err)
		goto early_exit;
	err = pthread_mutexattr_setpshared(&att, PTHREAD_PROCESS_SHARED);
	if (err)
		goto early_exit;
	err = pthread_mutex_init(&header->write_lock, &att);
	if (err)
		goto early_exit;
	sm2_file_lock(&map_ours);

	header->file_version = SM2_VERSION;
	header->ep_region_size = sm2_calculate_size_offsets(NULL, NULL);
	header->ep_allocation_offset = sizeof(*header);
	header->ep_regions_offset = header->ep_allocation_offset +
				    (SM2_MAX_UNIVERSE_SIZE * sizeof(*entries));
	header->ep_regions_offset =
		NEXT_MULTIPLE_OF(header->ep_regions_offset, page_size);

	/* Allocate enough space in the file for all our allocations, but no
	 * data exchange regions yet. We need to keep this allocation small b/c
	 * every rank will try this on startup.
	 */
	err = sm2_mmap_remap(&map_ours, header->ep_regions_offset);
	if (err)
		goto early_exit;

	header = (struct sm2_coord_file_header *) map_ours.base;
	entries = sm2_mmap_entries(&map_ours);
	for (item = 0; item < SM2_MAX_UNIVERSE_SIZE; item++)
		entries[item].pid = 0;

	/* Make sure the header is written before we link the file,
	 * flush file
	 */
	atomic_mb();

	tries = SM2_STARTUP_MAX_TRIES;
	do {
		/* create a hardlink to our file with the common name.
		 *  - on success: we hold the lock to a newly
		 *		  created coordination file.
		 *  - on failure: the file already exists, we should try opening
		 *  		  it.
		 */
		err = link(template, SM2_COORDINATION_FILE);
		if (0 == err) {
			/* We are using the memory we initialized.  Duplicate it
			 * and leave the map open. */
			memcpy(map_shared, &map_ours, sizeof(map_ours));
			have_file_lock = true;
			break;
		}

		common_fd = open(SM2_COORDINATION_FILE, O_RDWR);
		if (common_fd >= 0) {
			tmp_header = sm2_mmap_map(common_fd, map_shared);
			if (!tmp_header) {
				close(common_fd);
				continue;
			}
			err = sm2_mmap_check_version(tmp_header);
			if (err)
				break;
			assert(map_shared->size >= sizeof(*tmp_header));
			err = pthread_mutex_trylock(&tmp_header->write_lock);
			if (err == 0) {
				sm2_file_attempt_shrink(map_shared);
				sm2_mmap_cleanup(&map_ours);
				have_file_lock = true;
				break;
			}

			/* TODO Handle PID that dies while holding file lock */

			sm2_mmap_cleanup(map_shared);
		}
		/* we could not acquire the lock, sleep and try again. */
		/* TODO Consider reducing the amount of time we sleep here */
		usleep(10000);
	} while (tries-- > 0);

	unlink(template);

	if (!have_file_lock) {
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Failed to acquire the lock to the coordination "
			"file.\n");
		return -FI_EAVAIL;
	}

	/* Remap the file to be big enough to hold every region upfront.
	 * This is required to get rid of race conditions that may have
	 * previously occured when we tried to remap in sm2_av_insert(),
	 * sm2_fifo_send() or sm2_fifo_recv().
	 */
	header = (struct sm2_coord_file_header *) map_shared->base;
	max_file_size = header->ep_regions_offset +
			header->ep_region_size * SM2_MAX_UNIVERSE_SIZE;
	err = sm2_mmap_remap(map_shared, max_file_size);

	/* File we created either became the shared file, or got unlinked */
	sm2_file_unlock(map_shared);
	return 0;

early_exit:
	if (fd >= 0) {
		unlink(template);
		close(fd);
	}
	return -FI_ENOMEM;
}

/*
 * Insert the name into the ep_allocation array.  Requires the lock.
 *
 * Note: that because of speculative av_insert operations, we may need to
 * assign an index for an endpoint claimed by another peer.
 * When self == true, we will "own" the entry (entry.pid = getpid()).
 * When False, we set pid = -getpid(), allowing owner to claim later.
 */
ssize_t sm2_entry_allocate(const char *name, struct sm2_mmap *map,
			   sm2_gid_t *gid, bool self)
{
	struct sm2_ep_allocation_entry *entries;
	struct sm2_region *peer_region = NULL;
	int item, pid = getpid(), peer_pid;

	entries = sm2_mmap_entries(map);

retry_lookup:
	item = sm2_entry_lookup(name, map);
	if (item >= 0) {
		entries = sm2_mmap_entries(map);

		/* Check if it is dirty */
		if (entries[item].pid && !pid_lives(abs(entries[item].pid))) {
			peer_region = sm2_mmap_ep_region(map, item);
			if (!smr_freestack_isfull(sm2_freestack(peer_region))) {
				/* Region did not shut down properly, but other
				 * processes might be using it, make it a zombie
				 * region - never use this region for as long as
				 * the file exists */
				FI_WARN(&sm2_prov, FI_LOG_AV,
					"Found region at allocation[%d] that "
					"did not  shut down correctly, marking "
					"it as a zombie never to be used again "
					"(until all active processes die, and "
					"file size is reset)!\n",
					item);
				strncpy(entries[item].ep_name,
					ZOMBIE_ALLOCATION_NAME, FI_NAME_MAX);
				goto retry_lookup;
			}
		}

		if (!self) {
			if (!pid_lives(abs(entries[item].pid))) {
				entries[item].pid = 0;
			}
			/* Someone else allocated the entry for us */
			goto found;
		}

		if (entries[item].pid <= 0) {
			if (!pid_lives(abs(entries[item].pid))) {
				FI_WARN(&sm2_prov, FI_LOG_AV,
					"During sm2 allocation of space for "
					"endpoint named %s pid %d "
					"pre-allocated space at allocation "
					"entry[%d] and then died!\n",
					name, -entries[item].pid, item);
			}
			goto found;
		}

		FI_WARN(&sm2_prov, FI_LOG_AV,
			"During sm2 allocation of space for endpoint named %s "
			"an existing conflicting address was found at "
			"allocation entry[%d]\n",
			name, item);

		if (!pid_lives(entries[item].pid)) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"The pid which allocated the conflicting "
				"allocation entry is "
				"dead. Reclaiming as our own.\n");
			/* it is possible that EP's referencing this region are
			 * still alive... don't know how to check (they likely
			 * died if PID died) */
			goto found;
		}

		FI_WARN(&sm2_prov, FI_LOG_AV,
			"ERROR: The endpoint (pid: %d) with conflicting "
			"address %s is still alive.\n",
			entries[item].pid, name);
		return -FI_EADDRINUSE;
	}

	/* fine, we could not find the entry, so now look for an empty slot */
	for (item = 0; item < SM2_MAX_UNIVERSE_SIZE; item++) {
		peer_pid = entries[item].pid;
		if (peer_pid == 0)
			goto found;
		if (peer_pid < 0)
			/* A third peer might have entered this address into
			 * their AV, and there is no current way to check
			 * this... need to keep this entry in the file until we
			 * clean up */
			continue;

		if (!pid_lives(peer_pid)) {
			entries = sm2_mmap_entries(map);
			struct sm2_region *peer_region =
				sm2_mmap_ep_region(map, item);

			if (entries[item].startup_ready &&
			    smr_freestack_isfull(sm2_freestack(peer_region))) {
				/* we found a slot with a dead PID and
				 * the freestack is full */
				entries[item].pid = 0;
				goto found;
			}
		}
	}

	FI_WARN(&sm2_prov, FI_LOG_AV,
		"No available entries were found in the coordination file, all "
		"%d were used\n",
		SM2_MAX_UNIVERSE_SIZE);
	return -FI_EAVAIL;

found:
	if (self) {
		entries[item].startup_ready = 0;
		atomic_wmb();
		entries[item].pid = pid;
	}

	if (!self && entries[item].pid == 0) {
		entries[item].startup_ready = 0;
		atomic_wmb();
		entries[item].pid = -pid;
	}

	FI_INFO(&sm2_prov, FI_LOG_AV,
		"Using sm2 region at allocation entry[%d] for %s\n", item,
		name);

	strncpy(entries[item].ep_name, name, FI_NAME_MAX - 1);
	entries[item].ep_name[FI_NAME_MAX - 1] = '\0';

	*gid = item;

	return 0;
}

int sm2_entry_lookup(const char *name, struct sm2_mmap *map)
{
	struct sm2_ep_allocation_entry *entries;
	int item;

	entries = sm2_mmap_entries(map);
	/* TODO Optimize this lookup*/
	for (item = 0; item < SM2_MAX_UNIVERSE_SIZE; item++) {
		if (0 == strncmp(name, entries[item].ep_name, FI_NAME_MAX)) {
			FI_DBG(&sm2_prov, FI_LOG_AV,
			       "Found existing %s in slot %d\n", name, item);
			return item;
		}
		if (entries[item].ep_name[0] != '\0') {
			FI_DBG(&sm2_prov, FI_LOG_AV,
			       "Searching for %s. Not yet found in spot "
			       "allocation[%d]=%s, PID=%d\n",
			       name, item, entries[item].ep_name,
			       entries[item].pid);
		} else if (!strcmp(entries[item].ep_name,
				   ZOMBIE_ALLOCATION_NAME)) {
			FI_DBG(&sm2_prov, FI_LOG_AV,
			       "Found zombie in spot allocation[%d], PID= %d\n",
			       item, entries[item].pid);
		}
	}
	return -1;
}

/*
 * Clear the pid for this entry.  must already hold lock.
 */
void sm2_entry_free(struct sm2_mmap *map, sm2_gid_t gid)
{
	struct sm2_ep_allocation_entry *entries;

	entries = sm2_mmap_entries(map);
	assert(entries[gid].pid == getpid());
	entries[gid].pid = 0;
}

void sm2_file_lock(struct sm2_mmap *map)
{
	struct sm2_coord_file_header *header = (void *) map->base;

	pthread_mutex_lock(&header->write_lock);
}

void sm2_file_unlock(struct sm2_mmap *map)
{
	struct sm2_coord_file_header *header = (void *) map->base;
	pthread_mutex_unlock(&header->write_lock);
}

/*
 * TODO combine this logic with the remap function while keeping remaps()'s
 * short circuit logic
 */
static void *sm2_mmap_shrink_to_size(struct sm2_mmap *map, size_t shrink_size)
{
	struct stat st;
	int err;

	err = fstat(map->fd, &st);
	if (err)
		goto out;

	if (st.st_size > shrink_size) {
		/* we need to shrink the file. */
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Shrinking SHM file to be of size %zu\n", shrink_size);
		err = ftruncate(map->fd, shrink_size);
		if (err)
			goto out;

		/* now un-map and re-map the file */
		err = munmap(map->base, map->size);
		if (err)
			goto out;
		map->base = mmap(0, shrink_size, PROT_READ | PROT_WRITE,
				 MAP_SHARED, map->fd, 0);
		if (map->base == MAP_FAILED) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"Failed to remap when decreasing the map size. "
				"st.st_size=%ld shrink_size=%ld\n",
				st.st_size, shrink_size);
			map->base = NULL;
		}
		map->size = shrink_size;
	}

out:
	if (err) {
		FI_WARN(&sm2_prov, FI_LOG_AV,
			"Failed syscall during sm2_mmap_shrink_to_index()\n");
	}
	return map->base;
}

/*
 * If everything in the file is dead, shrink it to fit 0 entries.
 * This deals with peer pre-allocated entries by making the assumption that
 * my PID must be alive in the file in order for me to hold any of my peers
 * allocations.
 *
 * NOTE: SHM file lock must be held before calling this function
 */
static void sm2_file_attempt_shrink(struct sm2_mmap *map)
{
	struct sm2_coord_file_header *header = (void *) map->base;
	struct sm2_ep_allocation_entry *entries = sm2_mmap_entries(map);
	int item;

	for (item = 0; item < SM2_MAX_UNIVERSE_SIZE; item++) {
		if (entries[item].pid != 0 &&
		    pid_lives(abs(entries[item].pid))) {
			FI_INFO(&sm2_prov, FI_LOG_AV,
				"Cannot shrink file b/c PID %d still lives",
				abs(entries[item].pid));
			return;
		}
	}

	memset(entries, 0, sizeof(*entries) * SM2_MAX_UNIVERSE_SIZE);
	sm2_mmap_shrink_to_size(map, header->ep_regions_offset);
}
