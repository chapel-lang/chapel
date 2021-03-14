/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/pmi-spawner/gasnet_bootstrap_pmi.c $
 * Description: GASNet conduit-independent pmi-based spawner
 * Copyright 2013, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */
#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#if GASNETI_PMIX_HACK
// PMIx uses [mc]alloc(), free(), strdup() within its headers (regardless of
//  whether you use the PMI-1, PMI-2, or PMIx API)
#  undef malloc
#  undef calloc
#  undef free
#  undef strdup
#endif

#if HAVE_PMI_CRAY_H
#  include <pmi_cray.h>
#  define USE_PMI2_API 1
#  ifndef PMI2_SUCCESS
#    define PMI2_SUCCESS PMI_SUCCESS
#  endif
#elif HAVE_PMI_H
#  include <pmi.h>
#elif HAVE_PMI2_H
#  include <pmi2.h>
#  define USE_PMI2_API 1
#elif HAVE_PMIX_H
#  if HAVE_STDBOOL_H
#  include <stdbool.h> /* For use of "bool" in PMIx headers */
#  endif
#  include <pmix.h>
#  define USE_PMIX_API 1
#else
#  error "Unknown path to PMI header"
#endif

/* Sigh.  For portals4/pmi.h and perhaps others. */
#ifndef PMI_TRUE
   typedef int PMI_BOOL;
#  define PMI_TRUE     1
#  define PMI_FALSE    0
#endif

// The ANL, Cray and Open PMIx PMI servers do not behave "well" for multiple
// Puts to the same key (though the behaviors differ).  We cannot currently
// determine if we are using a server which does behave nicely (currently
// only SLURM that we are aware of) because the servers and clients from
// multiple projects are (intentionally) implementing the same wire
// protocols.  Notably SLURM adopts others' wire protocols to launch
// unmodified MPICH and Open MPI applications.
#if defined(GASNETI_PMI_DUP_KEYS)
  // enforce 1 or undef
  #if GASNETI_PMI_DUP_KEYS
    #undef GASNETI_PMI_DUP_KEYS
    #define GASNETI_PMI_DUP_KEYS 1
  #else
    #undef GASNETI_PMI_DUP_KEYS
  #endif
#endif

// We really don't want to use PMI "point-to-point" on a Cray.
// So, if the configure probe for PMI_Allgather() failed, we force
// it here.  We rather this results in an application link failure
// than the alternative: use of the non-scalable bootstrapExchange().
// TODO: revisit if/when we have a more scalable PMI based Exchange.
#if HAVE_PMI_CRAY_H && !defined(HAVE_PMI_ALLGATHER)
  #define HAVE_PMI_ALLGATHER 1
#endif

GASNETI_IDENT(gasnetc_IdentString_HavePMISpawner, "$GASNetPMISpawner: 1 $");

static gasneti_spawnerfn_t const spawnerfn;

static char *kvs_name = NULL, *kvs_key = NULL, *kvs_value = NULL;
static int max_name_len, max_key_len, max_val_len;
static int max_val_bytes;
#ifdef USE_PMIX_API
static pmix_proc_t myproc;
#endif

static int kvs_is_init = 0;  // Gates lazy initialization

#if GASNETI_PMI_DUP_KEYS
  // op "maps" used for GC of *only* previously-used keys
  //   bit0 (value 1) set if phase 0 key used
  //   bit1 (value 2) set if phase 1 key used
  #define OP_INC_MAP(stem,phase) op_##stem##_map |= (((phase)&1)+1)
  static unsigned int op_X_map, op_E_map, op_A_map, op_B_map, op_S_map;
#else
  #define OP_INC_MAP(stem,phase) ((void)0)
#endif

/* do_{en,de}code()
 * Use a (minor) variant on Adobe's Ascii85 encoding.
 *   See http://en.wikipedia.org/wiki/Ascii85
 * No begin or end delimiters (or explict length) are required.
 * We add a 'y' exception to encode 0xFFFFFFFF (NOT the 'y'=0x20202020 exception in btoa)
 * We use 'v', 'w', and 'x' to deal with Cray PMI oddities.
 */

