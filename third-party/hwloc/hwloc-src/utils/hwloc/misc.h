/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2022 Inria.  All rights reserved.
 * Copyright © 2009-2012 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_UTILS_MISC_H
#define HWLOC_UTILS_MISC_H

#include "private/autogen/config.h"
#include "hwloc.h"
#include "private/misc.h" /* for hwloc_strncasecmp() */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <assert.h>

extern void usage(const char *name, FILE *where);

static __hwloc_inline void
hwloc_utils_check_api_version(const char *callname)
{
  unsigned version = hwloc_get_api_version();
  if ((version >> 16) != (HWLOC_API_VERSION >> 16)) {
    fprintf(stderr,
	    "%s compiled for hwloc API 0x%x but running on library API 0x%x.\n"
	    "You may need to point LD_LIBRARY_PATH to the right hwloc library.\n"
	    "Aborting since the new ABI is not backward compatible.\n",
	    callname, (unsigned) HWLOC_API_VERSION, version);
    exit(EXIT_FAILURE);
  }
}

static __hwloc_inline void
hwloc_utils_input_format_usage(FILE *where, int addspaces)
{
  fprintf (where, "  --input <XML file>\n");
  fprintf (where, "  -i <XML file>   %*sRead topology from XML file <path>\n",
	   addspaces, " ");
#ifdef HWLOC_LINUX_SYS
  fprintf (where, "  --input <directory>\n");
  fprintf (where, "  -i <directory>  %*sRead topology from chroot containing the /proc and /sys\n",
	   addspaces, " ");
  fprintf (where, "                  %*sof another system\n",
	   addspaces, " ");
#endif
#ifdef HWLOC_HAVE_X86_CPUID
  fprintf (where, "  --input <directory>\n");
  fprintf (where, "  -i <directory>  %*sRead topology from directory containing a CPUID dump\n",
	   addspaces, " ");
#endif
  fprintf (where, "  --input \"node:2 2\"\n");
  fprintf (where, "  -i \"node:2 2\"   %*sSimulate a fake hierarchy, here with 2 NUMA nodes of 2\n",
	   addspaces, " ");
  fprintf (where, "                  %*sprocessors\n",
	   addspaces, " ");
  fprintf (where, "  --input-format <format>\n");
  fprintf (where, "  --if <format>   %*sEnforce input format among "
	   "xml, "
#ifdef HWLOC_LINUX_SYS
	   "fsroot, "
#endif
#ifdef HWLOC_HAVE_X86_CPUID
	   "cpuid, "
#endif
	   "synthetic\n",
	   addspaces, " ");
}

enum hwloc_utils_input_format {
  HWLOC_UTILS_INPUT_DEFAULT,
  HWLOC_UTILS_INPUT_XML,
  HWLOC_UTILS_INPUT_FSROOT,
  HWLOC_UTILS_INPUT_SYNTHETIC,
  HWLOC_UTILS_INPUT_CPUID,
  HWLOC_UTILS_INPUT_SHMEM,
  HWLOC_UTILS_INPUT_ARCHIVE
};

static __hwloc_inline enum hwloc_utils_input_format
hwloc_utils_parse_input_format(const char *name, const char *callname)
{
  if (!hwloc_strncasecmp(name, "default", 3))
    return HWLOC_UTILS_INPUT_DEFAULT;
  else if (!hwloc_strncasecmp(name, "xml", 1))
    return HWLOC_UTILS_INPUT_XML;
  else if (!hwloc_strncasecmp(name, "fsroot", 1))
    return HWLOC_UTILS_INPUT_FSROOT;
  else if (!hwloc_strncasecmp(name, "shmem", 5))
    return HWLOC_UTILS_INPUT_SHMEM;
  else if (!hwloc_strncasecmp(name, "synthetic", 1))
    return HWLOC_UTILS_INPUT_SYNTHETIC;
  else if (!hwloc_strncasecmp(name, "cpuid", 1))
    return HWLOC_UTILS_INPUT_CPUID;
  else if (!hwloc_strncasecmp(name, "archive", 1))
    return HWLOC_UTILS_INPUT_ARCHIVE;

  fprintf(stderr, "input format `%s' not supported\n", name);
  usage(callname, stderr);
  exit(EXIT_FAILURE);
}

