/*
 * Copyright © 2009, 2024 CNRS
 * Copyright © 2009-2024 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_CALC_H
#define HWLOC_CALC_H

#include "hwloc.h"
#include "private/misc.h" /* for HWLOC_OBJ_TYPE_NONE and for hwloc_strncasecmp() */
#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <ctype.h>
#include <assert.h>

/* this is a global that doesn't change when walking hierarchy of locations, etc */
struct hwloc_calc_location_context_s {
  hwloc_topology_t topology;
  int topodepth;
  int only_hbm; /* -1 for everything, 0 for only non-HBM, 1 for only HBM numa nodes
		 * stored by caller, and passed to struct hwloc_calc_level for actual filtering
		 */
  int logical;
  int verbose;
};

/* this a local that changes when going from one level to another in the hierarchy of locations, etc */
struct hwloc_calc_level {
  int depth;
  hwloc_obj_type_t type;
  union hwloc_obj_attr_u attr;
  char subtype[32];
  int memory_tier;
  int pci_vendor, pci_device;
  int only_hbm; /* -1 for everything, 0 for only non-HBM, 1 for only HBM numa nodes */
};

typedef enum hwloc_calc_append_mode_e {
  HWLOC_CALC_APPEND_ADD,
  HWLOC_CALC_APPEND_CLR,
  HWLOC_CALC_APPEND_AND,
  HWLOC_CALC_APPEND_XOR
} hwloc_calc_append_mode_t;