#if USE_PMI2_API
/* Cray disallows '/', ';' and '=' */
GASNETI_INLINE(a85_enc)
uint8_t a85_enc(uint8_t c) {
    c += '!';
    switch (c) {
        case '/': return 'v';
        case ';': return 'w';
        case '=': return 'x';
        default:  return c;
    }
}
GASNETI_INLINE(a85_dec)
uint8_t a85_dec(uint8_t c) {
    switch (c) {
        case 'v': return ('/' - '!');
        case 'w': return (';' - '!');
        case 'x': return ('=' - '!');
        default:  return (c - '!');
    }
}
#else
#  define a85_enc(_c) ((_c) + '!')
#  define a85_dec(_c) ((_c) - '!')
#endif

static
void do_encode(uint8_t *in, size_t len) {
  #if HAVE_PMI_CRAY_H
    // Cray PMI erroneously encodes a zero-length argument as the 6-char string "(null)"
    if (!len) {
      kvs_value[0] = 'z';
      kvs_value[1] = '\0';
      return;
    }
  #endif

    char *p = kvs_value;
    gasneti_assert_always(5 * ((len + 3) / 4) <= max_val_len);
    while (len) {
        uint32_t x = 0;
        const int sz = MIN(4, len);
        int i;

        for (i=0; i<sz; ++i) {
            x = (x << 8) | in[i];
        }

        if (0 == x) {
            p[0] = 'z';
            p += 1;
        } else if (0xFFFFFFFF == x) {
            p[0] = 'y';
            p += 1;
        } else {
            const int pad = 4 - sz;
            x = x << (8 * pad);
            p[4] = a85_enc(x % 85);
            x /= 85;
            p[3] = a85_enc(x % 85);
            x /= 85;
            p[2] = a85_enc(x % 85);
            x /= 85;
            p[1] = a85_enc(x % 85);
            x /= 85;
            p[0] = a85_enc(x);
            p += 5 - pad;
        }

        len -= sz;
        in += sz;
    }
    *p = '\0';
}

static
void do_decode(uint8_t *out, size_t len, size_t in_len) {
  #if HAVE_PMI_CRAY_H
    // Cray PMI erroneously encodes a zero-length argument as the 6-char string "(null)"
    if (!len) {
      gasneti_assert_always(in_len == 1);
      gasneti_assert_always(kvs_value[0] == 'z');
      return;
    }
  #endif

    const char *p = kvs_value;
    while (len) {
        uint32_t x;
        const int sz = MIN(4, len);
        int i;

        if ('z' == p[0]) {
            x = 0;
            p += 1;
        } else if ('y' == p[0]) {
            x = 0xFFFFFFFF;
            p += 1;
        } else {
            const int pad = 4 - sz;
            gasneti_assert_always(!pad || (strlen(p) == (5 - pad)));
            x  = a85_dec(p[0]);
            x *= 85;
            x += a85_dec(p[1]);
            x *= 85;
            x += a85_dec((pad < 3) ? p[2] : 'u');
            x *= 85;
            x += a85_dec((pad < 2) ? p[3] : 'u');
            x *= 85;
            x += a85_dec((pad < 1) ? p[4] : 'u');
            p += (5 - pad);
            x = x >> (8 * pad);
        }

        i = sz;
        while (i--) {
            out[i] = x & 0xff;
            x = x >> 8;
        }

        len -= sz;
        out += sz;
    }
    // Check that we consumed the entire input
    size_t decoded = p - kvs_value;
    gasneti_assert_always_uint(decoded ,==, in_len);
}

