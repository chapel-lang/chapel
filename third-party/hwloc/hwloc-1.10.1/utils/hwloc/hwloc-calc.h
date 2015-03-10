/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2014 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux 1
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_CALC_H
#define HWLOC_CALC_H

#include <private/private.h>
#include <private/misc.h>
#include <hwloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <ctype.h>
#include <assert.h>

typedef enum hwloc_calc_append_mode_e {
  HWLOC_CALC_APPEND_ADD,
  HWLOC_CALC_APPEND_CLR,
  HWLOC_CALC_APPEND_AND,
  HWLOC_CALC_APPEND_XOR
} hwloc_calc_append_mode_t;

static __hwloc_inline int
hwloc_calc_append_cpuset(hwloc_bitmap_t set, hwloc_const_bitmap_t newset,
		       hwloc_calc_append_mode_t mode, int verbose)
{
  char *s1, *s2;
  hwloc_bitmap_asprintf(&s1, newset);
  hwloc_bitmap_asprintf(&s2, set);
  switch (mode) {
  case HWLOC_CALC_APPEND_ADD:
    if (verbose > 0)
      fprintf(stderr, "adding %s to %s\n",
          s1, s2);
    hwloc_bitmap_or(set, set, newset);
    break;
  case HWLOC_CALC_APPEND_CLR:
    if (verbose > 0)
      fprintf(stderr, "clearing %s from %s\n",
          s1, s2);
    hwloc_bitmap_andnot(set, set, newset);
    break;
  case HWLOC_CALC_APPEND_AND:
    if (verbose > 0)
      fprintf(stderr, "and'ing %s from %s\n",
          s1, s2);
    hwloc_bitmap_and(set, set, newset);
    break;
  case HWLOC_CALC_APPEND_XOR:
    if (verbose > 0)
      fprintf(stderr, "xor'ing %s from %s\n",
          s1, s2);
    hwloc_bitmap_xor(set, set, newset);
    break;
  default:
    assert(0);
  }
  free(s1);
  free(s2);
  return 0;
}

static __hwloc_inline hwloc_obj_t __hwloc_attribute_pure
hwloc_calc_get_obj_inside_cpuset_by_depth(hwloc_topology_t topology, hwloc_const_bitmap_t rootset,
					 unsigned depth, unsigned i, int logical)
{
  if (logical) {
    return hwloc_get_obj_inside_cpuset_by_depth(topology, rootset, depth, i);
  } else {
    hwloc_obj_t obj = NULL;
    while ((obj = hwloc_get_next_obj_inside_cpuset_by_depth(topology, rootset, depth, obj)) != NULL) {
      if (obj->os_index == i)
        return obj;
    }
    return NULL;
  }
}

static __hwloc_inline int
hwloc_calc_depth_of_type(hwloc_topology_t topology, hwloc_obj_type_t type,
			 int depthattr, hwloc_obj_cache_type_t cachetype /* -1 if not specified */,
			 int verbose)
{
  int depth;
  int i;

  if (depthattr == -1) {
    hwloc_obj_type_t realtype;
    /* matched a type without depth attribute, try to get the depth from the type if it exists and is unique */
    if (type == HWLOC_OBJ_PCI_DEVICE || type == HWLOC_OBJ_OS_DEVICE || type == HWLOC_OBJ_BRIDGE)
      return -1;
    depth = hwloc_get_type_or_above_depth(topology, type);
    if (depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
      if (verbose >= 0)
	fprintf(stderr, "type %s has multiple possible depths\n", hwloc_obj_type_string(type));
      return -1;
    } else if (depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
      if (verbose >= 0)
	fprintf(stderr, "type %s isn't available\n", hwloc_obj_type_string(type));
      return -1;
    }
    realtype = hwloc_get_depth_type(topology, depth);
    if (type != realtype && verbose > 0)
      fprintf(stderr, "using type %s (depth %d) instead of %s\n",
	      hwloc_obj_type_string(realtype), depth, hwloc_obj_type_string(type));
    return depth;

  } else {
    /* matched a type with a depth attribute, look at the first object of each level to find the depth */
    if (type == HWLOC_OBJ_GROUP)
      for(i=0; ; i++) {
	hwloc_obj_t obj = hwloc_get_obj_by_depth(topology, i, 0);
	if (!obj) {
	  if (verbose >= 0)
	    fprintf(stderr, "Group with custom depth %d does not exist\n",
		    depthattr);
	  return -1;
	}
	if (obj->type == type
	    && (unsigned) depthattr == obj->attr->group.depth)
	  return i;
      }
    else if (type == HWLOC_OBJ_CACHE) {
      depth = hwloc_get_cache_type_depth(topology, depthattr, cachetype);
      if (verbose >= 0) {
	if (depth == HWLOC_TYPE_DEPTH_UNKNOWN)
	  fprintf(stderr, "Cache with custom depth %d and type %d does not exist\n", depthattr, cachetype);
	else if (depth == HWLOC_TYPE_DEPTH_MULTIPLE)
	  fprintf(stderr, "Cache with custom depth %d and type %d has multiple possible depths\n", depthattr, cachetype);
      }
      return depth;
    } else
      assert(0);
  }

  /* cannot come here, we'll exit above first */
  return -1;
}