static __hwloc_inline int
hwloc_utils_lookup_input_option(char *argv[], int argc, int *consumed_opts,
				char **inputp, enum hwloc_utils_input_format *input_formatp,
				const char *callname)
{
  if (!strcmp (argv[0], "--input")
	       || !strcmp (argv[0], "-i")) {
    if (argc <= 1) {
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    if (strlen(argv[1]))
      *inputp = argv[1];
    else
      *inputp = NULL;
    *consumed_opts = 1;
    return 1;
  }
  else if (!strcmp (argv[0], "--input-format")
	   || !strcmp (argv[0], "--if")) {
    if (argc <= 1) {
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    *input_formatp = hwloc_utils_parse_input_format (argv[1], callname);
    *consumed_opts = 1;
    return 1;
  }

  /* backward compat with 1.0 */
  else if (!strcmp (argv[0], "--synthetic")) {
    if (argc <= 1) {
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    *inputp = argv[1];
    *input_formatp = HWLOC_UTILS_INPUT_SYNTHETIC;
    *consumed_opts = 1;
    return 1;
  } else if (!strcmp (argv[0], "--xml")) {
    if (argc <= 1) {
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    *inputp = argv[1];
    *input_formatp = HWLOC_UTILS_INPUT_XML;
    *consumed_opts = 1;
    return 1;
  } else if (!strcmp (argv[0], "--fsys-root")) {
    if (argc <= 1) {
      usage (callname, stderr);
      exit(EXIT_FAILURE);
    }
    *inputp = argv[1];
    *input_formatp = HWLOC_UTILS_INPUT_FSROOT;
    *consumed_opts = 1;
    return 1;
  }

  return 0;
}

static __hwloc_inline enum hwloc_utils_input_format
hwloc_utils_autodetect_input_format(const char *input, int verbose)
{
  struct stat inputst;
  int err;
  err = stat(input, &inputst);
  if (err < 0) {
    if (verbose > 0)
      printf("assuming `%s' is a synthetic topology description\n", input);
    return HWLOC_UTILS_INPUT_SYNTHETIC;
  }
  if (S_ISREG(inputst.st_mode)) {
    size_t len = strlen(input);
    if (len >= 6 && !strcmp(input+len-6, ".shmem")) {
      if (verbose > 0)
	printf("assuming `%s' is a shmem topology file\n", input);
      return HWLOC_UTILS_INPUT_SHMEM;
    }
    if ((len >= 7 && !strcmp(input+len-7, ".tar.gz"))
        || (len >= 8 && !strcmp(input+len-8, ".tar.bz2"))) {
      if (verbose > 0)
	printf("assuming `%s' is an archive topology file\n", input);
      return HWLOC_UTILS_INPUT_ARCHIVE;
    }
    if (verbose > 0)
      printf("assuming `%s' is a XML file\n", input);
    return HWLOC_UTILS_INPUT_XML;
  }
  if (S_ISDIR(inputst.st_mode)) {
    char *childpath;
    struct stat childst;
    childpath = malloc(strlen(input) + 10); /* enough for appending /sys, /proc or /pu0 */
    if (childpath) {
      snprintf(childpath, strlen(input) + 10, "%s/pu0", input);
      if (stat(childpath, &childst) == 0 && S_ISREG(childst.st_mode)) {
	if (verbose > 0)
	  printf("assuming `%s' is a cpuid dump\n", input);
	free(childpath);
	return HWLOC_UTILS_INPUT_CPUID;
      }
      snprintf(childpath, strlen(input) + 10, "%s/proc", input);
      if (stat(childpath, &childst) == 0 && S_ISDIR(childst.st_mode)) {
	if (verbose > 0)
	  printf("assuming `%s' is a file-system root\n", input);
	free(childpath);
	return HWLOC_UTILS_INPUT_FSROOT;
      }
    }
    free(childpath);
  }
  fprintf (stderr, "Unrecognized input file: %s\n", input);
  return HWLOC_UTILS_INPUT_DEFAULT;
}

static __hwloc_inline int
hwloc_utils_enable_input_format(struct hwloc_topology *topology, unsigned long flags,
				const char *input,
				enum hwloc_utils_input_format *input_format,
				int verbose, const char *callname)
{
  if (*input_format == HWLOC_UTILS_INPUT_DEFAULT && !strcmp(input, "-.xml")) {
    *input_format = HWLOC_UTILS_INPUT_XML;
    input = "-";
  }

  if (*input_format == HWLOC_UTILS_INPUT_DEFAULT) {
    *input_format = hwloc_utils_autodetect_input_format(input, verbose);
    if (*input_format == HWLOC_UTILS_INPUT_DEFAULT) {
      usage (callname, stderr);
      return EXIT_FAILURE;
    }
  }

  switch (*input_format) {
  case HWLOC_UTILS_INPUT_XML:
    if (!strcmp(input, "-"))
      input = "/dev/stdin";
    if (hwloc_topology_set_xml(topology, input)) {
      perror("Setting source XML file");
      return EXIT_FAILURE;
    }
    break;

  case HWLOC_UTILS_INPUT_FSROOT: {
#ifdef HWLOC_LINUX_SYS
    char *env;
    if (asprintf(&env, "HWLOC_FSROOT=%s", input) < 0)
      fprintf(stderr, "Failed to pass input filesystem root directory to HWLOC_FSROOT environment variable\n");
    else
      putenv(env);
    putenv((char *) "HWLOC_DUMPED_HWDATA_DIR=/var/run/hwloc");
    env = getenv("HWLOC_COMPONENTS");
    if (env)
      fprintf(stderr, "Cannot force linux component first because HWLOC_COMPONENTS environment variable is already set to %s.\n", env);
    else
      putenv((char *) "HWLOC_COMPONENTS=linux,pci,stop");
    /* normally-set flags are overriden by envvar-forced backends */
    if (flags & HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM)
      putenv((char *) "HWLOC_THISSYSTEM=1");
#else /* HWLOC_LINUX_SYS */
    fprintf(stderr, "This installation of hwloc does not support changing the file-system root, sorry.\n");
    exit(EXIT_FAILURE);
#endif /* HWLOC_LINUX_SYS */
    break;
  }

  case HWLOC_UTILS_INPUT_CPUID: {
#ifdef HWLOC_HAVE_X86_CPUID
    size_t len = strlen("HWLOC_CPUID_PATH=")+strlen(input)+1;
    char *env = malloc(len);
    if (!env) {
      fprintf(stderr, "Failed to pass input cpuid dump path to HWLOC_CPUID_PATH environment variable\n");
    } else {
      snprintf(env, len, "HWLOC_CPUID_PATH=%s", input);
      putenv(env);
    }
    env = getenv("HWLOC_COMPONENTS");
    if (env)
      fprintf(stderr, "Cannot force x86 component first because HWLOC_COMPONENTS environment variable is already set to %s.\n", env);
    else
      putenv((char *) "HWLOC_COMPONENTS=x86,stop");
    /* normally-set flags are overriden by envvar-forced backends */
    if (flags & HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM)
      putenv((char *) "HWLOC_THISSYSTEM=1");
#else
    fprintf(stderr, "This installation of hwloc does not support loading from a cpuid dump, sorry.\n");
    exit(EXIT_FAILURE);
#endif
    break;
  }

  case HWLOC_UTILS_INPUT_ARCHIVE: {
#ifdef HWLOC_ARCHIVEMOUNT_PATH
    char mntpath[] = "/tmp/tmpdir.hwloc.archivemount.XXXXXX";
    char mntcmd[512];
    char umntcmd[512];
    DIR *dir;
    struct dirent *dirent;
    enum hwloc_utils_input_format sub_input_format;
    char *subdir = NULL;
    int err;

    if (!mkdtemp(mntpath)) {
      perror("Creating archivemount directory");
      return EXIT_FAILURE;
    }
    snprintf(mntcmd, sizeof(mntcmd), "%s %s %s", HWLOC_ARCHIVEMOUNT_PATH, input, mntpath);
    err = system(mntcmd);
    if (err) {
      perror("Archivemount'ing the archive");
      rmdir(mntpath);
      return EXIT_FAILURE;
    }
    snprintf(umntcmd, sizeof(umntcmd), "umount -l %s", mntpath);

    /* enter the mount point and stay there so that we can umount+rmdir immediately but still use it later */
    chdir(mntpath);
    system(umntcmd);
    rmdir(mntpath);

    /* there should be a single subdirectory in the archive */
    dir = opendir(".");
    while ((dirent = readdir(dir)) != NULL) {
      if (strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, "..")) {
        subdir = dirent->d_name;
        break;
      }
    }
    closedir(dir);

    if (!subdir) {
      perror("No subdirectory in archivemount directory");
      return EXIT_FAILURE;
    }

    /* call ourself recursively on subdir, it should be either a fsroot or a cpuid directory */
    sub_input_format = HWLOC_UTILS_INPUT_DEFAULT;
    err = hwloc_utils_enable_input_format(topology, flags, subdir, &sub_input_format, verbose, callname);
    if (!err)
      *input_format = sub_input_format;
    break;
#else
    fprintf(stderr, "This installation of hwloc does not support loading from an archive, sorry.\n");
    exit(EXIT_FAILURE);
#endif
  }

  case HWLOC_UTILS_INPUT_SYNTHETIC:
    if (hwloc_topology_set_synthetic(topology, input)) {
      perror("Setting synthetic topology description");
      return EXIT_FAILURE;
    }
    break;

  case HWLOC_UTILS_INPUT_SHMEM:
    break;

  case HWLOC_UTILS_INPUT_DEFAULT:
    assert(0);
  }

  return 0;
}

static __hwloc_inline void
hwloc_utils_print_distance_matrix(FILE *output, unsigned nbobjs, hwloc_obj_t *objs, hwloc_uint64_t *matrix, int logical, int show_types)
{
  unsigned i, j;
#define MATRIX_ITEM_SIZE_MAX 17 /* 16 + ending \0 */
  char *headers;
  char *values;
  char *buf;
  size_t len, max;

  headers = malloc((nbobjs+1)*MATRIX_ITEM_SIZE_MAX);
  values = malloc(nbobjs*nbobjs*MATRIX_ITEM_SIZE_MAX);
  if (!headers || !values) {
    free(headers);
    free(values);
    return;
  }

  snprintf(headers, MATRIX_ITEM_SIZE_MAX, "           index" /* 16 */);
  max = 5;
  /* prepare column headers */
  for(i=0, buf = headers + MATRIX_ITEM_SIZE_MAX;
      i<nbobjs;
      i++, buf += MATRIX_ITEM_SIZE_MAX) {
    char tmp[MATRIX_ITEM_SIZE_MAX];
    hwloc_obj_t obj = objs[i];
    unsigned index = logical ? obj->logical_index : obj->os_index;
    if (obj->type == HWLOC_OBJ_OS_DEVICE)
      len = snprintf(tmp, MATRIX_ITEM_SIZE_MAX,
                     "%s", obj->name);
    else if (obj->type == HWLOC_OBJ_PCI_DEVICE)
      len = snprintf(tmp, MATRIX_ITEM_SIZE_MAX,
                     "%04x:%02x:%02x.%01x",
                     obj->attr->pcidev.domain, obj->attr->pcidev.bus, obj->attr->pcidev.dev, obj->attr->pcidev.func);
    else if (show_types)
      len = snprintf(tmp, MATRIX_ITEM_SIZE_MAX,
                     "%s:%d", hwloc_obj_type_string(obj->type), (int) index);
    else
      len = snprintf(tmp, MATRIX_ITEM_SIZE_MAX,
                     "%d", (int) index);
    if (len >= max)
      max = len;
    /* store it at the end of the slot in headers */
    memcpy(buf + (MATRIX_ITEM_SIZE_MAX - len - 1), tmp, len+1);
    /* and pad with spaces at the begining */
    memset(buf, ' ', MATRIX_ITEM_SIZE_MAX - len - 1);
  }
  /* prepare values */
  for(i=0, buf = values;
      i<nbobjs;
      i++) {
    for(j=0; j<nbobjs; j++, buf += MATRIX_ITEM_SIZE_MAX) {
     char tmp[MATRIX_ITEM_SIZE_MAX];
     len = snprintf(tmp, MATRIX_ITEM_SIZE_MAX, "%llu", (unsigned long long) matrix[i*nbobjs+j]);
      if (len >= max)
        max = len;
      /* store it at the end of the slot in values */
      memcpy(buf + (MATRIX_ITEM_SIZE_MAX - len - 1), tmp, len+1);
      /* and pad with spaces at the begining */
      memset(buf, ' ', MATRIX_ITEM_SIZE_MAX - len - 1);
    }
  }

  /* now display everything */
  for(i=0; i<nbobjs + 1; i++)
    fprintf(output, " %s", headers + i*MATRIX_ITEM_SIZE_MAX + MATRIX_ITEM_SIZE_MAX-max-1);
  fprintf(output, "\n");
  for(i=0; i<nbobjs; i++) {
    fprintf(output, " %s", headers + (i+1)*MATRIX_ITEM_SIZE_MAX + MATRIX_ITEM_SIZE_MAX-max-1);
    for(j=0; j<nbobjs; j++)
      fprintf(output, " %s", values + (i*nbobjs+j)*MATRIX_ITEM_SIZE_MAX + MATRIX_ITEM_SIZE_MAX-max-1);
    fprintf(output, "\n");
  }

  free(headers);
  free(values);
}

static __hwloc_inline int
hwloc_pid_from_number(hwloc_pid_t *pidp, int pid_number, int set_info __hwloc_attribute_unused, int verbose __hwloc_attribute_unused)
{
  hwloc_pid_t pid;
#ifdef HWLOC_WIN_SYS
  pid = OpenProcess(set_info ? PROCESS_SET_INFORMATION : PROCESS_QUERY_INFORMATION, FALSE, pid_number);
  if (!pid) {
    if (verbose) {
      DWORD error = GetLastError();
      char *message;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		    NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *)&message, 0, NULL);
      fprintf(stderr, "OpenProcess %d failed %lu: %s\n", pid_number, (unsigned long) error, message);
    }
    return -1;
  }
#else
  pid = pid_number;
#endif
  *pidp = pid;
  return 0;
}

static __hwloc_inline void
hwloc_lstopo_show_summary_depth(FILE *output, size_t prefixmaxlen, hwloc_topology_t topology, int depth)
{
  hwloc_obj_type_t type = hwloc_get_depth_type(topology, depth);
  unsigned nbobjs = hwloc_get_nbobjs_by_depth(topology, depth);
  if (nbobjs) {
    size_t prefixlen;
    char _types[64];
    const char *types;

    if (depth < 0)
      prefixlen = fprintf(output, "Special depth %d:", depth);
    else
      prefixlen = fprintf(output, "%*sdepth %d:", depth, "", depth);

    if (depth < 0) {
      /* use plain type, we don't want OSdev subtype since it may differ for other objects in the level */
      types = hwloc_obj_type_string(type);
    } else {
      /* use verbose type name, those are identical for all objects on normal levels */
      hwloc_obj_type_snprintf(_types, sizeof(_types), hwloc_get_obj_by_depth(topology, depth, 0), 1);
      types = _types;
    }

    fprintf(output, "%*s%u %s (type #%d)\n",
	    (int)(prefixmaxlen-prefixlen), "",
	    nbobjs, types, (int) type);
  }
}

static __hwloc_inline void
hwloc_lstopo_show_summary(FILE *output, hwloc_topology_t topology)
{
  int topodepth = hwloc_topology_get_depth(topology);
  int depth;
  size_t prefixmaxlen, sdepthmaxlen;

  prefixmaxlen = topodepth-1 + strlen("depth xyz:  ");
  sdepthmaxlen = strlen("Special depth -x:  ");
  if (prefixmaxlen < sdepthmaxlen)
    prefixmaxlen = sdepthmaxlen;

  for (depth = 0; depth < topodepth; depth++)
    hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, depth);
  /* FIXME: which order? */
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_NUMANODE);
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_MEMCACHE);
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_BRIDGE);
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_PCI_DEVICE);
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_OS_DEVICE);
  hwloc_lstopo_show_summary_depth(output, prefixmaxlen, topology, HWLOC_TYPE_DEPTH_MISC);
}