/* Key generation helpers
 *
 * Default:
 * Stem - a single upper case char
 * Counter - 32-bit hex w/ a single-char separator
 * X,Y - 32-bit numbers expressed in hex w/ a single-char separator
 * Resulting max: 1 + 8 + 1 + 8 + 1 + 8 = 27 char
 *
 * GASNETI_PMI_DUP_KEYS:
 * Stem - a single upper case char
 * Counter - least significant bit determines upper/lower case use of Stem
 * X,Y - 32-bit numbers expressed in hex w/ a single-char separator
 * Resulting max: 1 + 8 + 1 + 8 = 18 char
 *
 * TODO: base85 could reduce the 8-char fields to max 5 chars each.
 */

static void do_kvs_key0(char stem, unsigned int counter) {
  gasneti_assert(isupper(stem));
#if GASNETI_PMI_DUP_KEYS
  kvs_key[0] = (counter&1) ? stem : tolower(stem);
  kvs_key[1] = '\0';
#else
  int rc = snprintf(kvs_key, max_key_len, "%c%x", stem, counter);
  gasneti_assert_always_int(rc ,>, 0);
  gasneti_assert_always_int(rc ,<, max_key_len);
#endif
}

static void do_kvs_key1(char stem, unsigned int counter, unsigned int x) {
  gasneti_assert(isupper(stem));
#if GASNETI_PMI_DUP_KEYS
  char c = (counter&1) ? stem : tolower(stem);
  int rc = snprintf(kvs_key, max_key_len, "%c%x", c, x);
#else
  int rc = snprintf(kvs_key, max_key_len, "%c%x.%x", stem, counter, x);
#endif
  gasneti_assert_always_int(rc ,>, 0);
  gasneti_assert_always_int(rc ,<, max_key_len);
}

static void do_kvs_key2(char stem, unsigned int counter, unsigned int x, unsigned int y) {
  gasneti_assert(isupper(stem));
#if GASNETI_PMI_DUP_KEYS
  char c = (counter&1) ? stem : tolower(stem);
  int rc = snprintf(kvs_key, max_key_len, "%c%x.%x", c, x, y);
#else
  int rc = snprintf(kvs_key, max_key_len, "%c%x.%x.%x", stem, counter, x, y);
#endif
  gasneti_assert_always_int(rc ,>, 0);
  gasneti_assert_always_int(rc ,<, max_key_len);
}

/* Put/Get/Fence wrappers */

GASNETI_INLINE(do_kvs_put)
void do_kvs_put(void *value, size_t sz, int is_local) {
#if USE_PMIX_API
    pmix_value_t val;
    pmix_status_t ret;
    val.type = PMIX_BYTE_OBJECT;
    val.data.bo.bytes = value;
    val.data.bo.size = sz;
    ret = PMIx_Put(is_local ? PMIX_LOCAL : PMIX_GLOBAL, kvs_key, &val);
    gasneti_assert_always_int(PMIX_SUCCESS, ==, ret);
#elif USE_PMI2_API
    do_encode(value, sz);
    int rc = PMI2_KVS_Put(kvs_key, kvs_value);
    gasneti_assert_always_int(PMI2_SUCCESS, ==, rc);
#else
    do_encode(value, sz);
    int rc = PMI_KVS_Put(kvs_name, kvs_key, kvs_value);
    gasneti_assert_always_int(PMI_SUCCESS, ==, rc);
#endif
}

