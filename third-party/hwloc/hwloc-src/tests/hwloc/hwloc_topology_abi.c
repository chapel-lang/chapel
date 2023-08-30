/*
 * Copyright © 2017-2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"
#include "private/debug.h" /* HWLOC_BUILD_ASSERT */
#include "private/private.h" /* for struct topology fields */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

int main(void)
{
  hwloc_topology_t topo, dup;
  int err;

  printf("loading and checking...\n");
  err = hwloc_topology_init(&topo);
  assert(!err);
  err = hwloc_topology_load(topo);
  assert(!err);
  err = hwloc_topology_abi_check(topo);
  assert(!err);

  printf("dupping and checking\n");
  err = hwloc_topology_dup(&dup, topo);
  assert(!err);
  err = hwloc_topology_abi_check(dup);
  assert(!err);
  hwloc_topology_destroy(dup);

#if (defined HWLOC_LINUX_SYS) && (defined HWLOC_X86_64_ARCH)
  if (!getenv("HWLOC_IGNORE_TOPOLOGY_ABI")) {
    size_t size, offset __hwloc_attribute_unused;
    printf("checking offsets and sizes in struct hwloc_topology for topology ABI 0x%x...\n", (unsigned) HWLOC_TOPOLOGY_ABI);

    /*******************************************************************
     * WARNING: if anything breaks below, the topology ABI has changed.
     * HWLOC_TOPOLOGY_ABI must be bumped when updating these checks.
     *******************************************************************/

    HWLOC_BUILD_ASSERT(HWLOC_OBJ_TYPE_MAX == 20);
    HWLOC_BUILD_ASSERT(HWLOC_NR_SLEVELS == 6);

    offset = offsetof(struct hwloc_topology, topology_abi);
    assert(offset == 0);

    offset = offsetof(struct hwloc_topology, adopted_shmem_addr);
    assert(offset == 232);

    offset = offsetof(struct hwloc_topology, binding_hooks);
    assert(offset == 456);
    size = sizeof(struct hwloc_binding_hooks);
    assert(size == 192);

    offset = offsetof(struct hwloc_topology, support);
    assert(offset == 648);

    offset = offsetof(struct hwloc_topology, first_dist);
    assert(offset == 704);
    size = sizeof(struct hwloc_internal_distances_s);
    assert(size == 88);

    offset = offsetof(struct hwloc_topology, memattrs);
    assert(offset == 728);
    size = sizeof(struct hwloc_internal_memattr_s);
    assert(size == 32);
    size = sizeof(struct hwloc_internal_memattr_target_s);
    assert(size == 48);
    size = sizeof(struct hwloc_internal_memattr_initiator_s);
    assert(size == 40);
    size = sizeof(struct hwloc_internal_location_s);
    assert(size == 32);

    offset = offsetof(struct hwloc_topology, cpukinds);
    assert(offset == 744);
    size = sizeof(struct hwloc_internal_cpukind_s);
    assert(size == 40);

    offset = offsetof(struct hwloc_topology, grouping_next_subkind);
    assert(offset == 784);

    /* fields after this one aren't needed after discovery */

    /* check bitmap ABI too, but those fields are private to bitmap.c */
    printf("checking bitmaps for topology ABI 0x%x...\n", (unsigned) HWLOC_TOPOLOGY_ABI);
    {
      hwloc_bitmap_t set = hwloc_bitmap_alloc();
      unsigned *ulongs_count =     (unsigned*)       (((char*)set)   );
      unsigned *ulongs_allocated = (unsigned*)       (((char*)set)+4 );
      unsigned long **ulongs =     (unsigned long**) (((char*)set)+8 );
      int *infinite =              (int*)            (((char*)set)+16);

      /* empty set */
      assert(*ulongs_count >= 1);
      assert(*ulongs_allocated >= *ulongs_count);
      assert((*ulongs)[0] == 0UL);
      assert(!*infinite);

      /* 260th bit */
      hwloc_bitmap_set(set, 260);
      assert(*ulongs_count >= 5);
      assert(*ulongs_allocated >= *ulongs_count);
      assert((*ulongs)[0] == 0UL);
      assert((*ulongs)[1] == 0UL);
      assert((*ulongs)[2] == 0UL);
      assert((*ulongs)[3] == 0UL);
      assert((*ulongs)[4] == 0x10UL);
      assert(!*infinite);

      /* full set */
      hwloc_bitmap_fill(set);
      assert((*ulongs)[0] == ~0UL);
      assert(*infinite);

      hwloc_bitmap_free(set);
    }

    /*******************************************************************
     * WARNING: if anything breaks above, the topology ABI has changed.
     * HWLOC_TOPOLOGY_ABI must be bumped when updating these checks.
     *******************************************************************/

    printf("checking sizes of public objects...\n");

    /* if these first lines break, you may be breaking the main library ABI.
     * adding fields may be OK. changing/removing likely isn't.
     */
    size = sizeof(struct hwloc_obj);
    assert(size == 248);
    size = sizeof(union hwloc_obj_attr_u);
#ifdef HWLOC_HAVE_32BITS_PCI_DOMAIN
    assert(size == 48);
#else
    assert(size == 40);
#endif
    size = sizeof(struct hwloc_info_s);
    assert(size == 16);

    size = sizeof(struct hwloc_topology_support);
    assert(size == 32);
    size = sizeof(struct hwloc_topology_discovery_support);
    assert(size == 6);
    size = sizeof(struct hwloc_topology_cpubind_support);
    assert(size == 11);
    size = sizeof(struct hwloc_topology_membind_support);
    assert(size == 15);
    size = sizeof(struct hwloc_topology_misc_support);
    assert(size == 1);

    size = sizeof(struct hwloc_distances_s);
    assert(size == 32);

    size = sizeof(struct hwloc_location);
    assert(size == 16);

    size = sizeof(union hwloc_topology_diff_u);
    assert(size == 56);
    size = sizeof(union hwloc_topology_diff_obj_attr_u);
    assert(size == 32);

  } else {
    /* if building with non-binary-compatible compiler flags */
    printf("checking topology ABI disabled by environment variable HWLOC_IGNORE_TOPOLOGY_ABI\n");
  }
#else /* !LINUX || !X86_64 */
  printf("checking topology ABI disabled, not running on Linux/x86_64\n");
#endif /* !LINUX || !X86_64 */

  hwloc_topology_destroy(topo);

  return 0;
}
