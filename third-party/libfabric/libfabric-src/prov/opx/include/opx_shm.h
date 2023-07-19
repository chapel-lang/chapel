/*
 * Copyright (c) 2016-2018 Intel Corporation. All rights reserved.
 * Copyright (c) 2022 Cornelis Networks.
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

/*  Code derived from Dmitry Vyukov */
/* see:  https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue */

/*  Multi-producer/multi-consumer bounded queue.
 *  Copyright (c) 2010-2011, Dmitry Vyukov. All rights reserved.
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *     1. Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list
 *        of conditions and the following disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *  THIS SOFTWARE IS PROVIDED BY DMITRY VYUKOV "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *  DMITRY VYUKOV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  The views and conclusions contained in the software and documentation are those of the authors and should not be interpreted
 *  as representing official policies, either expressed or implied, of Dmitry Vyukov.
 */

#ifndef _OPX_SHM_H_
#define _OPX_SHM_H_

#include <fcntl.h>
#include <limits.h>
#include <signal.h>

#define OPX_SHM_MAX_CONN_NUM 0xffff
#define OPX_SHM_SEGMENT_NAME_MAX_LENGTH (512)
#define OPX_SHM_TX_CONNECT_MAX_WAIT (5000)	// 5 seconds
#define OPX_SHM_SEGMENT_NAME_PREFIX "/opx.shm."

#define opx_shm_compiler_barrier() __asm__ __volatile__ ( "" ::: "memory" )
#define opx_shm_x86_pause()  __asm__ __volatile__ ( "pause"  )

struct opx_shm_connection {
	void	*segment_ptr;
	size_t	segment_size;
	bool	inuse;
	char segment_key[OPX_SHM_SEGMENT_NAME_MAX_LENGTH];
};

struct opx_shm_tx {
	struct opx_shm_fifo_segment	*fifo_segment[OPX_SHM_MAX_CONN_NUM];
	struct opx_shm_connection	connection[OPX_SHM_MAX_CONN_NUM];
	struct fi_provider		  	*prov;
	struct opx_shm_tx		  	*next; // for signal handler
	uint32_t				  	rank;
	uint32_t				  	rank_inst;
};

struct opx_shm_resynch {
	uint64_t	counter;
	bool		completed;
};

struct opx_shm_rx {
	struct opx_shm_fifo_segment	*fifo_segment;
	void						*segment_ptr;
	size_t				 		segment_size;
	char				 		segment_key[OPX_SHM_SEGMENT_NAME_MAX_LENGTH];
	struct fi_provider			*prov;
	struct opx_shm_rx   		*next; // for signal handler
	struct opx_shm_resynch 		resynch_connection[OPX_SHM_MAX_CONN_NUM];
};

extern struct opx_shm_tx *shm_tx_head;
extern struct opx_shm_rx *shm_rx_head;

struct opx_shm_packet
{
	ofi_atomic64_t sequence_;
	uint32_t		origin_rank;
	uint32_t		origin_rank_inst;
	uint64_t		pad;
	uint8_t			data[FI_OPX_SHM_PACKET_SIZE];
}__attribute__((__aligned__(64)));

struct opx_shm_fifo {
	ofi_atomic64_t              enqueue_pos_;
	char                        pad0_[FI_OPX_CACHE_LINE_SIZE];
	ofi_atomic64_t              dequeue_pos_;
	char                        pad1_[FI_OPX_CACHE_LINE_SIZE];
	struct opx_shm_packet       buffer_[FI_OPX_SHM_FIFO_SIZE];
} __attribute__((__aligned__(64)));

struct opx_shm_fifo_segment {
	ofi_atomic64_t              initialized_;
	char                        pad1_[FI_OPX_CACHE_LINE_SIZE];
	struct opx_shm_fifo			fifo;
} __attribute__((__aligned__(64)));