GASNETI_INLINE(do_kvs_get)
void do_kvs_get(void *value, size_t sz, gex_Rank_t src) {
#if USE_PMIX_API
    pmix_status_t ret;
    pmix_value_t *val;
    myproc.rank = (src == GEX_RANK_INVALID) ? PMIX_RANK_UNDEF : src;
    ret = PMIx_Get(&myproc, kvs_key, NULL, 0, &val);
    gasneti_assert_always_int(PMIX_SUCCESS, ==, ret);
    gasneti_assert_always_ptr(NULL, !=, val);
    gasneti_assert_always_int(PMIX_BYTE_OBJECT, ==, val->type);
    gasneti_assert_always_ptr(NULL, !=, val->data.bo.bytes);
    gasneti_assert_always_uint(sz, ==, val->data.bo.size);
    memcpy(value, val->data.bo.bytes, sz);
    PMIX_VALUE_RELEASE(val);
#elif USE_PMI2_API
    int rc;
    int len;
    if (src == GEX_RANK_INVALID) src = PMI2_ID_NULL;
    rc = PMI2_KVS_Get(kvs_name, src, kvs_key, kvs_value, max_val_len, &len);
    gasneti_assert_always_int(PMI2_SUCCESS, ==, rc);
    gasneti_assert_always_int(len, >=, 0); // Negative would mean value larger than max_val_len
    do_decode(value, sz, len);
#else
    int rc;
    rc = PMI_KVS_Get(kvs_name, kvs_key, kvs_value, max_val_len);
    gasneti_assert_always_int(PMI_SUCCESS, ==, rc);
    size_t len = strlen(kvs_value);
    do_decode(value, sz, len);
#endif
}

GASNETI_INLINE(do_kvs_fence)
void do_kvs_fence(void) {
#if USE_PMIX_API
    /* for now, we will assume that we direct the
     * server to collect all data because GASNET
     * will immediately pull all data for every
     * process. This can be changed to a non-blocking
     * fence, and to use the direct modex mode, at
     * some later date when further optimization
     * and/or memory footprint minimzation is desired */
    pmix_info_t info;
    pmix_status_t rc;
    memset(&info, 0, sizeof(pmix_info_t));
    (void)strncpy(info.key, PMIX_COLLECT_DATA, PMIX_MAX_KEYLEN);
    info.value.type = PMIX_UNDEF;
    info.value.data.flag = 1;
    rc = PMIx_Commit();
    gasneti_assert_always_int(PMIX_SUCCESS, ==, rc);
    rc = PMIx_Fence(NULL, 0, &info, 1);
    gasneti_assert_always_int(PMIX_SUCCESS, ==, rc);
#elif USE_PMI2_API
    PMI2_KVS_Fence();
#else
    PMI_KVS_Commit(kvs_name);
    PMI_Barrier();
#endif
}


PMI_BOOL  gasneti_pmi_initialized = PMI_FALSE;

/* gasneti_bootstrapInit
 */
