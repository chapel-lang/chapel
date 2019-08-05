/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/pmi-spawner/gasnet_bootstrap_pmi.c $
 * Description: GASNet conduit-independent pmi-based spawner
 * Copyright 2013, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */
#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

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
#else
#  error "Unknown path to PMI header"
#endif

/* Sigh.  For portals4/pmi.h and perhaps others. */
#ifndef PMI_TRUE
   typedef int PMI_BOOL;
#  define PMI_TRUE     1
#  define PMI_FALSE    0
#endif

GASNETI_IDENT(gasnetc_IdentString_HavePMISpawner, "$GASNetPMISpawner: 1 $");

static gasneti_spawnerfn_t const spawnerfn;

static char *kvs_name = NULL, *kvs_key = NULL, *kvs_value = NULL;
static int max_name_len, max_key_len, max_val_len;
static int max_val_bytes;

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

/* Put/Get/Fence wrappers */

GASNETI_INLINE(do_kvs_put)
void do_kvs_put(void *value, size_t sz) {
    int rc;
    do_encode(value, sz);
#if USE_PMI2_API
    rc = PMI2_KVS_Put(kvs_key, kvs_value);
    gasneti_assert_always(PMI2_SUCCESS == rc);
#else
    rc = PMI_KVS_Put(kvs_name, kvs_key, kvs_value);
    gasneti_assert_always(PMI_SUCCESS == rc);
#endif
}

GASNETI_INLINE(do_kvs_get)
void do_kvs_get(void *value, size_t sz) {
    int rc;
#if USE_PMI2_API
    int len;
    rc = PMI2_KVS_Get(kvs_name, PMI2_ID_NULL, kvs_key, kvs_value, max_val_len, &len);
    gasneti_assert_always(PMI2_SUCCESS == rc);
    gasneti_assert_always(len > 0); // Negative would mean value larger than max_val_len
#else
    rc = PMI_KVS_Get(kvs_name, kvs_key, kvs_value, max_val_len);
    gasneti_assert_always(PMI_SUCCESS == rc);
    size_t len = strlen(kvs_value);
#endif
    do_decode(value, sz, len);
}

GASNETI_INLINE(do_kvs_fence)
void do_kvs_fence(void) {
#if USE_PMI2_API
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

#if USE_PMI2_API
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

#if USE_PMI2_API
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

    kvs_name = (char*) gasneti_malloc(max_name_len);
    kvs_key = (char*) gasneti_malloc(max_key_len);
    kvs_value = (char*) gasneti_malloc(max_val_len);
    max_val_bytes = 4 * (max_val_len / 5);

#if USE_PMI2_API
    if (PMI2_SUCCESS != PMI2_Job_GetId(kvs_name, max_name_len)) {
        gasneti_fatalerror("PMI2_Job_GetId() failed");
    }
#else
    if (PMI_SUCCESS != PMI_KVS_Get_my_name(kvs_name, max_name_len)) {
        gasneti_fatalerror("PMI_KVS_Get_my_name() failed");
    }
#endif

    return &spawnerfn;
}

/* bootstrapFini
 */
static void bootstrapFini(void) {
#if USE_PMI2_API
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
#if USE_PMI2_API
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
#if USE_PMI2_API
#if GASNETI_PMI2_FENCE_IS_BARRIER
    PMI2_KVS_Fence();
#else
    static unsigned counter;
    char v[16];
    int i;

    snprintf(kvs_key, max_key_len, "B%u-%u", counter, (unsigned)gasneti_mynode);
    snprintf(v, sizeof(v), "%u", counter);

    do_kvs_put(v, sizeof(v));
    do_kvs_fence();

    for (i = 0; i < gasneti_nodes; ++i) {
        if (i == gasneti_mynode) continue;
        snprintf(kvs_key, max_key_len, "B%u-%u", counter, (unsigned)i);
        do_kvs_get(v, sizeof(v));
        if (atoi(v) != counter) gasneti_fatalerror("barrier failed: exp %u got %s\n", counter, v);
    }
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
      memcpy((void *) ((uintptr_t) dest + (peer * len)), &unsorted[i * len], len);
    }

    gasneti_free(unsorted);
#else
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);
        uint8_t *p;
        gex_Rank_t i;

        snprintf(kvs_key, max_key_len, "GNE%x-%x", counter, (unsigned int)gasneti_mynode);
        do_kvs_put(s, chunk);

        do_kvs_fence();

        for (i = 0, p = d; i < gasneti_nodes; ++i, p += len) {
            if (i == gasneti_mynode) continue;
            snprintf(kvs_key, max_key_len, "GNE%x-%x", counter, (unsigned int)i);
            do_kvs_get(p, chunk);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        ++counter;
    }

    memcpy((uint8_t*)dest + len*gasneti_mynode, src, len);
#endif
}

/* bootstrapAlltoall
 */
static void bootstrapAlltoall(void *src, size_t len, void *dest) {
    static unsigned int counter = 0;
    size_t remain = len;
    uint8_t *s = src;
    uint8_t *d = dest;

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);
        uint8_t *p;
        gex_Rank_t i;

        for (i = 0, p = s; i < gasneti_nodes; ++i, p += len) {
            snprintf(kvs_key, max_key_len, "GNA%x-%x.%x", counter, (unsigned int)gasneti_mynode, (unsigned int)i);
            do_kvs_put(p, chunk);
        }

        do_kvs_fence();

        for (i = 0, p = d; i < gasneti_nodes; ++i, p += len) {
            if (i == gasneti_mynode) continue;
            snprintf(kvs_key, max_key_len, "GNA%x-%x.%x", counter, (unsigned int)i, (unsigned int)gasneti_mynode);
            do_kvs_get(p, chunk);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        ++counter;
    }

    memcpy((uint8_t*)dest + len*gasneti_mynode, (uint8_t*)src + len*gasneti_mynode, len);
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

    while (remain) {
        size_t chunk = MIN(remain, max_val_bytes);

        snprintf(kvs_key, max_key_len, "GNB%x-%x", counter, rootnode);

        if (gasneti_mynode == rootnode) {
            do_kvs_put(s, chunk);
            do_kvs_fence();
        } else {
            do_kvs_fence();
            do_kvs_get(d, chunk);
        }

        s += chunk;
        d += chunk;
        remain -= chunk;
        ++counter;
    }

    if (gasneti_mynode == rootnode) {
        memcpy(dest, src, len);
    }
#endif
}

static void bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode) {
#if HAVE_PMI_ALLGATHER
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
            memcpy(dest, &tmp[i * len], len);
            break;
        }
    }

    gasneti_free(tmp);
#elif HAVE_PMI_BCAST && 0
    /* TODO - Need something here if Broadcast is ever implemented in terms of PMI_Bcast */
#else
    bootstrapBroadcast(src, len, dest, rootnode);
#endif
}

static void bootstrapCleanup(void) {
  #if HAVE_PMI_ALLGATHER
    gasneti_free(gasnetc_pmi_allgather_order);
    gasnetc_pmi_allgather_order = NULL;
  #endif
    gasneti_free(kvs_name);  kvs_name = NULL;
    gasneti_free(kvs_key);   kvs_key = NULL;
    gasneti_free(kvs_value); kvs_value = NULL;
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
#if GASNET_BLCR && 0 // BLCR-TODO
  bootstrapPreCheckpoint,
  bootstrapPostCheckpoint,
  bootstrapRollback,
#endif
};