static inline
ssize_t opx_shm_rx_init (struct opx_shm_rx *rx,
		struct fi_provider *prov,
		const char * const unique_job_key,
		const unsigned rx_id,
		const unsigned fifo_size,
		const unsigned packet_size)
{
	__attribute__((__unused__)) int err = 0;
	int segment_fd = 0;
	void *segment_ptr = 0;

	rx->segment_ptr = NULL;
	rx->segment_size = 0;
	rx->prov = prov;

	for (int i = 0; i < OPX_SHM_MAX_CONN_NUM; ++i) {
		rx->resynch_connection[i].completed = false;
		rx->resynch_connection[i].counter = 0;
	}

	memset(rx->segment_key, 0, OPX_SHM_SEGMENT_NAME_MAX_LENGTH);

	snprintf(rx->segment_key, OPX_SHM_SEGMENT_NAME_MAX_LENGTH,
		OPX_SHM_SEGMENT_NAME_PREFIX "%s.%d",
		unique_job_key, rx_id);

	FI_LOG(prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
		"SHM creating of %u context Segment (%s)\n", rx_id, rx->segment_key);
	/* to ensure 64-byte alignment of fifo */
	size_t segment_size = sizeof(struct opx_shm_fifo_segment) + 64;

	if (shm_unlink(rx->segment_key) == 0) {
		FI_LOG(prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"cleaned up stale shared memory object (\"%s\")\n", rx->segment_key);
	}

	segment_fd = shm_open(rx->segment_key, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (segment_fd == -1) {
		FI_LOG(prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"Unable to create shm object '%s'; errno = '%s'\n",
			rx->segment_key, strerror(errno));
		err = errno;
		goto error_return;
	}

	errno = 0;
	if (ftruncate(segment_fd, segment_size) == -1) {
		FI_LOG(prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"Unable to set size of shm object '%s' to %zu; errno = '%s'\n",
			rx->segment_key, segment_size, strerror(errno));
		err = errno;
		goto error_return;
	}

	errno = 0;
	segment_ptr = mmap(NULL, segment_size, PROT_READ | PROT_WRITE,
			MAP_SHARED, segment_fd, 0);
	if (segment_ptr == MAP_FAILED) {
		FI_LOG(prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"mmap failed: '%s'\n", strerror(errno));
		err = errno;
		goto error_return;
	}

	memset(segment_ptr, 0, segment_size);

	rx->fifo_segment = (struct opx_shm_fifo_segment *)(((uintptr_t)segment_ptr + 64) & (~0x03Full));

	ofi_atomic_initialize64(&rx->fifo_segment->initialized_, 0);

	uint64_t buffer_size = FI_OPX_SHM_FIFO_SIZE;
	assert((buffer_size >= 2) && ((buffer_size & (buffer_size - 1)) == 0));
	for (size_t i = 0; i != buffer_size; i += 1) {
		ofi_atomic_initialize64(&rx->fifo_segment->fifo.buffer_[i].sequence_, i);
		ofi_atomic_set64(&rx->fifo_segment->fifo.buffer_[i].sequence_, i);
	}
	ofi_atomic_initialize64(&rx->fifo_segment->fifo.enqueue_pos_, 0);
	ofi_atomic_initialize64(&rx->fifo_segment->fifo.dequeue_pos_, 0);

	opx_shm_compiler_barrier();

	rx->segment_ptr = segment_ptr;
	rx->segment_size = segment_size;

	// TODO: MHEINZ we probably need a lock here.
	rx->next = shm_rx_head; shm_rx_head = rx; // add to signal handler list.

	ofi_atomic_set64(&rx->fifo_segment->initialized_, 1);
	
	close(segment_fd);	/* safe to close now */

	FI_LOG(prov, FI_LOG_INFO, FI_LOG_FABRIC,
		"SHM creation of %u context passed. Segment (%s)\n", rx_id, rx->segment_key);

	return FI_SUCCESS;

error_return:

	return -FI_EINVAL;
}

static inline
ssize_t opx_shm_rx_fini (struct opx_shm_rx *rx)
{
	if (rx->segment_ptr != NULL) {
		munmap(rx->segment_ptr, rx->segment_size);
		shm_unlink(rx->segment_key);

		return FI_SUCCESS;
	}

	return -FI_EINVAL;
}



static inline
ssize_t opx_shm_tx_init (struct opx_shm_tx *tx,
		struct fi_provider *prov,
		uint32_t hfi_rank,
		uint32_t hfi_rank_inst)
{
	int i = 0;
	for (i = 0; i < OPX_SHM_MAX_CONN_NUM; ++i) {
		tx->connection[i].segment_ptr = NULL;
		tx->connection[i].segment_size = 0;
		tx->connection[i].inuse = false;
		tx->fifo_segment[i] = NULL;
	}

	tx->prov = prov;
	tx->rank = hfi_rank;
	tx->rank_inst = hfi_rank_inst;

	// TODO: MHEINZ we probably need a lock here.
	tx->next = shm_tx_head; shm_tx_head = tx; // add to signal handler list.

	return FI_SUCCESS;
}

static inline
ssize_t opx_shm_tx_connect (struct opx_shm_tx *tx,
		const char * const unique_job_key,
		const unsigned rx_id,
		const unsigned fifo_size,
		const unsigned packet_size)
{
	int err = 0;

	char segment_key[OPX_SHM_SEGMENT_NAME_MAX_LENGTH];
	memset(segment_key, 0, OPX_SHM_SEGMENT_NAME_MAX_LENGTH);

	snprintf(segment_key, OPX_SHM_SEGMENT_NAME_MAX_LENGTH,
		OPX_SHM_SEGMENT_NAME_PREFIX "%s.%d",
		unique_job_key, rx_id);

	if (rx_id >= OPX_SHM_MAX_CONN_NUM) {
		FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"Unable to create shm object '%s'; rx %d too large\n",
			segment_key, rx_id);
		return -FI_E2BIG;
	}

	int segment_fd;
	unsigned loop = 0;
	for (;;) {
		segment_fd = shm_open(segment_key, O_RDWR, 0600);
		if (segment_fd == -1) {
			if (loop++ > OPX_SHM_TX_CONNECT_MAX_WAIT) {
				FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
					"Unable to create shm object '%s'; errno = '%s'\n",
					segment_key, strerror(errno));
				return -FI_EAGAIN;
			}
			usleep(1000);
		} else {
			break;
		}
	}

	size_t segment_size = sizeof(struct opx_shm_fifo_segment) + 64;

	void *segment_ptr = mmap(NULL, segment_size, PROT_READ | PROT_WRITE,
			MAP_SHARED, segment_fd, 0);
	if (segment_ptr == MAP_FAILED) {
		FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"mmap failed: '%s'\n", strerror(errno));
		err = errno;
		goto error_return;
	}

	close(segment_fd);	/* safe to close now */

	/*
	 *  Wait for completion of the initialization of the SHM segment before using
	 *  it.
	 */
	loop = 0;
	struct opx_shm_fifo_segment *fifo_segment =
		(struct opx_shm_fifo_segment *)(((uintptr_t)segment_ptr + 64) & (~0x03Full));
	for (;;) {
		uint64_t init =
			atomic_load_explicit(&fifo_segment->initialized_.val, memory_order_acquire);

		if (init == 0) {
			if (loop++ > OPX_SHM_TX_CONNECT_MAX_WAIT) {
				FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
					"SHM object '%s' still initializing.\n",
					segment_key);
				return -FI_EAGAIN;
			}
			usleep(1000);
		} else {
			break;
		}
	}

	tx->connection[rx_id].segment_ptr = segment_ptr;
	tx->connection[rx_id].segment_size = segment_size;
	tx->connection[rx_id].inuse = false;
	tx->fifo_segment[rx_id] = fifo_segment;
	strcpy(tx->connection[rx_id].segment_key, segment_key);

	FI_LOG(tx->prov, FI_LOG_INFO, FI_LOG_FABRIC,
		"SHM connection to %u context passed. Segment (%s), %d, segment (%p) size %zu\n",
		rx_id, segment_key, segment_fd, segment_ptr, segment_size);

	return FI_SUCCESS;