static __hwloc_inline int
hwloc_calc_append_set(hwloc_bitmap_t set, hwloc_const_bitmap_t newset,
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

/* return 1 if obj is filtered out */
static __hwloc_inline int
hwloc_calc_check_object_filtered(hwloc_obj_t obj, struct hwloc_calc_level *level)
{
  if (level->subtype[0]) {
    if (!obj->subtype || strcasecmp(level->subtype, obj->subtype))
      return 1;
  }

  if (level->type == HWLOC_OBJ_NUMANODE) {
    if (level->memory_tier >= 0) {
      const char *tier = hwloc_obj_get_info_by_name(obj, "MemoryTier");
      if (!tier || atoi(tier) != level->memory_tier)
        return 1;
    }

    if (level->only_hbm >= 0) {
      /* filter on hbm */
      int obj_is_hbm = obj->subtype && !strcmp(obj->subtype, "MCDRAM");
      if (level->only_hbm != obj_is_hbm)
        return 1;
    }

  } else if (level->type == HWLOC_OBJ_PCI_DEVICE) {
    if (level->pci_vendor != -1 && (int) obj->attr->pcidev.vendor_id != level->pci_vendor)
      return 1;
    if (level->pci_device != -1 && (int) obj->attr->pcidev.device_id != level->pci_device)
      return 1;

  } else if (level->type == HWLOC_OBJ_OS_DEVICE) {
    if ((int) level->attr.osdev.type != -1 && obj->attr->osdev.type != level->attr.osdev.type)
      return 1;
  }

  return 0;
}

static __hwloc_inline unsigned
hwloc_calc_get_nbobjs_inside_sets_by_depth(struct hwloc_calc_location_context_s *lcontext,
					   hwloc_const_bitmap_t cpuset, hwloc_const_bitmap_t nodeset,
					   struct hwloc_calc_level *level)
{
  hwloc_topology_t topology = lcontext->topology;
  hwloc_obj_t obj = NULL;
  unsigned n = 0;
  while ((obj = hwloc_get_next_obj_by_depth(topology, level->depth, obj)) != NULL) {
    if (!hwloc_bitmap_iszero(obj->cpuset) && !hwloc_bitmap_intersects(obj->cpuset, cpuset))
      continue;
    if (!hwloc_bitmap_iszero(obj->nodeset) && !hwloc_bitmap_intersects(obj->nodeset, nodeset))
      continue;
    if (hwloc_bitmap_iszero(obj->cpuset) && hwloc_bitmap_iszero(obj->nodeset))
      /* ignore objects with empty sets (both can be empty when outside of cgroup) */
      continue;
    if (hwloc_calc_check_object_filtered(obj, level))
      continue;
    n++;
  }
  return n;
}

static __hwloc_inline hwloc_obj_t
hwloc_calc_get_obj_inside_sets_by_depth(struct hwloc_calc_location_context_s *lcontext,
					hwloc_const_bitmap_t cpuset, hwloc_const_bitmap_t nodeset,
					struct hwloc_calc_level *level, unsigned ind)
{
  hwloc_topology_t topology = lcontext->topology;
  int logical = lcontext->logical;
  hwloc_obj_t obj = NULL;
  unsigned i = 0;
  while ((obj = hwloc_get_next_obj_by_depth(topology, level->depth, obj)) != NULL) {
    if (!hwloc_bitmap_iszero(obj->cpuset) && !hwloc_bitmap_intersects(obj->cpuset, cpuset))
      continue;
    if (!hwloc_bitmap_iszero(obj->nodeset) && !hwloc_bitmap_intersects(obj->nodeset, nodeset))
      continue;
    if (hwloc_bitmap_iszero(obj->cpuset) && hwloc_bitmap_iszero(obj->nodeset))
      /* ignore objects with empty sets (both can be empty when outside of cgroup) */
      continue;
    if (hwloc_calc_check_object_filtered(obj, level))
      continue;
    if (logical) {
      if (i == ind)
	return obj;
      i++;
    } else {
      if (obj->os_index == ind)
	return obj;
    }
  }
  return NULL;
}

/* return the length of the type/depth prefix (including filters if any)
 * 0 if not found or invalid.
 */
static __hwloc_inline size_t
hwloc_calc_parse_level_size(const char *string)
{
  size_t len;
  const char *filter_end;

  /* type/depth prefix ends with either '.' (for child), "=" (for name of osdev),
   * ':' (for index), or '[' (for filters).
   */
  len = strcspn(string, ":=.[");
  if (string[len] != '[')
    return len;

  /* we want to include filters */
  filter_end = strchr(string+len, ']');
  if (!filter_end)
    return 0;
  return filter_end + 1 - string;
}

static __hwloc_inline int
hwloc_calc_parse_level_filter(hwloc_topology_t topology __hwloc_attribute_unused,
                              char *filter,
                              struct hwloc_calc_level *level)
{
  const char *current = filter;
  const char *end;
  unsigned subtypelen;

  if (!strncmp(current, "tier=", 5)) {
    level->memory_tier = atoi(current+5);
    return 0;
  }

  if (!strncmp(current, "subtype=", 8)) {
    current += 8;
    goto subtype;
  }

  if (level->type == HWLOC_OBJ_PCI_DEVICE) {
    /* try to match by [vendor:device] */
    unsigned vendor, device;
    if (sscanf(current, "%x:%x]", &vendor, &device) == 2) {
      level->pci_vendor = (int) vendor;
      level->pci_device = (int) device;
      return 0;
    } else if (sscanf(current, ":%x]", &device) == 1) {
      level->pci_device = (int) device;
      return 0;
    } else if (sscanf(current, "%x:]", &vendor) == 1) {
      level->pci_vendor = (int) vendor;
      return 0;
    } else if (!strncmp(current, ":]", 2)) {
      /* nothing */
      return 0;
    } else if (strchr(current, ':')) {
      fprintf(stderr, "invalid PCI vendor:device filter specification %s\n", filter);
      return -1;
    }

  } else if (level->type == HWLOC_OBJ_OS_DEVICE) {
    /* try to match by [osdevtype] */
    hwloc_obj_type_t type2;
    union hwloc_obj_attr_u attr;
    char *endp;
    int err;

    endp = strchr(current, ']');
    if (!endp) {
      fprintf(stderr, "invalid OS device type specification %s\n", filter);
      return -1;
    }
    *endp = 0;

    err = hwloc_type_sscanf(current, &type2, &attr, sizeof(attr));
    *endp = ']';
    if (err < 0 || type2 != HWLOC_OBJ_OS_DEVICE)
      goto subtype;
    level->attr.osdev.type = attr.osdev.type;
    return 0;
  }

 subtype:
  /* assume it's a subtype */
  end = strchr(current, ']');
  subtypelen = end - current;
  if (subtypelen >= sizeof(level->subtype))
    subtypelen = sizeof(level->subtype) - 1;
  snprintf(level->subtype, subtypelen+1, "%s", current);
  return 0;
}

static __hwloc_inline int
hwloc_calc_parse_level(struct hwloc_calc_location_context_s *lcontext,
                       hwloc_topology_t topology,
                       const char *_typestring, size_t typelen,
                       struct hwloc_calc_level *level)
{
  char typestring[20+1]; /* large enough to store all type names, even with a depth attribute */
  char *endptr;
  int err;

  level->subtype[0] = '\0';
  level->memory_tier = -1;
  level->pci_device = level->pci_vendor = -1;
  level->only_hbm = -1;
  if (lcontext)
    level->only_hbm = lcontext->only_hbm;

  level->depth = HWLOC_TYPE_DEPTH_UNKNOWN;

  if (typelen >= sizeof(typestring))
    return -1;
  snprintf(typestring, typelen+1, "%s", _typestring);

  err = hwloc_type_sscanf(typestring, &level->type, &level->attr, sizeof(level->attr));
  if (!err) {
    char *bracket;
    /* parsed a correct type */
    level->depth = hwloc_get_type_depth(topology, level->type);
    if (level->type == HWLOC_OBJ_GROUP
        && level->depth == HWLOC_TYPE_DEPTH_MULTIPLE
        && level->attr.group.depth != (unsigned)-1) {
      unsigned l;
      level->depth = HWLOC_TYPE_DEPTH_UNKNOWN;
      for(l=0; l<(unsigned) hwloc_topology_get_depth(topology); l++) {
        hwloc_obj_t tmp = hwloc_get_obj_by_depth(topology, l, 0);
        if (tmp->type == HWLOC_OBJ_GROUP && tmp->attr->group.depth == level->attr.group.depth) {
          level->depth = (int)l;
          break;
        }
      }
    }
    if (level->depth == HWLOC_TYPE_DEPTH_UNKNOWN
        || level->depth == HWLOC_TYPE_DEPTH_MULTIPLE)
      return -1;

    bracket = strchr(typestring, '[');
    if (bracket) {
      err = hwloc_calc_parse_level_filter(topology, bracket+1, level);
      if (err < 0)
        return -1;
    }
    return 0;
  }

  if (!strcasecmp(typestring, "HBM") || !strcasecmp(typestring, "MCDRAM")) {
    level->only_hbm = 1;
    level->type = HWLOC_OBJ_NUMANODE;
    level->depth = HWLOC_TYPE_DEPTH_NUMANODE;
    return 0;
  }

  /* couldn't parse the type, try a depth value */
  level->depth = strtoul(typestring, &endptr, 0);
  if (typestring[0] == '-' || *endptr || level->depth >= hwloc_topology_get_depth(topology)) {
    level->depth = HWLOC_TYPE_DEPTH_UNKNOWN;
    return -1;
  }
  level->type = HWLOC_OBJ_TYPE_NONE;
  return 0;
}

static __hwloc_inline int
hwloc_calc_parse_range(const char *_string,
		       int *firstp, int *amountp, int *stepp, int *wrapp,
		       const char **dotp,
		       int verbose)
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
    if (verbose >= 0)
      fprintf(stderr, "invalid range `%s', too long\n", _string);
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
      if (verbose >= 0)
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
      if (verbose >= 0)
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
      if (verbose >= 0)
	fprintf(stderr, "invalid character at `%s' after range at `%s'\n", end2, string);
      return -1;
    } else if (end2 == end+1) {
      if (verbose >= 0)
	fprintf(stderr, "missing width at `%s' in range at `%s'\n", end2, string);
      return -1;
    }

  } else if (*end) {
    if (verbose >= 0)
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
hwloc_calc_append_object_range(struct hwloc_calc_location_context_s *lcontext,
			       hwloc_const_bitmap_t rootcpuset, hwloc_const_bitmap_t rootnodeset, struct hwloc_calc_level *level,
			       const char *string, /* starts with indexes following the colon */
			       void (*cbfunc)(struct hwloc_calc_location_context_s *, void *, hwloc_obj_t), void *cbdata)
{
  int verbose = lcontext->verbose;
  hwloc_topology_t topology = lcontext->topology;
  hwloc_obj_t obj;
  unsigned width;
  const char *dot, *nextsep = NULL;
  struct hwloc_calc_level nextlevel;
  int first, wrap, amount, step;
  unsigned i,j;
  int found = 0;
  int err;

  err = hwloc_calc_parse_range(string,
			       &first, &amount, &step, &wrap,
			       &dot,
			       verbose);
  if (err < 0) {
    if (verbose >= 0)
      fprintf(stderr, "Failed to parse object index range %s\n", string);
    return -1;
  }
  assert(amount != -1 || !wrap);

  if (dot) {
    /* parse the next string before calling ourself recursively */
    size_t typelen;
    const char *nextstring = dot+1;
    typelen = hwloc_calc_parse_level_size(nextstring);
    if (!typelen || nextstring[typelen] != ':') {
      if (verbose >= 0)
	fprintf(stderr, "hierarchical sublocation %s contains types not followed by colon and index range\n", nextstring);
      return -1;
    }
    nextsep = &nextstring[typelen];

    err = hwloc_calc_parse_level(lcontext, topology, nextstring, typelen, &nextlevel);
    if (err < 0) {
      if (nextlevel.depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
        if (verbose >= 0)
          fprintf(stderr, "could not find level specified by location %s\n", nextstring);
        return -1;
      }
      if (nextlevel.depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
        if (verbose >= 0)
          fprintf(stderr, "found multiple levels for location %s\n", nextstring);
        return -1;
      }
    }

    /* we need an object with a cpuset, that's depth>=0 or memory */
    if (nextlevel.depth < 0 && nextlevel.depth != HWLOC_TYPE_DEPTH_NUMANODE) {
      if (verbose >= 0)
	fprintf(stderr, "hierarchical location %s only supported with normal object types\n", string);
      return -1;
    }
  }

  width = hwloc_calc_get_nbobjs_inside_sets_by_depth(lcontext, rootcpuset, rootnodeset, level);
  if (amount == -1)
    amount = (width-first+step-1)/step;

  for(i=first, j=0; j<(unsigned)amount; i+=step, j++) {
    if (wrap && i>=width)
      i = 0;

    obj = hwloc_calc_get_obj_inside_sets_by_depth(lcontext, rootcpuset, rootnodeset, level, i);
    if (verbose > 0 || (!obj && verbose >= 0)) {
      char *sc, *sn;
      hwloc_bitmap_asprintf(&sc, rootcpuset);
      hwloc_bitmap_asprintf(&sn, rootnodeset);
      if (obj)
	printf("using object #%u depth %d below cpuset %s nodeset %s\n",
	       i, level->depth, sc, sn);
      else
	fprintf(stderr, "object #%u depth %d below cpuset %s nodeset %s does not exist\n",
		i, level->depth, sc, sn);
      free(sc);
      free(sn);
    }
    if (obj) {
      found++;
      if (dot) {
	hwloc_calc_append_object_range(lcontext, obj->cpuset, obj->nodeset, &nextlevel, nextsep+1, cbfunc, cbdata);
      } else {
	/* add to the temporary cpuset
	 * and let the caller add/clear/and/xor for the actual final cpuset depending on cmdline options
	 */
        cbfunc(lcontext, cbdata, obj);
      }
    }
  }
  if (!found && verbose >= 0)
      fprintf(stderr, "failed to use any single object in index range %s\n", string);

  return 0;
}

static __hwloc_inline int
hwloc_calc_append_iodev(struct hwloc_calc_location_context_s *lcontext,
			void (*cbfunc)(struct hwloc_calc_location_context_s *, void *, hwloc_obj_t), void *cbdata,
			hwloc_obj_t obj)
{
  cbfunc(lcontext, cbdata, obj);
  return 0;
}

static __hwloc_inline int
hwloc_calc_append_iodev_by_index(struct hwloc_calc_location_context_s *lcontext,
				 struct hwloc_calc_level *level, const char *string,
				 void (*cbfunc)(struct hwloc_calc_location_context_s *, void *, hwloc_obj_t), void *cbdata)
{
  hwloc_topology_t topology = lcontext->topology;
  int verbose = lcontext->verbose;
  hwloc_obj_t obj, prev = NULL;
  const char *current, *dot;
  int first = 0, step = 1, amount = 1, wrap = 0; /* assume the index suffix is `:0' by default */
  int err, i, max;

  current = string;

  if (*current != '\0') {
    current++;
    err = hwloc_calc_parse_range(current,
				 &first, &amount, &step, &wrap,
				 &dot,
				 verbose);
    if (dot) {
      fprintf(stderr, "hierarchical location %s only supported with normal object types\n", string);
      return -1;
    }
    if (err < 0) {
      if (verbose >= 0)
	fprintf(stderr, "Failed to parse object index range %s\n", current);
      return -1;
    }
  }

  max = hwloc_get_nbobjs_by_depth(topology, level->depth);

  for(i=0; i < max*(wrap+1); i++) {
    if (i == max && wrap) {
      i = 0;
      wrap = 0;
    }

    obj = hwloc_get_obj_by_depth(topology, level->depth, i);
    assert(obj);

    if (obj == prev) /* already used that object, stop wrapping around */
      break;

    if (hwloc_calc_check_object_filtered(obj, level))
      continue;

    if (first--)
      continue;

    /* ok, got one object */
    hwloc_calc_append_iodev(lcontext, cbfunc, cbdata, obj);

    if (!prev)
      prev = obj;

    amount--;
    if (!amount)
      break;

    first = step-1;
  }

  return 0;
}

static __hwloc_inline int
hwloc_calc_process_location(struct hwloc_calc_location_context_s *lcontext,
			    const char *arg, size_t typelen,
			    void (*cbfunc)(struct hwloc_calc_location_context_s *, void *, hwloc_obj_t), void *cbdata)
{
  hwloc_topology_t topology = lcontext->topology;
  int verbose = lcontext->verbose;
  const char *sep = &arg[typelen];
  struct hwloc_calc_level level;
  int err;

  err = hwloc_calc_parse_level(lcontext, topology, arg, typelen, &level);
  if (err < 0) {
    if (level.depth == HWLOC_TYPE_DEPTH_UNKNOWN) {
      if (verbose >= 0)
        fprintf(stderr, "could not find level specified by location %s\n", arg);
      return -1;
    }
    if (level.depth == HWLOC_TYPE_DEPTH_MULTIPLE) {
      if (verbose >= 0)
        fprintf(stderr, "found multiple levels for location %s\n", arg);
      return -1;
    }
  }

  if (level.depth < 0 && level.depth != HWLOC_TYPE_DEPTH_NUMANODE) {
    /* special object without cpusets */

    /* if we didn't find a depth but found a type, handle special cases */
    hwloc_obj_t obj = NULL;

    if (*sep == ':') {
      return hwloc_calc_append_iodev_by_index(lcontext, &level, sep, cbfunc, cbdata);

    } else if (*sep == '=' && level.type == HWLOC_OBJ_PCI_DEVICE) {
      /* try to match a busid */
      obj = hwloc_get_pcidev_by_busidstring(topology, sep+1);
      if (obj)
	return hwloc_calc_append_iodev(lcontext, cbfunc, cbdata, obj);
      if (verbose >= 0)
	fprintf(stderr, "invalid PCI device %s\n", sep+1);
      return -1;

    } else if (*sep == '=' && level.type == HWLOC_OBJ_OS_DEVICE) {
      /* try to match a OS device name */
      while ((obj = hwloc_get_next_osdev(topology, obj)) != NULL) {
	if (!strcmp(obj->name, sep+1))
	  return hwloc_calc_append_iodev(lcontext, cbfunc, cbdata, obj);
      }
      if (verbose >= 0)
	fprintf(stderr, "invalid OS device %s\n", sep+1);
      return -1;

    } else if (*sep == '=' && level.type == HWLOC_OBJ_MISC) {
      /* try to match a Misc device name */
      obj = hwloc_get_obj_by_type(topology, HWLOC_OBJ_MISC, 0);
      while (obj) {
	if (!strcmp(obj->name, sep+1))
	  return hwloc_calc_append_iodev(lcontext, cbfunc, cbdata, obj);
	obj = obj->next_cousin;
      }
      if (verbose >= 0)
	fprintf(stderr, "invalid Misc object %s\n", sep+1);
      return -1;

    } else
      return -1;
  }

  /* look at indexes following this type/depth */
  return hwloc_calc_append_object_range(lcontext,
					hwloc_topology_get_complete_cpuset(topology),
					hwloc_topology_get_complete_nodeset(topology),
					&level, sep+1, cbfunc, cbdata);
}

struct hwloc_calc_set_context_s {
  int nodeset_input;
  int nodeset_output;
  enum hwloc_utils_cpuset_format_e cpuset_input_format;
  hwloc_bitmap_t output_set;
};

struct hwloc_calc_process_location_set_cbdata_s {
  struct hwloc_calc_set_context_s *scontext;
  hwloc_bitmap_t set;
};

static __hwloc_inline void
hwloc_calc_process_location_set_cb(struct hwloc_calc_location_context_s *lcontext, void *_data, hwloc_obj_t obj)
{
  int verbose = lcontext->verbose;
  struct hwloc_calc_process_location_set_cbdata_s *cbdata = _data;
  hwloc_bitmap_t set = cbdata->set;
  int nodeset_output = cbdata->scontext->nodeset_output;
  /* walk up out of I/O objects */
  while (obj && !obj->cpuset)
    obj = obj->parent;
  if (!obj)
    /* do nothing */
    return;
  hwloc_calc_append_set(set,
			nodeset_output ? obj->nodeset : obj->cpuset,
			HWLOC_CALC_APPEND_ADD, verbose);
}

static __hwloc_inline int
hwloc_calc_process_location_as_set(struct hwloc_calc_location_context_s *lcontext,
				   struct hwloc_calc_set_context_s *scontext,
				   const char *arg)
{
  hwloc_topology_t topology = lcontext->topology;
  int verbose = lcontext->verbose;
  int nodeset_output = scontext->nodeset_output;
  int nodeset_input = scontext->nodeset_input;
  hwloc_bitmap_t output_set = scontext->output_set;
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
    return hwloc_calc_append_set(output_set,
				 nodeset_output ? hwloc_topology_get_topology_nodeset(topology) : hwloc_topology_get_topology_cpuset(topology),
				 mode, verbose);

  /* try to match a type/depth followed by a special character */
  typelen = hwloc_calc_parse_level_size(arg);
  if (typelen && (arg[typelen] == ':' || arg[typelen] == '=')) {
    /* process type/depth */
    struct hwloc_calc_process_location_set_cbdata_s cbdata;
    cbdata.set = hwloc_bitmap_alloc();
    cbdata.scontext = scontext;
    err = hwloc_calc_process_location(lcontext, arg, typelen,
				      hwloc_calc_process_location_set_cb, &cbdata);
    if (!err)
      err = hwloc_calc_append_set(output_set, cbdata.set, mode, verbose);
    hwloc_bitmap_free(cbdata.set);

  } else {
    /* try to match a cpuset */
    hwloc_bitmap_t newset;
    enum hwloc_utils_cpuset_format_e cpuset_format = scontext->cpuset_input_format;

    if (cpuset_format == HWLOC_UTILS_CPUSET_FORMAT_UNKNOWN) {
      /* ambiguity list and hwloc if list of singleton like 1,3,5 which can be parsed as 0x1,0x3,0x5 or 1-1,3-3,5-5 */
      if (hwloc_strncasecmp(arg, "0x", 2) && strchr(arg, '-'))
        cpuset_format = HWLOC_UTILS_CPUSET_FORMAT_LIST;
      else if (strchr(arg, ','))
        cpuset_format = HWLOC_UTILS_CPUSET_FORMAT_HWLOC;
      else
        cpuset_format = HWLOC_UTILS_CPUSET_FORMAT_TASKSET;
    }

    newset = hwloc_bitmap_alloc();

    switch (cpuset_format) {
    case HWLOC_UTILS_CPUSET_FORMAT_HWLOC:
      err = hwloc_bitmap_sscanf(newset, arg);
      break;
    case HWLOC_UTILS_CPUSET_FORMAT_LIST:
      err = hwloc_bitmap_list_sscanf(newset, arg);
      break;
    case HWLOC_UTILS_CPUSET_FORMAT_TASKSET:
      err = hwloc_bitmap_taskset_sscanf(newset, arg);
      break;
    default:
      /* HWLOC_UTILS_CPUSET_FORMAT_SYSTEMD input not supported */
      abort();
    }
    if (err < 0) {
      hwloc_bitmap_free(newset);
      goto out;
    }

    if (nodeset_output && !nodeset_input) {
      hwloc_bitmap_t newnset = hwloc_bitmap_alloc();
      hwloc_cpuset_to_nodeset(topology, newset, newnset);
      err = hwloc_calc_append_set(output_set, newnset, mode, verbose);
      hwloc_bitmap_free(newnset);
    } else if (nodeset_input && !nodeset_output) {
      hwloc_bitmap_t newcset = hwloc_bitmap_alloc();
      hwloc_cpuset_from_nodeset(topology, newcset, newset);
      err = hwloc_calc_append_set(output_set, newcset, mode, verbose);
      hwloc_bitmap_free(newcset);
    } else {
      err = hwloc_calc_append_set(output_set, newset, mode, verbose);
    }
    hwloc_bitmap_free(newset);
  }

 out:
  return err;
}

static __hwloc_inline void
hwloc_calc_locations_usage(FILE *where)
{
  fprintf (where, "    core:2-3        for the third and fourth cores\n");
  fprintf (where, "    node:1.pu:2       the third PU of the second NUMA node\n");
  fprintf (where, "    0x12345678        a CPU set given a bitmask string\n");
  fprintf (where, "    os=eth0           the operating system device named eth0\n");
  fprintf (where, "    pci=0000:01:02.0  the PCI device with the given bus ID\n");
  fprintf (where, "  with prefix ~ to remove, ^ for xor and x for intersection\n");
  fprintf (where, "  (see Location Specification in hwloc(7) for details).\n");
}

#endif /* HWLOC_CALC_H */