static __hwloc_inline int
hwloc_calc_parse_depth_prefix(hwloc_topology_t topology, unsigned topodepth,
			      const char *string, size_t typelen,
			      hwloc_obj_type_t *typep,
			      int verbose)
{
  char typestring[20+1]; /* large enough to store all type names, even with a depth attribute */
  hwloc_obj_type_t type;
  hwloc_obj_cache_type_t cachetypeattr;
  int depthattr;
  int depth;
  char *end;
  int err;

  if (typelen >= sizeof(typestring)) {
    if (verbose >= 0)
      fprintf(stderr, "invalid type name %s\n", string);
    return -1;
  }
  strncpy(typestring, string, typelen);
  typestring[typelen] = '\0';

  /* try to match a type name */
  err = hwloc_obj_type_sscanf(typestring, &type, &depthattr, &cachetypeattr, sizeof(cachetypeattr));
  if (!err) {
    *typep = type;
    return hwloc_calc_depth_of_type(topology, type, depthattr, cachetypeattr, verbose);
  }

  /* try to match a numeric depth */
  depth = strtol(string, &end, 0);
  if (end != &string[typelen]) {
    if (verbose >= 0)
      fprintf(stderr, "invalid type name %s\n", string);
    return -1;
  }
  if ((unsigned) depth >= topodepth) {
    if (verbose >= 0)
      fprintf(stderr, "ignoring invalid depth %u\n", depth);
    return -1;
  }
  *typep = (hwloc_obj_type_t) -1;
  return depth;
}

static __hwloc_inline int
hwloc_calc_parse_range(const char *_string,
		       int *firstp, int *amountp, int *stepp, int *wrapp,
		       const char **dotp)
{
  char string[65];
  size_t len;
  char *dot, *end, *end2;
  long first, last, amount;
  int wrap;

  dot = strchr(_string, '.');
  *dotp = dot;
  if (dot) {
    len = dot - _string;
  } else {
    len = strlen(_string);
  }
  if (len >= sizeof(string)) {
    fprintf(stderr, "invalid range `%s', too long\n", string);
    return -1;
  }
  memcpy(string, _string, len);
  string[len] = '\0';

  if (!isdigit(*string)) {
    if (!strncmp(string, "all", 3)) {
      *firstp = 0;
      *amountp = -1;
      *stepp = 1;
      *wrapp = 0;
      return 0;
    } else if (!strncmp(string, "odd", 3)) {
      *firstp = 1;
      *amountp = -1;
      *stepp = 2;
      *wrapp = 0;
      return 0;
    } else if (!strncmp(string, "even", 4)) {
      *firstp = 0;
      *amountp = -1;
      *stepp = 2;
      *wrapp = 0;
      return 0;
    } else {
      fprintf(stderr, "unrecognized range keyword `%s'\n", string);
      return -1;
    }
  }

  first = strtol(string, &end, 10);
  amount = 1;
  wrap = 0;

  if (*end == '-') {
    last = strtol(end+1, &end2, 10);
    if (*end2) {
      fprintf(stderr, "invalid character at `%s' after range at `%s'\n", end2, string);
      return -1;
    } else if (end2 == end+1) {
      /* X- */
      amount = -1;
    } else {
      /* X-Y */
      amount = last-first+1;
    }

  } else if (*end == ':') {
    /* X:Y */
    wrap = 1;
    amount = strtol(end+1, &end2, 10);
    if (*end2) {
      fprintf(stderr, "invalid character at `%s' after range at `%s'\n", end2, string);
      return -1;
    } else if (end2 == end+1) {
      fprintf(stderr, "missing width at `%s' in range at `%s'\n", end2, string);
      return -1;
    }

  } else if (*end) {
    fprintf(stderr, "invalid character at `%s' after index at `%s'\n", end, string);
    return -1;
  }

  *firstp = first;
  *amountp = amount;
  *stepp = 1;
  *wrapp = wrap;
  return 0;
}