/*************************
 * Importing/exporting userdata buffers without understanding/decoding/modifying them
 * Caller must putenv("HWLOC_XML_USERDATA_NOT_DECODED=1") before loading the topology.
 */

struct hwloc_utils_userdata {
  char *name;
  size_t length;
  char *buffer; /* NULL if userdata entry in the list is not meant to be exported to XML (added by somebody else) */
  struct hwloc_utils_userdata *next;
};

static __hwloc_inline void
hwloc_utils_userdata_import_cb(hwloc_topology_t topology __hwloc_attribute_unused, hwloc_obj_t obj, const char *name, const void *buffer, size_t length)
{
  struct hwloc_utils_userdata *u, **up = (struct hwloc_utils_userdata **) &obj->userdata;
  while (*up)
    up = &((*up)->next);
  *up = u = malloc(sizeof(struct hwloc_utils_userdata));
  u->name = strdup(name);
  u->length = length;
  u->buffer = strdup(buffer);
  u->next = NULL;
}

static __hwloc_inline void
hwloc_utils_userdata_export_cb(void *reserved, hwloc_topology_t topology, hwloc_obj_t obj)
{
  struct hwloc_utils_userdata *u = obj->userdata;
  while (u) {
    if (u->buffer) /* not meant to be exported to XML (added by somebody else) */
      hwloc_export_obj_userdata(reserved, topology, obj, u->name, u->buffer, u->length);
    u = u->next;
  }
}

