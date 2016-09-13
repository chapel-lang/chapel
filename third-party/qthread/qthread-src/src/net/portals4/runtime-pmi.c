/* -*- C -*-
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S.  Government
 * retains certain rights in this software.
 *
 * Run-time support for the built-in runtime that is part of the
 * shared memory implementation Portals
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <portals4.h>
#ifdef PMI_SLURM
#include <slurm/pmi.h>
#else
#include <pmi.h>
#endif

#include "runtime.h"

#include "qt_debug.h" /* for malloc debug wrappers */

struct map_t {
    ptl_handle_ni_t handle;
    ptl_process_t *mapping;
};

static int rank = -1;
static int size = 0;
struct map_t maps[4] = { { PTL_INVALID_HANDLE, NULL },
                         { PTL_INVALID_HANDLE, NULL },
                         { PTL_INVALID_HANDLE, NULL },
                         { PTL_INVALID_HANDLE, NULL } };

static int
encode(const void *inval, int invallen, char *outval, int outvallen)
{
    static unsigned char encodings[] = {
        '0','1','2','3','4','5','6','7', \
        '8','9','a','b','c','d','e','f' };
    int i;

    if (invallen * 2 + 1 > outvallen) {
        return 1;
    }

    for (i = 0; i < invallen; i++) {
        outval[2 * i] = encodings[((unsigned char *)inval)[i] & 0xf];
        outval[2 * i + 1] = encodings[((unsigned char *)inval)[i] >> 4];
    }

    outval[invallen * 2] = '\0';

    return 0;
}


static int
decode(const char *inval, void *outval, int outvallen)
{
    int i;
    char *ret = (char*) outval;

    if (outvallen != strlen(inval) / 2) {
        return 1;
    }

    for (i = 0 ; i < outvallen ; ++i) {
        if (*inval >= '0' && *inval <= '9') {
            ret[i] = *inval - '0';
        } else {
            ret[i] = *inval - 'a' + 10;
        }
        inval++;
        if (*inval >= '0' && *inval <= '9') {
            ret[i] |= ((*inval - '0') << 4);
        } else {
            ret[i] |= ((*inval - 'a' + 10) << 4);
        }
        inval++;
    }

    return 0;
}

int
qthread_internal_net_driver_runtime_init(void)
{
    int initialized;

    if (PMI_SUCCESS != PMI_Initialized(&initialized)) {
        return 1;
    }

    if (!initialized) {
        if (PMI_SUCCESS != PMI_Init(&initialized)) {
            return 2;
        }
    }

    if (PMI_SUCCESS != PMI_Get_rank(&rank)) {
        return 3;
    }

    if (PMI_SUCCESS != PMI_Get_size(&size)) {
        return 4;
    }

    return 0;
}


int
qthread_internal_net_driver_runtime_fini(void)
{
    int i;

    for (i = 0 ; i < 4 ; ++i) {
        if (NULL != maps[i].mapping) {
            FREE(maps[i].mapping, sizeof(ptl_process_t) * size);
        }
    }

    PMI_Finalize();

    return 0;
}


ptl_process_t*
qthread_internal_net_driver_runtime_get_mapping(ptl_handle_ni_t ni_h)
{
    int i, ret, max_name_len, max_key_len, max_val_len;
    char *name, *key, *val;
    ptl_process_t my_id;
    struct map_t *map = NULL;
    
    for (i = 0 ; i < 4 ; ++i) {
        if (maps[i].handle == ni_h) {
            return maps[i].mapping;
        }
    }

    for (i = 0 ; i < 4 ; ++i) {
        if (PTL_INVALID_HANDLE == maps[i].handle) {
            map = &maps[i];
            break;
        }
    }

    if (NULL == map) return NULL;

    if (PMI_SUCCESS != PMI_KVS_Get_name_length_max(&max_name_len)) {
        return NULL;
    }
    name = (char*) MALLOC(max_name_len);
    if (NULL == name) return NULL;

    if (PMI_SUCCESS != PMI_KVS_Get_key_length_max(&max_key_len)) {
        return NULL;
    }
    key = (char*) MALLOC(max_key_len);
    if (NULL == key) return NULL;

    if (PMI_SUCCESS != PMI_KVS_Get_value_length_max(&max_val_len)) {
        return NULL;
    }
    val = (char*) MALLOC(max_val_len);
    if (NULL == val) return NULL;

    ret = PtlGetPhysId(ni_h, &my_id);
    if (PTL_OK != ret) return NULL;

    if (PMI_SUCCESS != PMI_KVS_Get_my_name(name, max_name_len)) {
        return NULL;
    }

    /* put my information */
    snprintf(key, max_key_len, "qthreads-%lu-%lu-nid", 
             (long unsigned) ni_h, (long unsigned) rank);
    if (0 != encode(&my_id.phys.nid, sizeof(my_id.phys.nid), val, 
                    max_val_len)) {
        return NULL;
    }
    if (PMI_SUCCESS != PMI_KVS_Put(name, key, val)) {
        return NULL;
    }

    snprintf(key, max_key_len, "qthreads-%lu-%lu-pid",
             (long unsigned) ni_h, (long unsigned) rank);
    if (0 != encode(&my_id.phys.pid, sizeof(my_id.phys.pid), val, 
                    max_val_len)) {
        return NULL;
    }
    if (PMI_SUCCESS != PMI_KVS_Put(name, key, val)) {
        return NULL;
    }

    if (PMI_SUCCESS != PMI_KVS_Commit(name)) {
        return NULL;
    }

    if (PMI_SUCCESS != PMI_Barrier()) {
        return NULL;
    }

    /* get everyone's information */
    map->mapping = MALLOC(sizeof(ptl_process_t) * size);
    if (NULL == map->mapping) return NULL;

    for (i = 0 ; i < size ; ++i) {
        snprintf(key, max_key_len, "qthreads-%lu-%lu-nid",
                 (long unsigned) ni_h, (long unsigned) i);
        if (PMI_SUCCESS != PMI_KVS_Get(name, key, val, max_val_len)) {
            return NULL;
        }
        if (0 != decode(val, &(map->mapping)[i].phys.nid, 
                        sizeof((map->mapping)[i].phys.nid))) {
            return NULL;
        }

        snprintf(key, max_key_len, "qthreads-%lu-%lu-pid",
                 (long unsigned) ni_h, (long unsigned) i);
        if (PMI_SUCCESS != PMI_KVS_Get(name, key, val, max_val_len)) {
            return NULL;
        }
        if (0 != decode(val, &(map->mapping)[i].phys.pid,
                        sizeof((map->mapping)[i].phys.pid))) {
            return NULL;
        }
    }

    return map->mapping;
}


void
qthread_internal_net_driver_runtime_barrier(void)
{
    PMI_Barrier();
}


int
qthread_internal_net_driver_get_rank(void)
{
    return rank;
}


int
qthread_internal_net_driver_get_size(void)
{
    return size;
}

