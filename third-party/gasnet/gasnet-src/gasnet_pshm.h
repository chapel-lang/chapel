/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_pshm.h $
 * Description: GASNet infrastructure for shared memory communications
 * Copyright 2009, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_SYSV_H
#define _GASNET_SYSV_H

#if !GASNET_PSHM
  #error "gasnet_pshm.h included in a non-PSHM build"
#endif

/* Must defined EXACTLY one */
/* TO DO: add to GASNet's config string */
#if defined(GASNETI_PSHM_POSIX) && !defined(GASNETI_PSHM_SYSV) && !defined(GASNETI_PSHM_FILE) && !defined(GASNETI_PSHM_XPMEM)
  #undef GASNETI_PSHM_POSIX
  #define GASNETI_PSHM_POSIX 1
#elif !defined(GASNETI_PSHM_POSIX) && defined(GASNETI_PSHM_SYSV) && !defined(GASNETI_PSHM_FILE) && !defined(GASNETI_PSHM_XPMEM)
  #undef GASNETI_PSHM_SYSV
  #define GASNETI_PSHM_SYSV 1
#elif !defined(GASNETI_PSHM_POSIX) && !defined(GASNETI_PSHM_SYSV) && defined(GASNETI_PSHM_FILE) && !defined(GASNETI_PSHM_XPMEM)
  #undef GASNETI_PSHM_FILE
  #define GASNETI_PSHM_FILE 1
#elif !defined(GASNETI_PSHM_POSIX) && !defined(GASNETI_PSHM_SYSV) && !defined(GASNETI_PSHM_FILE) && defined(GASNETI_PSHM_XPMEM)
  #undef GASNETI_PSHM_XPMEM
  #define GASNETI_PSHM_XPMEM 1
#else
  #error PSHM configuration must be exactly one of (GASNETI_PSHM_POSIX, GASNETI_PSHM_SYSV, GASNETI_PSHM_FILE, GASNETI_PSHM_XPMEM)
#endif

#if GASNET_PAGESIZE < 4096
  #define GASNETI_PSHMNET_PAGESIZE 4096
  #define GASNETI_PSHMNET_PAGESHIFT 12
#else
  #define GASNETI_PSHMNET_PAGESIZE GASNET_PAGESIZE
  #define GASNETI_PSHMNET_PAGESHIFT GASNETI_PAGESHIFT
#endif

/* In gasnet_mmap.c */
#define GASNETI_PSHM_UNIQUE_LEN 6

extern void *gasneti_mmap_vnet(uintptr_t segsize, gasneti_bootstrapBroadcastfn_t localbcastfn);
extern void gasneti_unlink_vnet(void);

/* Virtual network between processes within a shared
 * memory 'supernode'.  
 * - Implemented as a set of message queues located in a shared memory space
 *   provided by the client of this API.
 */
struct gasneti_pshmnet;			/* opaque type */
typedef struct gasneti_pshmnet gasneti_pshmnet_t;

/* Initialize pshm.
 * This includes allocating the PSHM memory region, initializing PSHM internal
 * state, and the request and reply networks for AMPSHM.
 * The first argument must be a conduit-specific exchange function.
 * The second argument is the amount of shared memory requested by the caller
 * for conduit-specific use, the address of which is returned.
 */
extern void
*gasneti_pshm_init(gasneti_bootstrapBroadcastfn_t localbcastfn, size_t aux_sz);

/* Provide a write of zero to at least one byte of each page in the given region */
extern void gasneti_pshm_prefault(void *addr, size_t len);

/*  PSHMnets needed for PSHM active messages.
 *
 * - Conduits using GASNET_PSHM must initialize these two vnets
 *   to allow a fast implementation of Active Messages to run within the
 *   supernode.  Other vnets may be created as are needed or useful.
 * - Initialize these vnets before use via gasneti_pshmnet_init().
 */
extern gasneti_pshmnet_t *gasneti_request_pshmnet;
extern gasneti_pshmnet_t *gasneti_reply_pshmnet;

/* Optional conduit-specific code if defaults in gasnet_pshm.c are not usable.
 * Conduits providing these should #define the appropriate token in gasnet_core_fwd.h
 * When a given token is NOT defined, internal default implementations are used.
 *
 * If gasnet_core_fwd.h defines GASNETC_GET_HANDLER, conduit must provide:
 *   gasnetc_get_handler(endpoint,index,field)
 *     Macro which returns the given field for the given handler 'index'
 *     The 'field' will be one of the following literals
 *       nargs, flags, fnptr, cdata, name
 *     For use ONLY by gasnet_pshm.[ch]
 */

/* Returns amount of memory needed (rounded up to a multiple of the system
 * page size) needed for a new gasneti_pshmnet_t.
 * - Takes the number of nodes in the gasnet supernode.
 * - Reads the GASNET_PSHM_NETWORK_DEPTH environment variable, if set.
 */
extern size_t gasneti_pshmnet_memory_needed(gasneti_pshm_rank_t nodes);