extern gasneti_spawnerfn_t const * gasneti_bootstrapInit_pmi(
        int *argc_p, char ***argv_p,
        gex_Rank_t *nodes_p, gex_Rank_t *mynode_p) {
    int size, rank;

#if USE_PMIX_API
    pmix_value_t *val;
    pmix_proc_t proc;

    if (PMIX_SUCCESS != PMIx_Init(&myproc, NULL, 0)) {
        return NULL;
    }
    /* the namespace is our kvs_name, and the rank is
     * our rank */
    rank = myproc.rank;
    /* we will have been given our job size during init,
     * so retrieve it from the local cache here */
    memset(proc.nspace, 0, PMIX_MAX_NSLEN+1);
    (void)strncpy(proc.nspace, myproc.nspace, PMIX_MAX_NSLEN+1);
    proc.rank = PMIX_RANK_WILDCARD;

    if (PMIX_SUCCESS != PMIx_Get(&proc, PMIX_JOB_SIZE,
                                 NULL, 0, &val)) {
        gasneti_fatalerror("PMIx Get Job Size failed");
    }
    if (NULL != val && PMIX_UINT32 == val->type) {
        size = val->data.uint32;
        PMIX_VALUE_RELEASE(val);
    } else {
        gasneti_fatalerror("PMIx Get Job Size failed");
    }
#elif USE_PMI2_API
    int spawned, appnum;
    if (PMI2_SUCCESS != PMI2_Init(&spawned, &size, &rank, &appnum)) {
        return NULL;
    }
#else
    if (PMI_SUCCESS != PMI_Initialized(&gasneti_pmi_initialized)) {
        return NULL;
    }

    if (PMI_FALSE == gasneti_pmi_initialized) {
        int spawned;
        if (PMI_SUCCESS != PMI_Init(&spawned)) {
            return NULL;
        }
    }

    if (PMI_SUCCESS != PMI_Get_rank(&rank)) {
        gasneti_fatalerror("PMI_Get_rank() failed");
    }

    if (PMI_SUCCESS != PMI_Get_size(&size)) {
        gasneti_fatalerror("PMI_Get_size() failed");
    }
#endif

    *mynode_p = rank;
    *nodes_p = size;

#if USE_PMIX_API
    max_name_len = 0; // myproc used in place of kvs_name
    max_key_len = PMIX_MAX_KEYLEN + 1;
    max_val_len = 4096;  /* totally arbitrary here */
#elif USE_PMI2_API
    max_name_len = 1024; /* XXX: can almost certainly be shorter than this! */
    max_key_len = PMI2_MAX_KEYLEN;
    max_val_len = PMI2_MAX_VALLEN;
#else
    if (PMI_SUCCESS != PMI_KVS_Get_name_length_max(&max_name_len)) {
        gasneti_fatalerror("PMI_KVS_Get_name_length_max() failed");
    }
    if (PMI_SUCCESS != PMI_KVS_Get_key_length_max(&max_key_len)) {
        gasneti_fatalerror("PMI_KVS_Get_key_length_max() failed");
    }
    if (PMI_SUCCESS != PMI_KVS_Get_value_length_max(&max_val_len)) {
        gasneti_fatalerror("PMI_KVS_Get_value_length_max() failed");
    }
#endif

    // Bound allocation to reasonable sizes
#if GASNETI_PMI_DUP_KEYS
    max_key_len  = MIN(max_key_len,  24);  // 18+1 should be sufficient
#else
    max_key_len  = MIN(max_key_len,  32);  // 29+1 should be sufficient
#endif
    max_val_len  = MIN(max_val_len,  4096);

    max_val_bytes = 4 * (max_val_len / 5);

    return &spawnerfn;
}

#define do_kvs_init() \
    do { if_pf(!kvs_is_init) do_kvs_init_inner(); } while (0)
static void do_kvs_init_inner(void) {
    gasneti_assert(! kvs_is_init);

    if (max_name_len) {
        kvs_name = (char*) gasneti_malloc(max_name_len);
    }
    kvs_key = (char*) gasneti_malloc(max_key_len);
    kvs_value = (char*) gasneti_malloc(max_val_len);

#if USE_PMIX_API
    // nothing else to do
#elif USE_PMI2_API
    if (PMI2_SUCCESS != PMI2_Job_GetId(kvs_name, max_name_len)) {
        gasneti_fatalerror("PMI2_Job_GetId() failed");
    }
#else
    if (PMI_SUCCESS != PMI_KVS_Get_my_name(kvs_name, max_name_len)) {
        gasneti_fatalerror("PMI_KVS_Get_my_name() failed");
    }
#endif

    kvs_is_init = 1;
}

/* bootstrapFini
 */
static void bootstrapFini(void) {
#if USE_PMIX_API
    PMIx_Finalize(NULL, 0);
#elif USE_PMI2_API
    PMI2_Finalize();
#else
    if (PMI_FALSE == gasneti_pmi_initialized) {
        PMI_Finalize();
    }
#endif
}

/* bootstrapAbort
 */
static void bootstrapAbort(int exitcode) {
#if USE_PMIX_API
    PMIx_Abort(exitcode, "GASNet abnormal exit", NULL, 0);
#elif USE_PMI2_API
    PMI2_Abort(1, "GASNet abnormal exit");
#else
    PMI_Abort(exitcode, "GASNet abnormal exit");
#endif
    gasneti_fatalerror("PMI_Abort failed.");
    /* NOT REACHED */
}