/* to be called when importing from shmem with non-NULL userdata pointing to stuff in the other process */
static __hwloc_inline void
hwloc_utils_userdata_clear_recursive(hwloc_obj_t obj)
{
  hwloc_obj_t child;
  obj->userdata= NULL;
  for_each_child(child, obj)
    hwloc_utils_userdata_clear_recursive(child);
  for_each_memory_child(child, obj)
    hwloc_utils_userdata_clear_recursive(child);
  for_each_io_child(child, obj)
    hwloc_utils_userdata_clear_recursive(child);
  for_each_misc_child(child, obj)
    hwloc_utils_userdata_clear_recursive(child);
}

/* must be called once the caller has removed its own userdata */
static __hwloc_inline void
hwloc_utils_userdata_free(hwloc_obj_t obj)
{
  struct hwloc_utils_userdata *u = obj->userdata, *next;
  while (u) {
    next = u->next;
    assert(u->buffer);
    free(u->name);
    free(u->buffer);
    free(u);
    u = next;
  }
  obj->userdata = NULL;
}

/* must be called once the caller has removed its own userdata */
static __hwloc_inline void
hwloc_utils_userdata_free_recursive(hwloc_obj_t obj)
{
  hwloc_obj_t child;
  hwloc_utils_userdata_free(obj);
  for_each_child(child, obj)
    hwloc_utils_userdata_free_recursive(child);
  for_each_memory_child(child, obj)
    hwloc_utils_userdata_free_recursive(child);
  for_each_io_child(child, obj)
    hwloc_utils_userdata_free_recursive(child);
  for_each_misc_child(child, obj)
    hwloc_utils_userdata_free_recursive(child);
}