error_return:

	FI_LOG(tx->prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
		"Connection failed: %s\n", strerror(err));

	return -FI_EINVAL;
}

static inline
ssize_t opx_shm_tx_close (struct opx_shm_tx *tx,
		const unsigned rx_id)
{
	if (tx->connection[rx_id].segment_ptr != NULL) {
		munmap(tx->connection[rx_id].segment_ptr,
			tx->connection[rx_id].segment_size);
		tx->connection[rx_id].segment_ptr = NULL;
		tx->connection[rx_id].segment_size = 0;
		tx->fifo_segment[rx_id] = NULL;
		tx->connection[rx_id].inuse = false;
	}

	return FI_SUCCESS;
}

static inline
ssize_t opx_shm_tx_fini (struct opx_shm_tx *tx)
{
	unsigned i = 0;

	for (i = 0; i < OPX_SHM_MAX_CONN_NUM; ++i) {
		if (tx->connection[i].segment_ptr != NULL) {
			munmap(tx->connection[i].segment_ptr,
				tx->connection[i].segment_size);
			tx->connection[i].segment_ptr = NULL;
			tx->connection[i].segment_size = 0;
			tx->connection[i].inuse = false;
			tx->fifo_segment[i] = NULL;
		}
	}

	return FI_SUCCESS;
}

static inline
unsigned opx_shm_daos_rank_index (unsigned rank, unsigned rank_inst)
{
	unsigned index = rank_inst << 8 | rank;

	return index;
}

