/*
 * Copyright (c) 2012 Mellanox Technologies LTD. All rights reserved.
 */

#ifndef _GASNET_FCA_TEAM_H
#define _GASNET_FCA_TEAM_H

#include <fca/fca_api.h>

typedef struct {
    fca_comm_t *fca_comm;
    fca_comm_desc_t     fca_comm_desc;
    fca_comm_caps_t     fca_comm_caps;
    int my_rank;
    int local_proc_idx;
    int num_local_procs;
    int *local_ranks;
    int proc_count;
} fca_comm_data_t;

#endif