/* Creates a new virtual network within a gasnet shared memory supernode.
 * This function must be called collectively, and with a shared memory region
 * already created that is accessible to all nodes in the supernode
 * - 'start': starting address of region: must be page-aligned
 * - 'len': length of shared region: must be at least as long as the value
 *   returned from gasneti_pshmnet_memory_needed().
 * - 'nodes': count of the nodes in the supernode.
 */
extern gasneti_pshmnet_t *
gasneti_pshmnet_init(void *start, size_t len, gasneti_pshm_rank_t node_count);

/* Bootstrap barrier via pshmnet.
 *
 * This function has the following restrictions:
 * 1) It must be called after gasneti_pshmnet_init() has completed.
 * 2) It must be called collectively by all nodes in the vnet.
 */
extern
void gasneti_pshmnet_bootstrapBarrier(void);

/* Bootstrap barrier via pshmnet, with polling
 * Same as above but also progresses AMs
 */
extern
void gasneti_pshmnet_bootstrapBarrierPoll(void);

/* Bootstrap broadcast via pshmnet.
 *
 * This function has the following restrictions:
 * 1) It must be called after gasneti_pshmnet_init() has completed.
 * 2) It must be called collectively by all nodes in the vnet.
 * 3) It must be called with the vnet's recv queues empty.
 * 4) The rootpshmnode is the supernode-local rank.
 */
extern
void gasneti_pshmnet_bootstrapBroadcast(gasneti_pshmnet_t *vnet, void *src, 
                                        size_t len, void *dest, int rootpshmnode);

/* Bootstrap exchange via pshmnet.
 *
 * This function has the following restrictions:
 * 1) It must be called after gasneti_pshmnet_init() has completed.
 * 2) It must be called collectively by all nodes in the vnet.
 * 3) It must be called with the vnet's recv queues empty.
 */
extern
void gasneti_pshmnet_bootstrapExchange(gasneti_pshmnet_t *vnet, void *src, 
                                       size_t len, void *dest);
/* Bootstrap gather via pshmnet.
 *
 * This function has the following restrictions:
 * 1) It must be called after gasneti_pshmnet_init() has completed.
 * 2) It must be called collectively by all nodes in the vnet.
 * 3) It must be called with the vnet's recv queues empty.
 * 4) The rootpshmnode is the supernode-local rank.
 */
extern
void gasneti_pshmnet_bootstrapGather(gasneti_pshmnet_t *vnet, void *src, 
                                     size_t len, void *dest, int rootpshmnode);

/* "critical sections" in which we notify peers if we abort() while
 * they are potentially blocked in gasneti_pshmnet_bootstrapBarrier().
  */
extern void gasneti_pshm_cs_enter(void (*callback)(void));
extern void gasneti_pshm_cs_leave(void);

/* returns the maximum size payload that pshmnet can offer.  This is the
 * maximum size one can ask of gasneti_pshmnet_get_send_buffer.
 */
extern
size_t gasneti_pshmnet_max_payload(void);

/* Returns send buffer, into which message should be written.  Then
 * deliver_send_buffer() must be called, after which is is not safe to touch the
 * buffer any more.
 * - 'nbytes' must be <= gasneti_pshmnet_max_payload().
 * - Returns NULL if no buffer is available (poll your receive queue, then try
 *   again).
 * 'target' is rank relative to the supernode
 */
extern
void * gasneti_pshmnet_get_send_buffer(gasneti_pshmnet_t *vnet, size_t nbytes, 
                                       gasneti_pshm_rank_t target);

/* "Sends" message to target process.
 * Notifies target that message is ready to be received.  After calling, 'buf'
 * logically belongs to the target process, and the caller should not touch
 * the memory pointed to by 'buf' again.
 * 'nbytes' must be no larger than was passed to get_send_buffer
 * 'target' is rank relative to the supernode
 */
extern
void gasneti_pshmnet_deliver_send_buffer(gasneti_pshmnet_t *vnet, void *buf, size_t nbytes,
                                         gasneti_pshm_rank_t target);


/* Polls receipt queue for any messages from any sender.
 * - 'pbuf': address of pointer which will point to message (if successful)
 * - 'psize': out parameter (msg length will be written into memory)
 * - 'from': out parameter (sender supernode-relative rank written into memory)
 *
 * returns nonzero if no message to receive */
extern
int gasneti_pshmnet_recv(gasneti_pshmnet_t *vnet, void **pbuf, size_t *psize, 
                         gasneti_pshm_rank_t *from);

/* Called by msg receiver, to release memory after message processed.
 * It is not safe to refer to the memory pointed to by 'buf' after this call
 * is made.
 */
extern
void gasneti_pshmnet_recv_release(gasneti_pshmnet_t *vnet, void *buf); 

/*******************************************************************************
 * AMPSHM: Active Messages over PSHMnet
 *******************************************************************************/

/* Processes pending messages:  if 'repliesOnly', only checks the 'reply'
 * PSHM network (i.e. gasneti_reply_pshmnet).  */
extern int gasneti_AMPSHMPoll(int repliesOnly GASNETI_THREAD_FARG);