static __hwloc_inline int
hwloc_calc_append_object_range(hwloc_topology_t topology, unsigned topodepth,
			       hwloc_const_bitmap_t rootset, int depth,
			       const char *string, /* starts with indexes following the colon */
			       int logical,
			       void (*cbfunc)(void *, hwloc_obj_t, int), void *cbdata,
			       int verbose)
{
  hwloc_obj_t obj;
  unsigned width;
  const char *dot, *nextsep = NULL;
  int nextdepth = -1;
  int first, wrap, amount, step;
  unsigned i,j;
  int err;

  err = hwloc_calc_parse_range(string,
			       &first, &amount, &step, &wrap,
			       &dot);
  if (err < 0)
    return -1;
  assert(amount != -1 || !wrap);

  if (dot) {
    /* parse the next string before calling ourself recursively */
    size_t typelen;
    hwloc_obj_type_t type;
    const char *nextstring = dot+1;
    typelen = strspn(nextstring, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    if (!typelen || nextstring[typelen] != ':')
      return -1;
    nextsep = &nextstring[typelen];

    nextdepth = hwloc_calc_parse_depth_prefix(topology, topodepth,
					      nextstring, typelen,
					      &type,
					      verbose);
    if (nextdepth < 0)
      return -1;
  }

  width = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, rootset, depth);
  if (amount == -1)
    amount = (width-first+step-1)/step;

  for(i=first, j=0; j<(unsigned)amount; i+=step, j++) {
    if (wrap && i>=width)
      i = 0;

    obj = hwloc_calc_get_obj_inside_cpuset_by_depth(topology, rootset, depth, i, logical);
    if (verbose > 0 || (!obj && verbose >= 0)) {
      char *s;
      hwloc_bitmap_asprintf(&s, rootset);
      if (obj)
	printf("using object #%u depth %u below cpuset %s\n",
	       i, depth, s);
      else
	fprintf(stderr, "object #%u depth %u below cpuset %s does not exist\n",
		i, depth, s);
      free(s);
    }
    if (obj) {
      if (dot) {
	hwloc_calc_append_object_range(topology, topodepth, obj->cpuset, nextdepth, nextsep+1, logical, cbfunc, cbdata, verbose);
      } else {
	/* add to the temporary cpuset
	 * and let the caller add/clear/and/xor for the actual final cpuset depending on cmdline options
	 */
        cbfunc(cbdata, obj, verbose);
      }
    }
  }

  return 0;
}

static __hwloc_inline int
hwloc_calc_append_iodev(void (*cbfunc)(void *, hwloc_obj_t, int), void *cbdata,
			hwloc_obj_t obj,
			int verbose)
{
  cbfunc(cbdata, obj, verbose);
  return 0;
}

static __hwloc_inline hwloc_obj_t
hwloc_calc_find_next_pci_object(hwloc_topology_t topology, int vendor, int device,
				hwloc_obj_t prev, unsigned index_, int wrap)
{
  unsigned i = 0;
  hwloc_obj_t obj = prev;
  while (1) {
    obj = hwloc_get_next_pcidev(topology, obj);
    if (obj == prev)
      break; /* don't reuse the same object, even if wrap==1 */
    if (!obj) {
      if (!wrap)
	break;
      wrap = 0; /* wrap only once per call, to avoid infinite loops */
      obj = hwloc_get_next_pcidev(topology, NULL);
      if (!obj)
	break;
    }
    if ((vendor == -1 || (int) obj->attr->pcidev.vendor_id == vendor)
	&& (device == -1 || (int) obj->attr->pcidev.device_id == device))
      if (++i == index_)
        return obj;
  }
  return NULL;
}