struct hwloc_utils_parsing_flag
{
    unsigned long ulong_flag;
    const char *str_flag;
};

#define HWLOC_UTILS_PARSING_FLAG(flag){ flag, #flag }

static __hwloc_inline void
hwloc_utils_parsing_flag_error(const char *err_message, struct hwloc_utils_parsing_flag possible_flags[], int len_possible_flags) {
  int i;
  fprintf(stderr, "Supported %s flags are substrings of:\n", err_message);
  for(i = 0; i < len_possible_flags; i++) {
    fprintf(stderr, "  ");
    fprintf(stderr, "%s", possible_flags[i].str_flag);
    fprintf(stderr, "\n");
  }
}

static __hwloc_inline unsigned long
hwloc_utils_parse_flags(char * str, struct hwloc_utils_parsing_flag possible_flags[], int len_possible_flags, const char * kind) {
  char *ptr;
  char *end;
  int ul_flag;
  int i;
  size_t j;
  unsigned long ul_flags = 0;

  ul_flag = strtoul(str, &end, 0);
  if(end != str && *end == '\0')
    return ul_flag;

  for(j=0; str[j]; j++)
    str[j] = toupper(str[j]);

  if(strcmp(str, "NONE") == 0)
    return 0;

  ptr = str;
  while (ptr) {
    int count = 0;
    unsigned long prv_flags = ul_flags;
    char *pch;
    int nosuffix = 0;

    /* skip separators at the beginning */
    ptr += strspn(ptr, ",|+");

    /* find separator after next token */
    j = strcspn(ptr, " ,|+");
    if (!j)
      break;

    if (ptr[j]) {
      /* mark the end of the token */
      ptr[j] = '\0';
      /* mark beginning of next token */
      end = ptr + j + 1;
    } else {
      /* no next token */
      end = NULL;
    }

    /* '$' means matching the end of a flag */
    pch = strchr(ptr, '$');
    if(pch) {
      nosuffix = 1;
      *pch = '\0';
    }

    for(i = 0; i < len_possible_flags; i++) {
      if(nosuffix == 1) {
        /* match the end */
        if(strcmp(ptr, possible_flags[i].str_flag + strlen(possible_flags[i].str_flag) - strlen(ptr)))
          continue;
      } else {
        /* match anywhere */
        if(!strstr(possible_flags[i].str_flag, ptr))
          continue;
      }

      if(count){
        fprintf(stderr, "Duplicate match for %s flag `%s'.\n", kind, ptr);
        hwloc_utils_parsing_flag_error(kind, possible_flags, len_possible_flags);
        return (unsigned long) - 1;
      }

      ul_flags |= possible_flags[i].ulong_flag;
      count++;
    }

    if(prv_flags == ul_flags) {
      fprintf(stderr, "Failed to parse %s flag `%s'.\n", kind, ptr);
      hwloc_utils_parsing_flag_error(kind, possible_flags, len_possible_flags);
      return (unsigned long) - 1;
    }

    ptr = end;
  }

  return ul_flags;
}

