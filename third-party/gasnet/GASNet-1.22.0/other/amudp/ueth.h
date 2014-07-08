/*   $Source: /var/local/cvs/gasnet/other/amudp/ueth.h,v $
 *     $Date: 2009/03/30 02:40:45 $
 * $Revision: 1.4 $
 * Description: UETH interface to ISTORE-specific hardware driver
 * Copyright 2000, Dan Hettena <danielh@cs.berkeley.edu>
 *                 and Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef UETH_H
#define UETH_H

/* GENERAL NOTES
 *
 * While the API is not reentrant, it can be used from multiple threads (so
 * long as the used isn't concurrent, of course).
 */

#if PLATFORM_OS_MSWINDOWS
/* ueth doesn't run on win32, but allow static checking of win32 amudp apps */
  #include <stddef.h>
  typedef unsigned __int8		uint8_t;
  typedef unsigned __int16	uint16_t;
  typedef unsigned __int32	uint32_t;
  typedef unsigned __int64	uint64_t;
  typedef __int8			int8_t;
  typedef __int16			int16_t;
  typedef __int32			int32_t;
  typedef __int64			int64_t;
#elif PLATFORM_OS_CYGWIN
  #include <sys/types.h>
  typedef u_int8_t     uint8_t;
  typedef u_int16_t   uint16_t; 
  typedef u_int32_t   uint32_t;
  typedef u_int64_t   uint64_t;

  typedef unsigned int    uintptr_t; /* unsigned type big enough to hold any pointer offset */
#else
#include <inttypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif


/*** TIMING ***/

/* 266 MHz => cycle time is 3.75ns = (15/4)ns */
#define	ueth_ticks_per_second	266666667

#define	ueth_ticks_to_us(ticks)	(((ticks)*3)/800)
#define	ueth_us_to_ticks(us)	(((us)*800)/3)

/* May be used any time. Return value strictly increases with time. */
static inline uint64_t ueth_getcputime(void) {
  uint64_t result;
  asm volatile("rdtsc" : "=A" (result));
  return result;
}

static inline void ueth_microdelay(uint64_t us) {
  uint64_t time_limit = ueth_getcputime() + ueth_us_to_ticks(us);
  while (ueth_getcputime() < time_limit);
}

/* Time in microseconds since CPU was turned on. Requires 64-bit divide. */
#define	ueth_getustime()	ueth_ticks_to_us(ueth_getcputime())


/*** NETWORKING ***/

typedef struct {
  int16_t fixed;
  struct {
    int16_t index;
  } variable;
} ueth_addr_t;

/* the length of this must be a multiple of UETH_ALIGNMENT */  
typedef struct {
  uint8_t	dst[6];
  uint8_t	src[6];
  uint16_t	proto;
  uint16_t	checksum;
} ueth_header_t;

#define UETH_HEADERLENGTH	sizeof(ueth_header_t)	/* subject to change */
#define UETH_MAXPACKETSIZE	1514
#define UETH_ALIGNMENT		8			/* subject to change */
#define	UETH_ALIGN(x)		(((x) + UETH_ALIGNMENT-1)&~(UETH_ALIGNMENT-1))

typedef	void	(*ueth_addr_hook_t)(ueth_addr_t *address);

/* return values for functions */
#define UETH_OK			0	/* operation completed successfully */
#define UETH_ERR_TIMEDOUT	1	/* a blocking operation timed out */
#define UETH_ERR_MEMRESOURCE	2	/* insuf. phys mem or bad alignment */
#define UETH_ERR_NICRESOURCE	3	/* no working network device found */
#define	UETH_ERR_INVALID	4	/* call invalid in current state */
#define	UETH_ERR_HARDWARE	5	/* unexpected hardware failure */
#define	UETH_ERR_SOFTWARE	6	/* bad software configuration */

#define UETH_ERR_NOTINIT	UETH_ERR_INVALID

int	ueth_init();
/* Ensures card can be allocated, and that ueth_getaddress() can subsequently
 * be called.
 * Returns: UETH_OK, UETH_ERR_NICRESOURCE, UETH_ERR_INVALID
 */

int	ueth_allocatepool(void **sendpool, unsigned int maxpacketsize,
			  int maxsends, int maxrecvs);
/* Must be called exactly once after ueth_init().
 * No packets longer than maxpacketsize will be sendable or receivable.
 *
 * Guarantees (sendpool % UETH_ALIGNMENT) == 0.
 * The send pool will be exactly maxpacketsize*maxsends bytes long.
 *
 * Returns: UETH_OK, UETH_ERR_NOTINIT, UETH_ERR_MEMRESOURCE
 */

int	ueth_getaddress(ueth_addr_t *address);
int	ueth_setaddresshook(ueth_addr_hook_t hook);
/* May be called any time after ueth_init(). The hook will only be called
 * during a call to ueth_send() or to ueth_recv().
 * Returns: UETH_OK, UETH_ERR_NOTINIT
 */
  
