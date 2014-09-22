/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2011 inria.  All rights reserved.
 * Copyright © 2009 Université Bordeaux 1
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* check hwloc_bitmap_asprintf(), hwloc_obj_cpuset_snprintf() and hwloc_bitmap_sscanf() */

static void check_cpuset(hwloc_bitmap_t set, const char *expected)
{
  hwloc_bitmap_t set2 = hwloc_bitmap_alloc();
  char *string = NULL;

  hwloc_bitmap_asprintf(&string, set);
  if (expected && strcmp(string, expected))
    assert(0);
  hwloc_bitmap_sscanf(set2, string);
  free(string);
  assert(hwloc_bitmap_isequal(set, set2));

  hwloc_bitmap_list_asprintf(&string, set);
  hwloc_bitmap_list_sscanf(set2, string);
  free(string);
  assert(hwloc_bitmap_isequal(set, set2));

  hwloc_bitmap_taskset_asprintf(&string, set);
  hwloc_bitmap_taskset_sscanf(set2, string);
  free(string);
  assert(hwloc_bitmap_isequal(set, set2));

  hwloc_bitmap_free(set2);
}

int main(void)
{
  hwloc_topology_t topology;
  unsigned depth;
  char *string = NULL;
  int stringlen, len;
  hwloc_obj_t obj;
  hwloc_bitmap_t set;

  /* check an empty cpuset */
  set = hwloc_bitmap_alloc();
  check_cpuset(set, "0x0");
  hwloc_bitmap_free(set);
  printf("empty cpuset converted back and forth, ok\n");

  /* check a full (and infinite) cpuset */
  set = hwloc_bitmap_alloc_full();
  check_cpuset(set, "0xf...f");
  hwloc_bitmap_free(set);
  printf("full cpuset converted back and forth, ok\n");

  /* check an infinite (but non full) cpuset */
  set = hwloc_bitmap_alloc_full();
  hwloc_bitmap_clr(set, 173);
  hwloc_bitmap_clr_range(set, 60, 70);
  check_cpuset(set, NULL);
  hwloc_bitmap_free(set);
  printf("infinite/nonfull cpuset converted back and forth, ok\n");

  /* check an finite cpuset */
  set = hwloc_bitmap_alloc();
  hwloc_bitmap_set(set, 2);
  hwloc_bitmap_set_range(set, 67, 70);
  check_cpuset(set, "0x00000078,,0x00000004");
  hwloc_bitmap_free(set);
  printf("finite/nonnull cpuset converted back and forth, ok\n");

  hwloc_topology_init(&topology);
  hwloc_topology_set_synthetic(topology, "6 5 4 3 2");
  hwloc_topology_load(topology);
  depth = hwloc_topology_get_depth(topology);

  obj = hwloc_get_root_obj(topology);
  stringlen = hwloc_bitmap_asprintf(&string, obj->cpuset);
  printf("system cpuset is %s\n", string);
  check_cpuset(obj->cpuset, NULL);
  printf("system cpuset converted back and forth, ok\n");

  printf("truncating system cpuset to NULL buffer\n");
  len = hwloc_obj_cpuset_snprintf(NULL, 0, 1, &obj);
  assert(len == stringlen);

  printf("truncating system cpuset to 0 char (no modification)\n");
  memset(string, 'X', 1);
  string[1] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 0, 1, &obj);
  assert(len == stringlen);
  assert(string[0] == 'X');

  printf("truncating system cpuset to 1 char (empty string)\n");
  memset(string, 'X', 2);
  string[2] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 1, 1, &obj);
  printf("got %s\n", string);
  assert(len == stringlen);
  assert(string[0] == 0);
  assert(string[1] == 'X');

  printf("truncating system cpuset to 10 chars (single 32bit subset except last char)\n");
  memset(string, 'X', 11);
  string[11] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 10, 1, &obj);
  printf("got %s\n", string);
  assert(len == stringlen);
  assert(string[8] == 'f');
  assert(string[9] == 0);
  assert(string[10] == 'X');

  printf("truncating system cpuset to 11 chars (single 32bit subset)\n");
  memset(string, 'X', 12);
  string[12] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 11, 1, &obj);
  printf("got %s\n", string);
  assert(len == stringlen);
  assert(string[9] == 'f');
  assert(string[10] == 0);
  assert(string[11] == 'X');

  printf("truncating system cpuset to 23 chars (two 32bit subsets with ending comma)\n");
  memset(string, 'X', 24);
  string[24] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 23, 1, &obj);
  printf("got %s\n", string);
  assert(len == stringlen);
  assert(string[20] == 'f');
  assert(string[21] == ',');
  assert(string[22] == 0);
  assert(string[23] == 'X');

  printf("truncating system cpuset to 51 chars (truncate to four and a half 32bit subsets)\n");
  memset(string, 'X', 52);
  string[52] = 0;
  len = hwloc_obj_cpuset_snprintf(string, 51, 1, &obj);
  printf("got %s\n", string);
  assert(len == stringlen);
  assert(string[49] == 'f');
  assert(string[50] == 0);
  assert(string[51] == 'X');

  obj = hwloc_get_obj_by_depth(topology, depth-1, 0);
  hwloc_obj_cpuset_snprintf(string, stringlen+1, 1, &obj);
  printf("first cpu cpuset is %s\n", string);
  check_cpuset(obj->cpuset, NULL);
  printf("first cpu cpuset converted back and forth, ok\n");

  obj = hwloc_get_obj_by_depth(topology, depth-1, hwloc_get_nbobjs_by_depth(topology, depth-1) - 1);
  hwloc_obj_cpuset_snprintf(string, stringlen+1, 1, &obj);
  printf("last cpu cpuset is %s\n", string);
  check_cpuset(obj->cpuset, NULL);
  printf("last cpu cpuset converted back and forth, ok\n");

  free(string);

  hwloc_topology_destroy(topology);

  return 0;
}