static __hwloc_inline hwloc_memattr_id_t
hwloc_utils_parse_memattr_name(hwloc_topology_t topo, const char *str)
{
  const char *name;
  hwloc_memattr_id_t id;
  int err;
  /* try by name, case insensitive */
  for(id=0; ; id++) {
    err = hwloc_memattr_get_name(topo, id, &name);
    if (err < 0)
      break;
    if (!strcasecmp(name, str))
      return id;
  }
  /* try by id */
  if (*str < '0' || *str > '9')
    return (hwloc_memattr_id_t) -1;
  id = atoi(str);
  err = hwloc_memattr_get_name(topo, id, &name);
  if (err < 0)
    return (hwloc_memattr_id_t) -1;
  else
    return id;
}

static __hwloc_inline int
hwloc_utils_get_best_node_in_array_by_memattr(hwloc_topology_t topology, hwloc_memattr_id_t id,
                                              unsigned nbnodes, hwloc_obj_t *nodes,
                                              struct hwloc_location *initiator)
{
  unsigned nbtgs, i, j;
  hwloc_obj_t *tgs;
  int best;
  hwloc_uint64_t *values, bestvalue;
  unsigned long mflags;
  int err;

  err = hwloc_memattr_get_flags(topology, id, &mflags);
  if (err < 0)
    goto out;