static __hwloc_inline int
hwloc_calc_append_pci_object_range(hwloc_topology_t topology, const char *string,
				   void (*cbfunc)(void *, hwloc_obj_t, int), void *cbdata,
				   int verbose)
{
  hwloc_obj_t obj;
  int vendor, device;
  const char *current, *dot;
  char *endp;
  int first = 0, step = 1, amount = 1, wrap = 0; /* assume the index suffix is `:0' by default */
  int err, i, oldi, j;

  current = string;

  /* try to match by vendor:device */
  vendor = strtoul(current, &endp, 16);
  if (*endp != ':')
    goto failedvendordevice;
  if (endp == current)
    vendor = -1;
  current = endp+1;

  device = strtoul(current, &endp, 16);
  if (*endp != ':' && *endp != '\0')
    goto failedvendordevice;
  if (endp == current)
    device = -1;

  if (*endp != '\0') {
    current = endp+1;
    err = hwloc_calc_parse_range(current,
				 &first, &amount, &step, &wrap,
				 &dot);
    if (err < 0 || dot)
      goto failedvendordevice;
  }

  obj = NULL;
  for(oldi=-1, i=first, j=0; j<amount || amount == -1; oldi=i, i+=step, j++) {
    obj = hwloc_calc_find_next_pci_object(topology, vendor, device, obj, i-oldi, wrap);
    if (obj) {
      if (verbose > 0)
	printf("using matching PCI object #%d bus id %04x:%02x:%02x.%01x\n", i,
	       obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
      hwloc_calc_append_iodev(cbfunc, cbdata, obj, verbose);
    } else {
      if (amount != -1)
	if (verbose >= 0)
	  fprintf(stderr, "no matching PCI object #%d\n", i);
      break;
    }
  }
  return 0;

 failedvendordevice:
  /* TODO: more matching variants? vendor/device names? class?
   * but we don't want some ugly and unmaintainable code
   */

  if (verbose >= 0)
    fprintf(stderr, "invalid PCI device %s\n", string);
  return -1;
}

static __hwloc_inline int
hwloc_calc_process_type_arg(hwloc_topology_t topology, unsigned topodepth,
			    const char *arg, size_t typelen,
			    int logical,
			    void (*cbfunc)(void *, hwloc_obj_t, int), void *cbdata,
			    int verbose)
{
  const char *sep = &arg[typelen];
  hwloc_obj_type_t type = (hwloc_obj_type_t) -1;
  int depth;

  depth = hwloc_calc_parse_depth_prefix(topology, topodepth,
					arg, typelen,
					&type,
					verbose);
  if (depth < 0) {
    /* if we didn't find a depth but found a type, handle special cases */
    hwloc_obj_t obj = NULL;
    if (*sep == ':' && type == HWLOC_OBJ_PCI_DEVICE) {
      /* FIXME: change to another syntax? */
      return hwloc_calc_append_pci_object_range(topology, sep+1, cbfunc, cbdata, verbose);

    } else if (*sep == '=' && type == HWLOC_OBJ_PCI_DEVICE) {
      /* try to match a busid */
      obj = hwloc_get_pcidev_by_busidstring(topology, sep+1);
      if (obj)
	return hwloc_calc_append_iodev(cbfunc, cbdata, obj, verbose);
      if (verbose >= 0)
	fprintf(stderr, "invalid PCI device %s\n", sep+1);
      return -1;

    } else if (*sep == '=' && type == HWLOC_OBJ_OS_DEVICE) {
      /* try to match a OS device name */
      while ((obj = hwloc_get_next_osdev(topology, obj)) != NULL) {
	if (!strcmp(obj->name, sep+1))
	  return hwloc_calc_append_iodev(cbfunc, cbdata, obj, verbose);
      }
      if (verbose >= 0)
	fprintf(stderr, "invalid OS device %s\n", sep+1);
      return -1;
    } else
      return -1;
  }

  /* look at indexes following this type/depth */
  return hwloc_calc_append_object_range(topology, topodepth, hwloc_topology_get_complete_cpuset(topology), depth, sep+1, logical, cbfunc, cbdata, verbose);
}

static __hwloc_inline void
hwloc_calc_process_arg_cpuset_cb(void *_data, hwloc_obj_t obj, int verbose)
{
  hwloc_bitmap_t set = _data;
  /* walk up out of I/O objects */
  while (obj && !obj->cpuset)
    obj = obj->parent;
  if (!obj)
    /* do nothing */
    return;
  hwloc_calc_append_cpuset(set, obj->cpuset, HWLOC_CALC_APPEND_ADD, verbose);
}

static __hwloc_inline int
hwloc_calc_process_arg(hwloc_topology_t topology, unsigned topodepth,
		       const char *arg, int logical, hwloc_bitmap_t set,
		       int verbose)
{
  hwloc_calc_append_mode_t mode = HWLOC_CALC_APPEND_ADD;
  size_t typelen;
  int err;

  if (*arg == '~') {
    mode = HWLOC_CALC_APPEND_CLR;
    arg++;
  } else if (*arg == 'x') {
    mode = HWLOC_CALC_APPEND_AND;
    arg++;
  } else if (*arg == '^') {
    mode = HWLOC_CALC_APPEND_XOR;
    arg++;
  }

  if (!strcmp(arg, "all") || !strcmp(arg, "root"))
    return hwloc_calc_append_cpuset(set, hwloc_topology_get_topology_cpuset(topology), mode, verbose);

  /* try to match a type/depth followed by a special character */
  typelen = strspn(arg, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
  if (typelen && (arg[typelen] == ':' || arg[typelen] == '=')) {
    /* process type/depth */
    hwloc_bitmap_t newset = hwloc_bitmap_alloc();
    err = hwloc_calc_process_type_arg(topology, topodepth, arg, typelen, logical,
				      hwloc_calc_process_arg_cpuset_cb, newset,
				      verbose);
    if (!err)
      err = hwloc_calc_append_cpuset(set, newset, mode, verbose);
    hwloc_bitmap_free(newset);

  } else {
    /* try to match a cpuset */
    char *tmp = (char*) arg;
    hwloc_bitmap_t newset;
    int taskset = ( strchr(tmp, ',') == NULL );

    /* check the infinite prefix */
    if (hwloc_strncasecmp(tmp, "0xf...f,", 7+!taskset) == 0) {
      tmp += 7+!taskset;
      if (0 == *tmp) {
        err = -1;
        goto out;
      }
    }

    if (taskset) {
      /* check that the remaining is 0x followed by a huge hexadecimal number */
      if (hwloc_strncasecmp(tmp, "0x", 2) != 0) {
        err = -1;
        goto out;
      }
      tmp += 2;
      if (0 == *tmp) {
        err = -1;
        goto out;
      }
      if (strlen(tmp) != strspn(tmp, "0123456789abcdefABCDEF")) {
        err = -1;
        goto out;
      }

    } else {
      /* check that the remaining is a comma-separated list of hexadecimal integer with 0x as an optional prefix */
      while (1) {
	char *next = strchr(tmp, ',');
	size_t len;
        if (hwloc_strncasecmp(tmp, "0x", 2) == 0) {
	  tmp += 2;
	  if (',' == *tmp || 0 == *tmp) {
	    err = -1;
	    goto out;
	  }
	}
	len = next ? (size_t) (next-tmp) : strlen(tmp);
	if (len != strspn(tmp, "0123456789abcdefABCDEF")) {
	  err = -1;
	  goto out;
	}
	if (!next)
	  break;
	tmp = next+1;
      }
    }

    newset = hwloc_bitmap_alloc();
    if (taskset)
      hwloc_bitmap_taskset_sscanf(newset, arg);
    else
      hwloc_bitmap_sscanf(newset, arg);
    err = hwloc_calc_append_cpuset(set, newset, mode, verbose);
    hwloc_bitmap_free(newset);
  }

 out:
  return err;
}

static __hwloc_inline void
hwloc_calc_locations_usage(FILE *where)
{
  fprintf (where, "    core:2-3        for the second and third core\n");
  fprintf (where, "    node:1.pu:2       the third PU of the second NUMA node\n");
  fprintf (where, "    0x12345678        a CPU set given a bitmask string\n");
  fprintf (where, "    os=eth0           the operating system device named eth0\n");
  fprintf (where, "    pci=0000:01:02.0  the PCI device with the given bus ID\n");
  fprintf (where, "  with prefix ~ to remove, ^ for xor and x for intersection\n");
  fprintf (where, "  (see Location Specification in hwloc(7) for details).\n");
}

#endif /* HWLOC_CALC_H */