extern int gasneti_AMPSHM_RequestShort(
                        gex_Rank_t jobrank, gex_AM_Index_t handler,
                        gex_Flags_t flags, int numargs, va_list argptr
                        GASNETI_THREAD_FARG);
extern int gasneti_AMPSHM_RequestMedium(
                        gex_Rank_t jobrank, gex_AM_Index_t handler,
                        void *source_addr, size_t nbytes,
                        gex_Flags_t flags, int numargs, va_list argptr
                        GASNETI_THREAD_FARG);
extern int gasneti_AMPSHM_RequestLong(
                        gex_Rank_t jobrank, gex_AM_Index_t handler,
                        void *source_addr, size_t nbytes, void *dest_addr,
                        gex_Flags_t flags, int numargs, va_list argptr
                        GASNETI_THREAD_FARG);

extern int gasneti_AMPSHM_ReplyShort(
                        gex_Token_t token, gex_AM_Index_t handler,
                        gex_Flags_t flags, int numargs, va_list argptr);
extern int gasneti_AMPSHM_ReplyMedium(
                        gex_Token_t token, gex_AM_Index_t handler,
                        void *source_addr, size_t nbytes,
                        gex_Flags_t flags, int numargs, va_list argptr);
extern int gasneti_AMPSHM_ReplyLong(
                        gex_Token_t token, gex_AM_Index_t handler,
                        void *source_addr, size_t nbytes, void *dest_addr,
                        gex_Flags_t flags, int numargs, va_list argptr);

int gasnetc_AMPSHM_PrepareRequestMedium(gasneti_AM_SrcDesc_t sd,
                                        gex_Rank_t           jobrank,
                                        const void          *client_buf,
                                        size_t               least_payload,
                                        size_t               most_payload,
                                        gex_Event_t         *lc_opt,
                                        gex_Flags_t          flags,
                                        unsigned int         nargs);
void gasnetc_AMPSHM_CommitRequestMedium(gasneti_AM_SrcDesc_t sd,
                                        gex_AM_Index_t handler, size_t nbytes,
                                        va_list argptr);
int gasnetc_AMPSHM_PrepareRequestLong(gasneti_AM_SrcDesc_t sd,
                                      gex_Rank_t           jobrank,
                                      const void          *client_buf,
                                      size_t               least_payload,
                                      size_t               most_payload,
                                      void                *dest_addr,
                                      gex_Event_t         *lc_opt,
                                      gex_Flags_t          flags,
                                      unsigned int         nargs);
void gasnetc_AMPSHM_CommitRequestLong(gasneti_AM_SrcDesc_t sd,
                                      gex_AM_Index_t handler, size_t nbytes,
                                      void *dest_addr, va_list argptr);

int gasnetc_AMPSHM_PrepareReplyMedium(gasneti_AM_SrcDesc_t sd,
                                      gex_Token_t          token,
                                      const void          *client_buf,
                                      size_t               least_payload,
                                      size_t               most_payload,
                                      gex_Event_t         *lc_opt,
                                      gex_Flags_t          flags,
                                      unsigned int         nargs);
void gasnetc_AMPSHM_CommitReplyMedium(gasneti_AM_SrcDesc_t sd,
                                      gex_AM_Index_t handler, size_t nbytes,
                                      va_list argptr);
int gasnetc_AMPSHM_PrepareReplyLong(gasneti_AM_SrcDesc_t sd,
                                    gex_Token_t          token,
                                    const void          *client_buf,
                                    size_t               least_payload,
                                    size_t               most_payload,
                                    void                *dest_addr,
                                    gex_Event_t         *lc_opt,
                                    gex_Flags_t          flags,
                                    unsigned int         nargs);
void gasnetc_AMPSHM_CommitReplyLong(gasneti_AM_SrcDesc_t sd,
                                    gex_AM_Index_t handler, size_t nbytes,
                                    void *dest_addr, va_list argptr);

/*******************************************************************************
 * Intra-supernode shared-memory barrier
 *******************************************************************************/

typedef struct {
    gasneti_atomic_t state; /* One done bit per phase and result in remaining bits */
    gasneti_atomic_t ready; /* Indicates when initialization is completed */
    int size;
    gex_AM_Arg_t volatile flags, value; /* supernode consensus for hierarchical barrier */
    char _pad1[GASNETI_CACHE_PAD(  2*sizeof(gasneti_atomic_t)
                                 + sizeof(int)
                                 + 2*sizeof(gex_AM_Arg_t))];
    /*---------------*/
    struct gasneti_pshm_barrier_node {
      union gasneti_pshm_barrier_node_u {
        struct {
          int volatile value, flags;
          int volatile phase;
        } wmb;
        uint64_t volatile u64;
      } u;
      char _pad[GASNETI_CACHE_PAD(sizeof(union gasneti_pshm_barrier_node_u))];
    } node[1]; /* VLA */
} gasneti_pshm_barrier_t;

extern gasneti_pshm_barrier_t *gasneti_pshm_barrier;

#endif /* _GASNET_SYSV_H */