/* bootstrapBarrier
 */
static void bootstrapBarrier(void) {
#if USE_PMIX_API
    if (PMIX_SUCCESS != PMIx_Fence(NULL, 0, NULL, 0)) {
        gasneti_fatalerror("barrier failed");
    }
#elif USE_PMI2_API
#if GASNETI_PMI2_FENCE_IS_BARRIER
    PMI2_KVS_Fence();
#else
    static unsigned counter;
    char v[16] = "";

    do_kvs_init();

    // This barrier assumes a worst-case "lazy" fence implementation.
    // TODO: is it possible to do even less than 1 Get per rank here?

    do_kvs_key0('X', counter);

  #if GASNET_DEBUG
    snprintf(v, sizeof(v), "%u", counter);
  #endif

    if (!gasneti_mynode) {
      OP_INC_MAP(X, counter);
      do_kvs_put(v, strlen(v), 0);
    }

    do_kvs_fence();

    do_kvs_get(v, strlen(v), 0);
  #if GASNET_DEBUG
    if (atoi(v) != counter) gasneti_fatalerror("barrier failed: exp %u got %s\n", counter, v);
  #endif

    counter++;
#endif
#else
    PMI_Barrier();
#endif
}

#if HAVE_PMI_ALLGATHER
static gex_Rank_t *gasnetc_pmi_allgather_order = NULL;
GASNETI_INLINE(gasnetc_pmi_allgather_init)
void gasnetc_pmi_allgather_init(void) {
    /* perform (just once) an Allgather of node number to establish the order */
    if_pf (!gasnetc_pmi_allgather_order) {
        int rc;
        gasnetc_pmi_allgather_order = gasneti_malloc(gasneti_nodes * sizeof(gex_Rank_t));
        rc = PMI_Allgather(&gasneti_mynode, gasnetc_pmi_allgather_order, sizeof(gex_Rank_t));
        gasneti_assert_always(PMI_SUCCESS == rc);
    }
}
#endif

/* bootstrapExchange
 */
static void bootstrapExchange(void *src, size_t len, void *dest) {
#if HAVE_PMI_ALLGATHER
    uint8_t *unsorted = gasneti_malloc(len * gasneti_nodes); /* TODO: use alloca()? */
    gex_Rank_t i;
    int rc;

    /* Allgather the callers data to a temporary array */
    gasnetc_pmi_allgather_init();
    rc = PMI_Allgather(src, unsorted, len);
    gasneti_assert_always(PMI_SUCCESS == rc);

    /* extract the records from the unsorted array by using the 'order' array */
    for (i = 0; i < gasneti_nodes; i += 1) {
      gex_Rank_t peer = gasnetc_pmi_allgather_order[i];
      gasneti_assert_always(peer < gasneti_nodes);
      GASNETI_MEMCPY((void *) ((uintptr_t) dest + (peer * len)), &unsorted[i * len], len);
    }

    gasneti_free(unsorted);
#else
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    do_kvs_init();

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);
        uint8_t *p;
        gex_Rank_t i;

        do_kvs_key1('E', counter, gasneti_mynode);
        do_kvs_put(s, chunk, 0);

        do_kvs_fence();

        for (i = 0, p = d; i < gasneti_nodes; ++i, p += len) {
            if (i == gasneti_mynode) continue;
            do_kvs_key1('E', counter, i);
            do_kvs_get(p, chunk, i);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        OP_INC_MAP(E, counter);
        ++counter;
    }

    GASNETI_MEMCPY_SAFE_IDENTICAL((uint8_t*)dest + len*gasneti_mynode, src, len);
#endif
}

#if USE_PMIX_API // only case that consumes is_local
static int peer_is_local(gex_Rank_t peer) {
    if (gasneti_myhost.nodes) { // could be called before nodemap has been initialized
        for (int i = 0; i < gasneti_myhost.node_count; ++i) {
            if (gasneti_myhost.nodes[i] == peer) return 1;
        }
    }
    return 0;
}
#else
  #define peer_is_local(peer) 0
