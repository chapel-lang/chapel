/*   $Source: /var/local/cvs/gasnet/portals4-conduit/gasnet_portals4.h,v $
 *     $Date: 2013/04/14 01:14:26 $
 * $Revision: 1.5 $
 * Description: Portals 4 specific configuration
 * Copyright 2012, Sandia National Laboratories
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_PORTALS4_H
#define GASNET_PORTALS4_H

#define COLLECTIVE_PT 6
#define AM_PT         7

/* match bits used on collective PT */
#define BOOTSTRAP_BARRIER_MB    0x00001
#define BOOTSTRAP_BCAST_MB      0x00002
#define BOOTSTRAP_EXCHANGE_MB   0x00003

/*
 * match bits used on the active message PT
 * 
 * header data: op count
 *
 * match bits used on AM_PT
 *
 *  0 1 23 4567 0 1234567 0 1234567 01234567 01234567 01234567 01234567 01234567
 * | | |  |      |         |             payload length
 *  ^ ^ ^    ^      ^
 *  | | |    |      +--- handler id
 *  | | |    +--- AM argument count
 *  | | +--- Protocol: AM_MEDIUM, AM_LONG, or AM_LONG_PACKED
 *  | +--- Request/Response: AM_REQUEST or AM_REPLY (0 if LONG_DATA) 
 *  +-- Type: ACTIVE_MESSAGE or LONG_DATA
 */

#define LONG_DATA      0x0000000000000000ULL
#define ACTIVE_MESSAGE 0x8000000000000000ULL

#define AM_REPLY       0x0000000000000000ULL
#define AM_REQUEST     0x4000000000000000ULL

#define AM_SHORT       0x0000000000000000ULL
#define AM_MEDIUM      0x1000000000000000ULL
#define AM_LONG        0x2000000000000000ULL
/* GASNet long protocol, but message small enough to in send buffer */
#define AM_LONG_PACKED 0x3000000000000000ULL
#define AM_TYPE_MASK   0x3000000000000000ULL

#define AM_REQREP_BITS   0x7FFFFFFFFFFFFFFFULL

#define CREATE_MATCH_BITS(type, req_type, protocol, arg_count, handler_id, payload_length) \
    (type | req_type | protocol | ((ptl_match_bits_t) arg_count << 55) | ((ptl_match_bits_t) handler_id << 47) | payload_length)

#define IS_ACTIVE_MESSAGE(match_bits)  (((ACTIVE_MESSAGE & match_bits) == 0) ? 0 : 1)
#define IS_LONG_DATA(match_bits)       (((ACTIVE_MESSAGE & match_bits) == 0) ? 1 : 0)

#define IS_AM_REQUEST(match_bits)      (((AM_REQUEST & match_bits) == 0) ? 0 : 1)
#define IS_AM_REPLY(match_bits)        (((AM_REQUEST & match_bits) == 0) ? 1 : 0)

#define IS_AM_SHORT(match_bits)        (((AM_TYPE_MASK & match_bits) == AM_SHORT) ? 1 : 0)
#define IS_AM_MEDIUM(match_bits)       (((AM_TYPE_MASK & match_bits) == AM_MEDIUM) ? 1 : 0)
#define IS_AM_LONG(match_bits)         (((AM_TYPE_MASK & match_bits) == AM_LONG) ? 1 : 0)
#define IS_AM_LONG_PACKED(match_bits)  (((AM_TYPE_MASK & match_bits) == AM_LONG_PACKED) ? 1 : 0)

#define GET_ARG_COUNT(match_bits)       ((match_bits & 0x0F80000000000000ULL) >> 55)
#define GET_HANDLERID(match_bits)       ((match_bits & 0x007F800000000000ULL) >> 47)
#define GET_PAYLOAD_LENGTH(match_bits)   (match_bits & 0x00007FFFFFFFFFFFULL)

/* Portals 4 AM handler token */
struct gasnetc_p4_token_t {
    int reply_sent;
    gasnet_node_t sourceid;
};
typedef struct gasnetc_p4_token_t gasnetc_p4_token_t;


/* Active message receive blocks.  An array of these entries is
   created at initialization time, along with p4_am_size sized blocks
   of memory pointed to by data.  The array will be of length
   p4_am_num_entriess. */
struct p4_am_block_t {
    ptl_handle_me_t me_h;
    void *data;
};
typedef struct p4_am_block_t p4_am_block_t;

#define P4_FRAG_TYPE_AM   0x01
#define P4_FRAG_TYPE_DATA 0x02

struct p4_frag_t {
    int type;
};
typedef struct p4_frag_t p4_frag_t;

/* match_bits and hdr_data are for retransmit */
struct p4_frag_am_t {
    p4_frag_t base;

    int rank;
    ptl_match_bits_t match_bits;
    ptl_hdr_data_t hdr_data;
    ptl_size_t data_length;
    gasneti_weakatomic_t op_count;

    char data[1];
};
typedef struct p4_frag_am_t p4_frag_am_t;

struct p4_frag_data_t {
    p4_frag_t base;

    p4_frag_am_t *am_frag;
    volatile int32_t *send_complete_ptr;
    ptl_size_t local_offset;
    ptl_size_t length;
    ptl_match_bits_t match_bits;
    ptl_size_t remote_offset;
};
typedef struct p4_frag_data_t p4_frag_data_t;

int gasnetc_p4_init(int *rank, int *size);
int gasnetc_p4_attach(void *segbase, uintptr_t segsize);
void gasnetc_p4_exit(void);
void gasnetc_p4_poll(void);

int gasnetc_p4_TransferGeneric(int category, ptl_match_bits_t req_type,
                               gasnet_node_t dest, 
                               gasnet_handler_t handler, void *source_addr, size_t nbytes,
                               void *dest_addr, int numargs, va_list argptr);

void gasnetc_bootstrapBarrier(void);
void gasnetc_bootstrapExchange(void *src, size_t len, void *dest);

/* Use alloca()?  (e.g. to work-around bug 2079) */
#ifdef GASNETI_USE_ALLOCA
  /* Keep defn */
#elif !PLATFORM_COMPILER_PGI
  #define GASNETI_USE_ALLOCA 1
#endif

#endif