int	ueth_terminate();
/* Terminate all network operations, releasing all resources (the NIC and any
 * allocated memory). ueth_init() may be used again.
 *
 * If the application exits without calling ueth_terminate(), resources will
 * still be released properly.
 *
 * Returns: UETH_OK, UETH_ERR_NOTINIT
 */

/* The user must first call ueth_init() and ueth_allocatepool() (and optionally
 * ueth_setaddresshook()), and then the user may call the procedures below.
 * If this initialization order is not observed, the procedures below are
 * permitted to return UETH_ERR_NOTINIT, but will more likely just segfault.
 *
 * Warning: when a link failover fails, ueth moves to the uninitialized state.
 * Link failovers are attempted only in calls to ueth_recv(). When a failover
 * fails, ueth_recv() returns an error code other than UETH_ERR_TIMEDOUT. It is
 * therefore always necessary to check the ueth_recv() return value. Note that
 * this check need not lengthen the critical path, because the caller need only
 * check this condition after it has determined that the return value is not
 * UETH_OK.
 */

int	ueth_set_packet_destination(void *msg, ueth_addr_t *dst);
int	ueth_send_preset(void *msg, unsigned int len, int *cb_addr);

static inline int ueth_send(void *msg, unsigned int len, ueth_addr_t *dst,
			    int *cb_addr) {
  ueth_set_packet_destination(msg, dst);
  return ueth_send_preset(msg, len, cb_addr);
}

/* If ueth_send() or ueth_send_preset() are unable to send a packet because all
 * ethernet cards are lost, the program will abort.
 * Returns UETH_OK (or UETH_ERR_NOTINIT)
 *
 * The message passed in includes space for the ueth header. The actual data
 * payload starts at msg + UETH_HEADERLENGTH. The len argument must INCLUDE
 * this header space. The message must be aligned properly.
 *
 * Loopback messages (destined for a local NIC) WILL be handled correctly.
 */

int	ueth_query_send(const void *msg, int cb_addr);
/* Returns non-zero iff the transmit descriptor generated by the most recent
 * call to ueth_send_preset() or ueth_send() is still in the transmit queue.
 */

int	ueth_cancel_send(const void *msg, int cb_addr);
/* Returns non-zero iff the transmit descriptor generated by the most recent
 * call to ueth_send_preset() or ueth_send() is still in the transmit queue
 * and a best-effort attempt was made to nullify the transmit descriptor.
 */

int	ueth_recv(void **buf, unsigned int *len,
		  int block_us, ueth_addr_t *src);
/* Try to receive a message, optionally blocking.
 *
 * block_us specifies how to block:
 * -1	block until a message arrives
 * 0	strict poll
 * n>0	wait up to n microseconds for a message, then time out
 *
 * Link tests are done once in a while regardless of whether ueth_recv()
 * blocks, so it is never necessary to tell ueth_recv() to block.
 *
 * Returns: UETH_OK, ETH_ERR_TIMEDOUT, UETH_ERR_SOFTWARE, UETH_ERR_NICRESOURCE,
 *		or UETH_ERR_MEMRESOURCE (or UETH_ERR_NOTINIT)
 *
 * If successful and buf is not NULL, then len and src must also be non-NULL.
 * *buf is set to point to the header of the new message, and *len is set to
 * either 60 or the length of the message (INCLUDING the header), whichever is
 * greater (all ueth messages are hardware-padded up to 60 bytes before they
 * are sent). The actual data payload begins at *buf + UETH_HEADERLENGTH. Both
 * *buf and the actual data payload are guaranteed to be aligned by
 * UETH_ALIGNMENT. *src is set to the address of the SENDER of the message.
 * The receive buffer must eventually be returned to the ueth layer with a call
 * to ueth_freerxbuf().
 *
 * If buf is NULL, *buf and *len are ignored, and state of the receive pool
 * is unchanged by this call, but ueth_recv() still blocks according to
 * block_us. This can be used to wait for a message to arrive, without actually
 * dequeuing the message: if buf was NULL and ueth_recv() returned UETH_OK,
 * then the next call to ueth_recv() is guaranteed to return without blocking.
 */

int	ueth_freerxbuf(void *buf);
/* Release a receive buffer returned by ueth_recv().
 * Returns: UETH_OK (or UETH_ERR_NOTINIT)
 */

int	ueth_fixup_recv(ueth_addr_t *old_addr, ueth_addr_t *new_addr);
/* For each packet in the receive queue, if its source address is set to
 * old_addr, change it to new_addr. Returns the number of packets modified, or
 * -1 if ueth is uninitialized or a failover failed (fatal error).
 */

int	ueth_kill_link(void);
/* Kill the current link. Because of the spanning tree protocol, the link will
 * not be available again for 45 seconds.
 *
 * Returns: UETH_OK (or UETH_ERR_NOTINIT)
 */

int	ueth_kill_link_on_signal(int signum);
/* Schedule a call to ueth_kill_link() on signal signum.
 *
 * Returns: UETH_OK (or UETH_ERR_NOTINIT)
 */

#ifdef __cplusplus
#if 0
{
#endif
}
#endif


#endif