#endif

/* bootstrapAlltoall
 */
static void bootstrapAlltoall(void *src, size_t len, void *dest) {
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    do_kvs_init();

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);
        uint8_t *p;
        gex_Rank_t i;

        for (i = 0, p = s; i < gasneti_nodes; ++i, p += len) {
            if (i == gasneti_mynode) continue;
            do_kvs_key2('A', counter, gasneti_mynode, i);
            do_kvs_put(p, chunk, peer_is_local(i));
        }

        do_kvs_fence();

        for (i = 0, p = d; i < gasneti_nodes; ++i, p += len) {
            if (i == gasneti_mynode) continue;
            do_kvs_key2('A', counter, i, gasneti_mynode);
            do_kvs_get(p, chunk, i);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        OP_INC_MAP(A, counter);
        ++counter;
    }

    GASNETI_MEMCPY_SAFE_IDENTICAL((uint8_t*)dest + len*gasneti_mynode, (uint8_t*)src + len*gasneti_mynode, len);
}

/* bootstrapBroadcast
 */
static void bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
#if HAVE_PMI_BCAST && 0
    /* TODO */
#else
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    do_kvs_init();

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);

        do_kvs_key0('B', counter);

        if (gasneti_mynode == rootnode) {
            OP_INC_MAP(B, counter);
            do_kvs_put(s, chunk, 0);
            do_kvs_fence();
        } else {
            do_kvs_fence();
            do_kvs_get(d, chunk, rootnode);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        ++counter;
    }

    if (gasneti_mynode == rootnode) {
        GASNETI_MEMCPY_SAFE_IDENTICAL(dest, src, len);
    }
#endif
}

#if HAVE_PMI_ALLGATHER_ON_SMP
static gex_Rank_t *gasnetc_pmi_allgather_on_smp_order = NULL;
GASNETI_INLINE(gasnetc_pmi_allgather_on_smp_init)
void gasnetc_pmi_allgather_on_smp_init(void) {
    /* perform (just once) an Allgather_on_smp of node number to establish the order */
    if_pf (!gasnetc_pmi_allgather_on_smp_order) {
        int count = gasneti_myhost.node_count;
        gasnetc_pmi_allgather_on_smp_order = gasneti_malloc(count * sizeof(gex_Rank_t));
        int rc = PMI_Allgather_on_smp(&gasneti_mynode, gasnetc_pmi_allgather_on_smp_order, sizeof(gex_Rank_t));
        gasneti_assert_always(PMI_SUCCESS == rc);
    }
}
#endif

static void bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode) {
#if HAVE_PMI_ALLGATHER_ON_SMP
    const int count = gasneti_myhost.node_count;
    uint8_t *tmp = gasneti_malloc(len * count);

    /* Allgather_on_smp the callers data to a temporary array */
    gasnetc_pmi_allgather_on_smp_init();
    int rc = PMI_Allgather_on_smp(src ? src : dest, tmp, len);
    gasneti_assert_always(PMI_SUCCESS == rc);

    /* Extract the right piece */
    int i;
    for (i = 0; i < count; ++i) {
      if (rootnode == gasnetc_pmi_allgather_on_smp_order[i]) {
        GASNETI_MEMCPY(dest, &tmp[i * len], len);
        break;
      }
    }
    gasneti_assert_always(i != count);

    gasneti_free(tmp);
#elif HAVE_PMI_ALLGATHER
    /* TODO: test our assumption that PMI_Allgather it better than the Put/Get code below */
    uint8_t *tmp = gasneti_malloc(len * gasneti_nodes);
    int rc, i;

    /* Allgather the data to the temporary array */
    gasnetc_pmi_allgather_init();
    rc = PMI_Allgather(src ? src : dest, tmp, len);
    gasneti_assert_always(PMI_SUCCESS == rc);

    /* Find the right piece */
    for (i = 0; i < gasneti_nodes; i += 1) {
        gex_Rank_t peer = gasnetc_pmi_allgather_order[i];
        if (peer == rootnode) {
            GASNETI_MEMCPY(dest, &tmp[i * len], len);
            break;
        }
    }

    gasneti_free(tmp);
#elif HAVE_PMI_BCAST && 0
    /* TODO - Need something here if Broadcast is ever implemented in terms of PMI_Bcast */
#else
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    do_kvs_init();

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);

        // encoding rootnode allows all SNode's bcast concurrently
        do_kvs_key1('S', counter, rootnode);

        if (gasneti_mynode == rootnode) {
            OP_INC_MAP(S, counter);
            do_kvs_put(s, chunk, 1);
            do_kvs_fence();
        } else {
            do_kvs_fence();
            do_kvs_get(d, chunk, rootnode);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        ++counter;
    }

    if (gasneti_mynode == rootnode) {
        GASNETI_MEMCPY_SAFE_IDENTICAL(dest, src, len);
    }