static inline
void * opx_shm_tx_next (struct opx_shm_tx *tx, unsigned peer, uint64_t *pos,
		bool use_rank, unsigned rank, unsigned rank_inst, ssize_t *rc)
{
	/* HFI Rank Support:  Used HFI rank index instead of HFI index. */
	unsigned rx_index = (!use_rank) ? peer : opx_shm_daos_rank_index(rank, rank_inst);

	if (rx_index >= OPX_SHM_MAX_CONN_NUM) {
		*rc = -FI_EIO;
		FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"SHM %u context exceeds maximum contexts supported.\n", rx_index);
		return NULL;
	}

	if (tx->fifo_segment[rx_index] == NULL) {
		*rc = -FI_EIO;
		FI_LOG(tx->prov, FI_LOG_WARN, FI_LOG_FABRIC,
			"SHM %u context FIFO not initialized.\n", rx_index);
		return NULL;
	}

	struct opx_shm_fifo_segment *tx_fifo_segment = tx->fifo_segment[rx_index];
	struct opx_shm_fifo *tx_fifo = &tx_fifo_segment->fifo;

	FI_LOG(tx->prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
		"SHM sending to %u context. Segment (%s)\n", rx_index,
			tx->connection[rx_index].segment_key);

    struct opx_shm_packet* packet;
	*pos = atomic_load_explicit(&tx_fifo->enqueue_pos_.val, memory_order_acquire);
    for (;;)
    {
        packet = &tx_fifo->buffer_[*pos & FI_OPX_SHM_BUFFER_MASK];
		size_t seq = atomic_load_explicit(&(packet->sequence_.val), memory_order_acquire);
		 intptr_t dif = (intptr_t)seq - (intptr_t)*pos;
        if (dif == 0)
        {
			if(atomic_compare_exchange_weak(&tx_fifo->enqueue_pos_.val, pos, *pos + 1))
				break;
        }
        else if (dif < 0) {
			// queue is full. can't return a packet.
			FI_LOG(tx->prov, FI_LOG_DEBUG, FI_LOG_FABRIC, "Handle NULL enqueue\n");
			*rc = -FI_EAGAIN;
            return NULL;
		}
        else {
			*pos = atomic_load_explicit(&tx_fifo->enqueue_pos_.val, memory_order_acquire);
			//opx_shm_x86_pause();
		}
    }

	tx->connection[rx_index].inuse = true;
	*rc = FI_SUCCESS;
	FI_LOG(tx->prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
		"SHM sent to %u context. Segment (%s)\n", rx_index,
			tx->connection[rx_index].segment_key);

	return (void*) packet->data;
}

static inline
void opx_shm_tx_advance (struct opx_shm_tx *tx, void *packet_data, uint64_t pos)
{
	struct opx_shm_packet *packet = container_of(packet_data, struct opx_shm_packet, data);
	/* HFI Rank Support:  Rank and PID included with packet sequence and data */
	packet->origin_rank = tx->rank;
	packet->origin_rank_inst = tx->rank_inst;
	atomic_store_explicit(&packet->sequence_.val, pos+1, memory_order_release);
	return;
}

static inline
struct opx_shm_packet * opx_shm_rx_next (struct opx_shm_rx *rx, uint64_t * pos)
{
	struct opx_shm_fifo_segment *rx_fifo_segment = rx->fifo_segment;
	struct opx_shm_fifo *rx_fifo = &rx_fifo_segment->fifo;

    struct opx_shm_packet* packet;
	*pos = atomic_load_explicit(&rx_fifo->dequeue_pos_.val, memory_order_acquire);

    for (;;)
    {
        packet = &rx_fifo->buffer_[*pos & FI_OPX_SHM_BUFFER_MASK];
		size_t seq = atomic_load_explicit(&(packet->sequence_.val), memory_order_acquire);
        intptr_t dif = (intptr_t)seq - (intptr_t)(*pos + 1);
        if (dif == 0)
        {
			if (atomic_compare_exchange_weak(&rx_fifo->dequeue_pos_.val, pos, *pos + 1))
				break;
        }
        else if (dif < 0) {
            return NULL;
		}
        else {
			*pos = atomic_load_explicit(&rx_fifo->dequeue_pos_.val, memory_order_acquire);
			//opx_shm_x86_pause();
		}
    }
	return packet;
}

static inline
void opx_shm_rx_advance (struct opx_shm_rx *rx, void *packet_data, uint64_t pos)
{
	struct opx_shm_packet *packet = container_of(packet_data, struct opx_shm_packet, data);
	atomic_store_explicit(&packet->sequence_.val, pos + FI_OPX_SHM_BUFFER_MASK + 1, memory_order_release);
	return;
}

void opx_register_shm_handler();

#endif /* _OPX_SHM_H_ */
