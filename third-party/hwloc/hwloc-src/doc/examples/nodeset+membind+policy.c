/* This example program plays with:
 * - finding and counting NUMA nodes
 * - manipulating nodesets
 * - memory binding and binding policies
 *
 * Copyright © 2014 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  hwloc_topology_t topology;
  hwloc_bitmap_t set;
  hwloc_const_bitmap_t cset;
  hwloc_membind_policy_t policy;
  const struct hwloc_topology_support *support;
  int nbnodes;
  hwloc_obj_t obj;
  char *buffer, *s;
  unsigned i;
  int err;

  /* create a topology */
  err = hwloc_topology_init(&topology);
  if (err < 0) {
    fprintf(stderr, "failed to initialize the topology\n");
    return EXIT_FAILURE;
  }
  err = hwloc_topology_load(topology);
  if (err < 0) {
    fprintf(stderr, "failed to load the topology\n");
    hwloc_topology_destroy(topology);
    return EXIT_FAILURE;
  }

  /* retrieve the entire set of NUMA nodes and count them */
  cset = hwloc_topology_get_topology_nodeset(topology);
  nbnodes = hwloc_bitmap_weight(cset);
  if (nbnodes <= 0) {
    /* nbnodes may be -1 when there's no NUMA information,
     * or 0 when the machine is known to be non-NUMA */
    printf("this machine is not NUMA, nothing to do\n");
    hwloc_topology_destroy(topology);
    return EXIT_SUCCESS;
  }
  printf("there are %d nodes in the machine\n", nbnodes);

  /* get the process memory binding as a nodeset */
  set = hwloc_bitmap_alloc();
  if (!set) {
    fprintf(stderr, "failed to allocate a bitmap\n");
    hwloc_topology_destroy(topology);
    return EXIT_FAILURE;
  }
  err = hwloc_get_membind_nodeset(topology, set, &policy, 0);
  if (err < 0) {
    fprintf(stderr, "failed to retrieve my memory binding and policy\n");
    hwloc_topology_destroy(topology);
    hwloc_bitmap_free(set);
    return EXIT_FAILURE;
  }

  /* print the corresponding NUMA nodes */
  hwloc_bitmap_asprintf(&s, set);
  printf("bound to nodeset %s with contains:\n", s);
  free(s);
  hwloc_bitmap_foreach_begin(i, set) {
    obj = hwloc_get_numanode_obj_by_os_index(topology, i);
    printf("  node #%u (OS index %u) with %lld bytes of memory\n",
	   obj->logical_index, i, (unsigned long long) obj->memory.local_memory);
  } hwloc_bitmap_foreach_end();
  hwloc_bitmap_free(set);

  /* allocate replicated memory of all nodes */
  support = hwloc_topology_get_support(topology);
  buffer = NULL;
  if (support->membind->replicate_membind) {
    printf("replicated memory binding is supported\n");
    buffer = hwloc_alloc_membind_nodeset(topology, 4096, cset, HWLOC_MEMBIND_REPLICATE, HWLOC_MEMBIND_STRICT);
  } else {
    printf("replicated memory binding is NOT supported\n");
  }
  if (!buffer) {
    /* could not allocate replicated memory, manually allocate of each node by iterating over them */
    printf("manually allocating memory on each node\n");
    obj = NULL;
    while ((obj = hwloc_get_next_obj_by_type(topology, HWLOC_OBJ_NUMANODE, obj)) != NULL) {
      buffer = hwloc_alloc_membind_nodeset(topology, 4096, obj->nodeset, HWLOC_MEMBIND_BIND, HWLOC_MEMBIND_STRICT);
      if (!buffer) {
	fprintf(stderr, "failed to manually allocate memory on node %u\n", obj->os_index);
	hwloc_topology_destroy(topology);
	return EXIT_SUCCESS;
      }
      /* now the application must manually manage these different buffers on different nodes */
    }
  }

  /* check where buffer is allocated */
  set = hwloc_bitmap_alloc();
  if (!set) {
    fprintf(stderr, "failed to allocate a bitmap\n");
    hwloc_topology_destroy(topology);
    return EXIT_FAILURE;
  }
  err = hwloc_get_area_membind_nodeset(topology, buffer, 4096, set, &policy, 0);
  if (err < 0) {
    fprintf(stderr, "failed to retrieve the buffer binding and policy\n");
    hwloc_topology_destroy(topology);
    hwloc_bitmap_free(set);
    return EXIT_FAILURE;
  }

  /* check the binding policy, it should be what we requested above,
   * but may be different if the implementation of different policies
   * is identical for the current operating system.
   */
  printf("buffer membind policy is %d while we requested %d or %d\n",
	 policy, HWLOC_MEMBIND_REPLICATE, HWLOC_MEMBIND_BIND);

  /* print the corresponding NUMA nodes */
  hwloc_bitmap_asprintf(&s, set);
  printf("buffer bound to nodeset %s with contains:\n", s);
  free(s);
  hwloc_bitmap_foreach_begin(i, set) {
    obj = hwloc_get_numanode_obj_by_os_index(topology, i);
    printf("  node #%u (OS index %u) with %lld bytes of memory\n",
	   obj->logical_index, i, (unsigned long long) obj->memory.local_memory);
  } hwloc_bitmap_foreach_end();
  hwloc_bitmap_free(set);

  /* try to migrate the buffer to the first node */
  obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, 0);
  err = hwloc_set_area_membind_nodeset(topology, buffer, 4096, obj->nodeset, HWLOC_MEMBIND_BIND, HWLOC_MEMBIND_MIGRATE);
  if (err < 0) {
    fprintf(stderr, "failed to migrate buffer\n");
    hwloc_topology_destroy(topology);
    return EXIT_FAILURE;
  }

  hwloc_topology_destroy(topology);
  return EXIT_SUCCESS;
}