  nbtgs = 0;
  err = hwloc_memattr_get_targets(topology, id, initiator, 0, &nbtgs, NULL, NULL);
  if (err < 0)
    goto out;

  tgs = malloc(nbtgs * sizeof(*tgs));
  values = malloc(nbtgs * sizeof(*values));
  if (!tgs || !values)
    goto out_with_arrays;

  err = hwloc_memattr_get_targets(topology, id, initiator, 0, &nbtgs, tgs, values);
  if (err < 0)
    goto out_with_arrays;

  best = -1;
  bestvalue = 0;
  for(i=0; i<nbnodes; i++) {
    for(j=0; j<nbtgs; j++)
      if (tgs[j] == nodes[i])
        break;
    if (j==nbtgs)
      /* no target info for this node */
      continue;
    if (best == -1) {
      best = i;
      bestvalue = values[j];
    } else if (mflags & HWLOC_MEMATTR_FLAG_HIGHER_FIRST) {
      if (values[j] > bestvalue) {
        best = i;
        bestvalue = values[j];
      }
    } else {
      assert(mflags & HWLOC_MEMATTR_FLAG_LOWER_FIRST);
      if (values[j] < bestvalue) {
        best = i;
        bestvalue = values[j];
      }
    }
  }

  free(tgs);
  free(values);
  return best;

 out_with_arrays:
  free(tgs);
  free(values);
 out:
  return -1;
}

static __hwloc_inline int
hwloc_utils_get_best_node_in_nodeset_by_memattr(hwloc_topology_t topology, hwloc_memattr_id_t id,
                                                hwloc_nodeset_t nodeset,
                                                struct hwloc_location *initiator)
{
  unsigned nbtgs, i, j;
  hwloc_obj_t *tgs;
  int best;
  hwloc_uint64_t *values, bestvalue;
  unsigned long mflags;
  int err;