#endif
}

#if GASNETI_PMI_DUP_KEYS
// Put zero-length keys to free up space in KVS
static void bootstrapGC(void) {
    char value[] = "";
    size_t sz = 0;
    int phase;

    // First a barrier to ensure we cannot clobber keys being read by another proc
    // Consequently, we don't RESET_KVS(X,...) below.
    bootstrapBarrier();

    #define RESET_KVS(stem, key_gen, is_local) \
        do { \
            unsigned int map = op_##stem##_map; \
            op_##stem##_map = 0; \
            if (map & 1) { phase = 0; key_gen; do_kvs_put(value, sz, is_local); } \
            if (map & 2) { phase = 1; key_gen; do_kvs_put(value, sz, is_local); } \
        } while (0)
    // RESET_KVS(X, do_kvs_key0('X', phase), 0); - omit to avoid interference w/ barrier above
    RESET_KVS(E, do_kvs_key1('E', phase, gasneti_mynode), 0);
    RESET_KVS(B, do_kvs_key0('B', phase), 0);
    RESET_KVS(S, do_kvs_key1('S', phase, gasneti_mynode), 1);
    #undef RESET_KVS
    if (op_A_map & 1) {
        for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
            if (i == gasneti_mynode) continue;
            do_kvs_key2('A', 0, gasneti_mynode, i);
            do_kvs_put(value, sz, peer_is_local(i));
        }
    }
    if (op_A_map & 2) {
        for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
            if (i == gasneti_mynode) continue;
            do_kvs_key2('A', 1, gasneti_mynode, i);
            do_kvs_put(value, sz, peer_is_local(i));
        }
    }
    op_A_map = 0;

    do_kvs_fence();
}
#else
#define bootstrapGC() ((void)0)
#endif

static void bootstrapCleanup(void) {
  #if HAVE_PMI_ALLGATHER
    gasneti_free(gasnetc_pmi_allgather_order);
    gasnetc_pmi_allgather_order = NULL;
  #endif
  #if HAVE_PMI_ALLGATHER_ON_SMP
    gasneti_free(gasnetc_pmi_allgather_on_smp_order);
    gasnetc_pmi_allgather_on_smp_order = NULL;
  #endif

    bootstrapGC();

    if (kvs_is_init) {
        gasneti_free(kvs_name);  kvs_name = NULL;
        gasneti_free(kvs_key);   kvs_key = NULL;
        gasneti_free(kvs_value); kvs_value = NULL;

        kvs_is_init = 0;
    }
}

static gasneti_spawnerfn_t const spawnerfn = {
  bootstrapBarrier,
  bootstrapExchange,
  bootstrapBroadcast,
  bootstrapSNodeBroadcast,
  bootstrapAlltoall,
  bootstrapAbort,
  bootstrapCleanup,
  bootstrapFini,
};

