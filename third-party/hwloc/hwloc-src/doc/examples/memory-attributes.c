/* This example program plays with:
 * - finding local NUMA nodes
 * - finding the best NUMA nodes for bandwidth/latency
 * - displaying the bandwidth/latency values of NUMA nodes
 * - allocating on the best NUMA node for bandwidth
 *
 * Copyright © 2022 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    hwloc_topology_t topology;
    hwloc_obj_t core, *nodes, bestnode;
    struct hwloc_location initiator;
    unsigned i,n;
    char *s, *buffer;
    int err;

    /* Allocate, initialize and load topology object. */
    hwloc_topology_init(&topology);
    hwloc_topology_load(topology);

    /* Find max number of NUMA nodes to allocate the array for hwloc_get_local_numanode_objs() */
    n = hwloc_bitmap_weight(hwloc_topology_get_topology_nodeset(topology));
    printf("There are %u NUMA nodes\n", n);
    nodes = malloc(n * sizeof(*nodes));
    assert(nodes);

    /* Take the first core */
    core = hwloc_get_obj_by_type(topology, HWLOC_OBJ_CORE, 0);
    if (!core)
      goto out;

    hwloc_bitmap_asprintf(&s, core->cpuset);
    printf("Core L#0 cpuset = %s\n", s);
    free(s);

    /* setup the initiator to the first core cpuset */
    initiator.type = HWLOC_LOCATION_TYPE_CPUSET;
    initiator.location.cpuset = core->cpuset;

    /* get local NUMA nodes and display their attributes */
    err = hwloc_get_local_numanode_objs(topology, &initiator, &n, nodes,
                                        /* we want NUMA nodes that are local to that core or to more */
                                        HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY);
    printf("Found %u local NUMA nodes\n", n);
    for(i=0; i<n; i++) {
      hwloc_uint64_t latency, bandwidth;

      printf("NUMA node L#%u P#%u (subtype %s) is local to core L#0\n", nodes[i]->logical_index, nodes[i]->os_index, nodes[i]->subtype);

      err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_BANDWIDTH, nodes[i], &initiator, 0, &bandwidth);
      if (err < 0) {
        printf("  bandwidth is unknown\n");
      } else {
        printf("  bandwidth = %llu MiB/s\n", (unsigned long long) bandwidth);
      }
      err = hwloc_memattr_get_value(topology, HWLOC_MEMATTR_ID_LATENCY, nodes[i], &initiator, 0, &latency);
      if (err < 0) {
        printf("  latency is unknown\n");
      } else {
        printf("  latency = %llu ns\n", (unsigned long long) latency);
      }
    }
    free(nodes);

    /* allocate on best-bandwidth node */
    err = hwloc_memattr_get_best_target(topology, HWLOC_MEMATTR_ID_BANDWIDTH, &initiator, 0, &bestnode, NULL);
    if (err < 0) {
      printf("Couldn't find best NUMA node for bandwidth to core L#0\n");
    } else {
      printf("Best bandwidth NUMA node for core L#0 is L#%u P#%u\n", bestnode->logical_index, bestnode->os_index);
      /* allocate memory on best node */
      buffer = hwloc_alloc_membind(topology, 1048576, bestnode->nodeset, HWLOC_MEMBIND_BIND, HWLOC_MEMBIND_BYNODESET);
      printf("Allocated buffer %p on best node\n", buffer);
      free(buffer);
    }

 out:
    /* Destroy topology object. */
    hwloc_topology_destroy(topology);

    return 0;
}