  err = hwloc_memattr_get_flags(topology, id, &mflags);
  if (err < 0)
    goto out;

  nbtgs = 0;
  err = hwloc_memattr_get_targets(topology, id, initiator, 0, &nbtgs, NULL, NULL);
  if (err < 0)
    goto out;

  tgs = malloc(nbtgs * sizeof(*tgs));
  values = malloc(nbtgs * sizeof(*values));
  if (!tgs || !values)
    goto out_with_arrays;

  err = hwloc_memattr_get_targets(topology, id, initiator, 0, &nbtgs, tgs, values);
  if (err < 0)
    goto out_with_arrays;

  best = -1;
  bestvalue = 0;
  hwloc_bitmap_foreach_begin(i, nodeset) {
    for(j=0; j<nbtgs; j++)
      if (tgs[j]->os_index == i)
        break;
    if (j==nbtgs)
      /* no target info for this node */
      continue;
    if (best == -1) {
      best = i;
      bestvalue = values[j];
    } else if (mflags & HWLOC_MEMATTR_FLAG_HIGHER_FIRST) {
      if (values[j] > bestvalue) {
        best = i;
        bestvalue = values[j];
      }
    } else {
      assert(mflags & HWLOC_MEMATTR_FLAG_LOWER_FIRST);
      if (values[j] < bestvalue) {
        best = i;
        bestvalue = values[j];
      }
    }
  } hwloc_bitmap_foreach_end();

  if (best == -1)
    hwloc_bitmap_zero(nodeset);
  else
    hwloc_bitmap_only(nodeset, best);

  free(tgs);
  free(values);
  return 0;

 out_with_arrays:
  free(tgs);
  free(values);
 out:
  return -1;
}

static __hwloc_inline unsigned long
hwloc_utils_parse_restrict_flags(char * str){
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_RESTRICT_FLAG_REMOVE_CPULESS),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_RESTRICT_FLAG_BYNODESET),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_RESTRICT_FLAG_REMOVE_MEMLESS),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_RESTRICT_FLAG_ADAPT_MISC),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_RESTRICT_FLAG_ADAPT_IO)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "restrict");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_topology_flags(char * str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_THISSYSTEM_ALLOWED_RESOURCES),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_IMPORT_SUPPORT),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_RESTRICT_TO_CPUBINDING),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_RESTRICT_TO_MEMBINDING),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_DONT_CHANGE_BINDING),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_NO_DISTANCES),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_NO_MEMATTRS),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_FLAG_NO_CPUKINDS)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "topology");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_allow_flags(char * str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_ALLOW_FLAG_ALL),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_ALLOW_FLAG_LOCAL_RESTRICTIONS),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_ALLOW_FLAG_CUSTOM)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "allow");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_export_synthetic_flags(char * str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_NO_EXTENDED_TYPES),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_NO_ATTRS),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_V1),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_EXPORT_SYNTHETIC_FLAG_IGNORE_MEMORY)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "synthetic");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_export_xml_flags(char * str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_TOPOLOGY_EXPORT_XML_FLAG_V1)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "xml");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_distances_add_flags(char * str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_DISTANCES_ADD_FLAG_GROUP),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_DISTANCES_ADD_FLAG_GROUP_INACCURATE)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "distances_add");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_memattr_flags(char *str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_MEMATTR_FLAG_HIGHER_FIRST),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_MEMATTR_FLAG_LOWER_FIRST),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_MEMATTR_FLAG_NEED_INITIATOR)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "memattr");
}

static __hwloc_inline unsigned long
hwloc_utils_parse_local_numanode_flags(char *str) {
  struct hwloc_utils_parsing_flag possible_flags[] = {
    HWLOC_UTILS_PARSING_FLAG(HWLOC_LOCAL_NUMANODE_FLAG_LARGER_LOCALITY),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_LOCAL_NUMANODE_FLAG_SMALLER_LOCALITY),
    HWLOC_UTILS_PARSING_FLAG(HWLOC_LOCAL_NUMANODE_FLAG_ALL)
  };

  return hwloc_utils_parse_flags(str, possible_flags, (int) sizeof(possible_flags) / sizeof(possible_flags[0]), "local_numanode");
}

#endif /* HWLOC_UTILS_MISC_H */
