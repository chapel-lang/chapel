/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2017 Inria.  All rights reserved.
 * Copyright © 2009-2012, 2015 Université Bordeaux
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <hwloc.h>
#ifdef HWLOC_LINUX_SYS
#include <hwloc/linux.h>
#endif /* HWLOC_LINUX_SYS */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#ifdef HAVE_TIME_T
#include <time.h>
#endif

#ifdef LSTOPO_HAVE_GRAPHICS
#ifdef HWLOC_HAVE_CAIRO
#include <cairo.h>
#endif
#endif

#ifdef HAVE_SETLOCALE
#include <locale.h>
#endif

#include "lstopo.h"
#include "misc.h"

int lstopo_pid_number = -1;
hwloc_pid_t lstopo_pid;
hwloc_obj_type_t lstopo_show_only = (hwloc_obj_type_t) -1;
int lstopo_show_cpuset = 0;
int lstopo_show_taskset = 0;
int lstopo_ignore_pus = 0;
int lstopo_collapse = 1;
unsigned long lstopo_export_synthetic_flags = 0;

char **lstopo_append_legends = NULL;
unsigned lstopo_append_legends_nr = 0;

unsigned int fontsize = 10;
unsigned int gridsize = 10;
enum lstopo_orient_e force_orient[HWLOC_OBJ_TYPE_MAX];

static unsigned int top = 0;

FILE *open_output(const char *filename, int overwrite)
{
  const char *extn;
  struct stat st;

  if (!filename || !strcmp(filename, "-"))
    return stdout;

  extn = strrchr(filename, '.');
  if (filename[0] == '-' && extn == filename + 1)
    return stdout;

  if (!stat(filename, &st) && !overwrite) {
    errno = EEXIST;
    return NULL;
  }

  return fopen(filename, "w");
}

static hwloc_obj_t insert_task(hwloc_topology_t topology, hwloc_cpuset_t cpuset, const char * name)
{
  hwloc_obj_t obj;

  hwloc_bitmap_and(cpuset, cpuset, hwloc_topology_get_topology_cpuset(topology));
  if (hwloc_bitmap_iszero(cpuset))
    return NULL;

  /* try to insert at exact position */
  obj = hwloc_topology_insert_misc_object_by_cpuset(topology, cpuset, name);
  if (!obj) {
    /* try to insert in a larger parent */
    char *s;
    hwloc_bitmap_asprintf(&s, cpuset);
    obj = hwloc_get_obj_covering_cpuset(topology, cpuset);
    if (obj) {
      obj = hwloc_topology_insert_misc_object_by_parent(topology, obj, name);
      fprintf(stderr, "Inserted process `%s' below parent larger than cpuset %s\n", name, s);
    } else {
      fprintf(stderr, "Failed to insert process `%s' with cpuset %s\n", name, s);
    }
    free(s);
  } else {
    hwloc_obj_add_info(obj, "Type", "Process");
  }

  return obj;
}

static void add_process_objects(hwloc_topology_t topology)
{
#ifdef HAVE_DIRENT_H
  hwloc_obj_t root;
  hwloc_bitmap_t cpuset;
#ifdef HWLOC_LINUX_SYS
  hwloc_bitmap_t task_cpuset;
#endif /* HWLOC_LINUX_SYS */
  DIR *dir;
  struct dirent *dirent;
  const struct hwloc_topology_support *support;

  root = hwloc_get_root_obj(topology);

  support = hwloc_topology_get_support(topology);

  if (!support->cpubind->get_proc_cpubind)
    return;

  dir  = opendir("/proc");
  if (!dir)
    return;
  cpuset = hwloc_bitmap_alloc();
#ifdef HWLOC_LINUX_SYS
  task_cpuset = hwloc_bitmap_alloc();
#endif /* HWLOC_LINUX_SYS */

  while ((dirent = readdir(dir))) {
    long local_pid_number;
    hwloc_pid_t local_pid;
    char *end;
    char name[64];
    int proc_cpubind;

    local_pid_number = strtol(dirent->d_name, &end, 10);
    if (*end)
      /* Not a number */
      continue;

    snprintf(name, sizeof(name), "%ld", local_pid_number);

    local_pid = hwloc_pid_from_number(local_pid_number, 0);

    proc_cpubind = hwloc_get_proc_cpubind(topology, local_pid, cpuset, 0) != -1;

#ifdef HWLOC_LINUX_SYS
    {
      /* Get the process name */
      char *path;
      unsigned pathlen = 6 + strlen(dirent->d_name) + 1 + 7 + 1;
      char cmd[64], *c;
      int file;
      ssize_t n;

      path = malloc(pathlen);
      snprintf(path, pathlen, "/proc/%s/cmdline", dirent->d_name);
      file = open(path, O_RDONLY);
      free(path);

      if (file >= 0) {
        n = read(file, cmd, sizeof(cmd) - 1);
        close(file);

        if (n <= 0)
          /* Ignore kernel threads and errors */
          continue;

        cmd[n] = 0;
        if ((c = strchr(cmd, ' ')))
          *c = 0;
        snprintf(name, sizeof(name), "%ld %s", local_pid_number, cmd);
      }
    }

    {
      /* Get threads */
      char *path;
      unsigned pathlen = 6+strlen(dirent->d_name) + 1 + 4 + 1;
      DIR *task_dir;
      struct dirent *task_dirent;

      path = malloc(pathlen);
      snprintf(path, pathlen, "/proc/%s/task", dirent->d_name);
      task_dir = opendir(path);
      free(path);

      if (task_dir) {
        while ((task_dirent = readdir(task_dir))) {
          long local_tid;
          char *task_end;
          char task_name[64];

          local_tid = strtol(task_dirent->d_name, &task_end, 10);
          if (*task_end)
            /* Not a number, or the main task */
            continue;

          if (hwloc_linux_get_tid_cpubind(topology, local_tid, task_cpuset))
            continue;

          if (proc_cpubind && hwloc_bitmap_isequal(task_cpuset, cpuset))
            continue;

          snprintf(task_name, sizeof(task_name), "%s %li", name, local_tid);

          insert_task(topology, task_cpuset, task_name);
        }
        closedir(task_dir);
      }
    }
#endif /* HWLOC_LINUX_SYS */

    if (!proc_cpubind)
      continue;

    if (hwloc_bitmap_isincluded(root->cpuset, cpuset))
      continue;

    insert_task(topology, cpuset, name);
  }

  hwloc_bitmap_free(cpuset);
#ifdef HWLOC_LINUX_SYS
  hwloc_bitmap_free(task_cpuset);
#endif /* HWLOC_LINUX_SYS */
  closedir(dir);
#endif /* HAVE_DIRENT_H */
}

static void
lstopo_add_collapse_attributes(hwloc_topology_t topology)
{
  hwloc_obj_t obj, collapser = NULL;
  unsigned collapsed = 0;
  /* collapse identical PCI devs */
  for(obj = hwloc_get_next_pcidev(topology, NULL); obj; obj = hwloc_get_next_pcidev(topology, obj)) {
    if (collapser) {
      if (!obj->arity
	  && obj->parent == collapser->parent
	  && obj->attr->pcidev.vendor_id == collapser->attr->pcidev.vendor_id
	  && obj->attr->pcidev.device_id == collapser->attr->pcidev.device_id
	  && obj->attr->pcidev.subvendor_id == collapser->attr->pcidev.subvendor_id
	  && obj->attr->pcidev.subdevice_id == collapser->attr->pcidev.subdevice_id) {
	/* collapse another one */
	hwloc_obj_add_info(obj, "lstopoCollapse", "0");
	collapsed++;
	continue;
      } else if (collapsed > 1) {
	/* end this collapsing */
	char text[10];
	snprintf(text, sizeof(text), "%u", collapsed);
	hwloc_obj_add_info(collapser, "lstopoCollapse", text);
	collapser = NULL;
	collapsed = 0;
      }
    }
    if (!obj->arity) {
      /* start a new collapsing */
      collapser = obj;
      collapsed = 1;
    }
  }
  if (collapsed > 1) {
    /* end this collapsing */
    char text[10];
    snprintf(text, sizeof(text), "%u", collapsed);
    hwloc_obj_add_info(collapser, "lstopoCollapse", text);
  }
}

static void
lstopo_populate_userdata(hwloc_obj_t parent)
{
  hwloc_obj_t child;
  struct lstopo_obj_userdata *save = malloc(sizeof(*save));

  save->common.buffer = NULL; /* so that it is ignored on XML export */
  save->common.next = parent->userdata;
  save->fontsize = (unsigned) -1;
  save->gridsize = (unsigned) -1;
  parent->userdata = save;

  for(child = parent->first_child; child; child = child->next_sibling)
    lstopo_populate_userdata(child);
}

static void
lstopo_destroy_userdata(hwloc_obj_t parent)
{
  hwloc_obj_t child;
  struct lstopo_obj_userdata *save = parent->userdata;

  if (save) {
    parent->userdata = save->common.next;
    free(save);
  }

  for(child = parent->first_child; child; child = child->next_sibling)
    lstopo_destroy_userdata(child);
}

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ... [ filename.format ]\n\n", name);
  fprintf (where, "See lstopo(1) for more details.\n");

  fprintf (where, "\nDefault output is "
#ifdef LSTOPO_HAVE_GRAPHICS
#ifdef HWLOC_WIN_SYS
		  "graphical"
#elif CAIRO_HAS_XLIB_SURFACE && (defined HWLOC_HAVE_X11_KEYSYM)
		  "graphical (X11) if DISPLAY is set, console otherwise"
#else
		  "console"
#endif
#else
		  "console"
#endif
		  ".\n");

  fprintf (where, "Supported output file formats: console, ascii, fig"
#ifdef LSTOPO_HAVE_GRAPHICS
#if CAIRO_HAS_PDF_SURFACE
		  ", pdf"
#endif /* CAIRO_HAS_PDF_SURFACE */
#if CAIRO_HAS_PS_SURFACE
		  ", ps"
#endif /* CAIRO_HAS_PS_SURFACE */
#if CAIRO_HAS_PNG_FUNCTIONS
		  ", png"
#endif /* CAIRO_HAS_PNG_FUNCTIONS */
#if CAIRO_HAS_SVG_SURFACE
		  ", svg"
#endif /* CAIRO_HAS_SVG_SURFACE */
#endif /* LSTOPO_HAVE_GRAPHICS */
		  ", xml, synthetic"
		  "\n");
  fprintf (where, "\nFormatting options:\n");
  fprintf (where, "  -l --logical          Display hwloc logical object indexes\n");
  fprintf (where, "                        (default for console output)\n");
  fprintf (where, "  -p --physical         Display physical object indexes\n");
  fprintf (where, "                        (default for graphical output)\n");
  fprintf (where, "Output options:\n");
  fprintf (where, "  --output-format <format>\n");
  fprintf (where, "  --of <format>         Force the output to use the given format\n");
  fprintf (where, "  -f --force            Overwrite the output file if it exists\n");
  fprintf (where, "Textual output options:\n");
  fprintf (where, "  --only <type>         Only show objects of the given type in the textual output\n");
  fprintf (where, "  -v --verbose          Include additional details\n");
  fprintf (where, "  -s --silent           Reduce the amount of details to show\n");
  fprintf (where, "  -c --cpuset           Show the cpuset of each object\n");
  fprintf (where, "  -C --cpuset-only      Only show the cpuset of each object\n");
  fprintf (where, "  --taskset             Show taskset-specific cpuset strings\n");
  fprintf (where, "Object filtering options:\n");
  fprintf (where, "  --ignore <type>       Ignore objects of the given type\n");
  fprintf (where, "  --no-caches           Do not show caches\n");
  fprintf (where, "  --no-useless-caches   Do not show caches which do not have a hierarchical\n"
                  "                        impact\n");
  fprintf (where, "  --no-icaches          Do not show instruction caches\n");
  fprintf (where, "  --merge               Do not show levels that do not have a hierarchical\n"
                  "                        impact\n");
  fprintf (where, "  --no-collapse         Do not collapse identical PCI devices\n");
  fprintf (where, "  --restrict <cpuset>   Restrict the topology to processors listed in <cpuset>\n");
  fprintf (where, "  --restrict binding    Restrict the topology to the current process binding\n");
  fprintf (where, "  --restrict-flags <n>  Set the flags to be used during restrict\n");
  fprintf (where, "  --no-io               Do not show any I/O device or bridge\n");
  fprintf (where, "  --no-bridges          Do not any I/O bridge except hostbridges\n");
  fprintf (where, "  --whole-io            Show all I/O devices and bridges\n");
  fprintf (where, "Input options:\n");
  hwloc_utils_input_format_usage(where, 6);
  fprintf (where, "  --thissystem          Assume that the input topology provides the topology\n"
		  "                        for the system on which we are running\n");
  fprintf (where, "  --pid <pid>           Detect topology as seen by process <pid>\n");
  fprintf (where, "  --whole-system        Do not consider administration limitations\n");
  fprintf (where, "Graphical output options:\n");
  fprintf (where, "  --fontsize 10         Set size of text font\n");
  fprintf (where, "  --gridsize 10         Set size of margin between elements\n");
  fprintf (where, "  --horiz[=<type,...>]  Horizontal graphical layout instead of nearly 4/3 ratio\n");
  fprintf (where, "  --vert[=<type,...>]   Vertical graphical layout instead of nearly 4/3 ratio\n");
  fprintf (where, "  --rect[=<type,...>]   Rectangular graphical layout with nearly 4/3 ratio\n");
  fprintf (where, "  --no-legend           Remove the text legend at the bottom\n");
  fprintf (where, "  --append-legend <s>   Append a new line of text at the bottom of the legend\n");
  fprintf (where, "Miscellaneous options:\n");
  fprintf (where, "  --export-synthetic-flags <n>\n"
		  "                        Set flags during the synthetic topology export\n");
  fprintf (where, "  --ps --top            Display processes within the hierarchy\n");
  fprintf (where, "  --version             Report version and exit\n");
}

enum output_format {
  LSTOPO_OUTPUT_DEFAULT,
  LSTOPO_OUTPUT_CONSOLE,
  LSTOPO_OUTPUT_SYNTHETIC,
  LSTOPO_OUTPUT_ASCII,
  LSTOPO_OUTPUT_FIG,
  LSTOPO_OUTPUT_PNG,
  LSTOPO_OUTPUT_PDF,
  LSTOPO_OUTPUT_PS,
  LSTOPO_OUTPUT_SVG,
  LSTOPO_OUTPUT_XML
};

static enum output_format
parse_output_format(const char *name, char *callname)
{
  if (!hwloc_strncasecmp(name, "default", 3))
    return LSTOPO_OUTPUT_DEFAULT;
  else if (!hwloc_strncasecmp(name, "console", 3))
    return LSTOPO_OUTPUT_CONSOLE;
  else if (!strcasecmp(name, "synthetic"))
    return LSTOPO_OUTPUT_SYNTHETIC;
  else if (!strcasecmp(name, "ascii")
	   || !strcasecmp(name, "txt") /* backward compat with 1.10 */)
    return LSTOPO_OUTPUT_ASCII;
  else if (!strcasecmp(name, "fig"))
    return LSTOPO_OUTPUT_FIG;
  else if (!strcasecmp(name, "png"))
    return LSTOPO_OUTPUT_PNG;
  else if (!strcasecmp(name, "pdf"))
    return LSTOPO_OUTPUT_PDF;
  else if (!strcasecmp(name, "ps"))
    return LSTOPO_OUTPUT_PS;
  else if (!strcasecmp(name, "svg"))
    return LSTOPO_OUTPUT_SVG;
  else if (!strcasecmp(name, "xml"))
    return LSTOPO_OUTPUT_XML;

  fprintf(stderr, "file format `%s' not supported\n", name);
  usage(callname, stderr);
  exit(EXIT_FAILURE);
}

#define LSTOPO_VERBOSE_MODE_DEFAULT 1

int
main (int argc, char *argv[])
{
  int err;
  hwloc_topology_t topology;
  const char *filename = NULL;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES | HWLOC_TOPOLOGY_FLAG_ICACHES;
  unsigned long restrict_flags = 0;
  int merge = 0;
  int ignorecache = 0;
  char * callname;
  char * input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  enum output_format output_format = LSTOPO_OUTPUT_DEFAULT;
  char *restrictstring = NULL;
  struct lstopo_output loutput;
#ifdef HAVE_CLOCK_GETTIME
  struct timespec ts1, ts2;
  unsigned long ms;
  int measure_load_time = !!getenv("HWLOC_DEBUG_LOAD_TIME");
#endif
  int opt;
  unsigned i;

  loutput.overwrite = 0;
  loutput.logical = -1;
  loutput.legend = 1;
  loutput.verbose_mode = LSTOPO_VERBOSE_MODE_DEFAULT;

  for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
    force_orient[i] = LSTOPO_ORIENT_NONE;
  force_orient[HWLOC_OBJ_PU] = LSTOPO_ORIENT_HORIZ;
  force_orient[HWLOC_OBJ_CACHE] = LSTOPO_ORIENT_HORIZ;
  force_orient[HWLOC_OBJ_NUMANODE] = LSTOPO_ORIENT_HORIZ;

  /* enable verbose backends */
  putenv("HWLOC_XML_VERBOSE=1");
  putenv("HWLOC_SYNTHETIC_VERBOSE=1");

#ifdef HAVE_SETLOCALE
  setlocale(LC_ALL, "");
#endif

  callname = strrchr(argv[0], '/');
  if (!callname)
    callname = argv[0];
  else
    callname++;
  /* skip argv[0], handle options */
  argc--;
  argv++;

  err = hwloc_topology_init (&topology);
  if (err)
    return EXIT_FAILURE;

  while (argc >= 1)
    {
      opt = 0;
      if (!strcmp (argv[0], "-v") || !strcmp (argv[0], "--verbose")) {
	loutput.verbose_mode++;
      } else if (!strcmp (argv[0], "-s") || !strcmp (argv[0], "--silent")) {
	loutput.verbose_mode--;
      } else if (!strcmp (argv[0], "-h") || !strcmp (argv[0], "--help")) {
	usage(callname, stdout);
        exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[0], "-f") || !strcmp (argv[0], "--force"))
	loutput.overwrite = 1;
      else if (!strcmp (argv[0], "-l") || !strcmp (argv[0], "--logical"))
	loutput.logical = 1;
      else if (!strcmp (argv[0], "-p") || !strcmp (argv[0], "--physical"))
	loutput.logical = 0;
      else if (!strcmp (argv[0], "-c") || !strcmp (argv[0], "--cpuset"))
	lstopo_show_cpuset = 1;
      else if (!strcmp (argv[0], "-C") || !strcmp (argv[0], "--cpuset-only"))
	lstopo_show_cpuset = 2;
      else if (!strcmp (argv[0], "--taskset")) {
	lstopo_show_taskset = 1;
	if (!lstopo_show_cpuset)
	  lstopo_show_cpuset = 1;
      } else if (!strcmp (argv[0], "--only")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        if (hwloc_obj_type_sscanf(argv[1], &lstopo_show_only, NULL, NULL, 0) < 0)
	  fprintf(stderr, "Unsupported type `%s' passed to --only, ignoring.\n", argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--ignore")) {
	hwloc_obj_type_t type;
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	if (hwloc_obj_type_sscanf(argv[1], &type, NULL, NULL, 0) < 0)
	  fprintf(stderr, "Unsupported type `%s' passed to --ignore, ignoring.\n", argv[1]);
	else if (type == HWLOC_OBJ_PU)
	  lstopo_ignore_pus = 1;
	else
	  hwloc_topology_ignore_type(topology, type);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-caches"))
	ignorecache = 2;
      else if (!strcmp (argv[0], "--no-useless-caches"))
	ignorecache = 1;
      else if (!strcmp (argv[0], "--no-icaches"))
	flags &= ~HWLOC_TOPOLOGY_FLAG_ICACHES;
      else if (!strcmp (argv[0], "--whole-system"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
      else if (!strcmp (argv[0], "--no-io"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[0], "--no-bridges"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[0], "--whole-io"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_IO;
      else if (!strcmp (argv[0], "--merge"))
	merge = 1;
      else if (!strcmp (argv[0], "--no-collapse"))
	lstopo_collapse = 0;
      else if (!strcmp (argv[0], "--thissystem"))
	flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
      else if (!strcmp (argv[0], "--flags")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	flags = strtoul(argv[1], NULL, 0);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--restrict")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	restrictstring = strdup(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--restrict-flags")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	restrict_flags = (unsigned long) strtoull(argv[1], NULL, 0);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--export-synthetic-flags")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	lstopo_export_synthetic_flags = (unsigned long) strtoull(argv[1], NULL, 0);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--horiz"))
	for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
	  force_orient[i] = LSTOPO_ORIENT_HORIZ;
      else if (!strcmp (argv[0], "--vert"))
	for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
	  force_orient[i] = LSTOPO_ORIENT_VERT;
      else if (!strcmp (argv[0], "--rect"))
	for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
	  force_orient[i] = LSTOPO_ORIENT_RECT;
      else if (!strncmp (argv[0], "--horiz=", 8)
	       || !strncmp (argv[0], "--vert=", 7)
	       || !strncmp (argv[0], "--rect=", 7)) {
	enum lstopo_orient_e orient = (argv[0][2] == 'h') ? LSTOPO_ORIENT_HORIZ : (argv[0][2] == 'v') ? LSTOPO_ORIENT_VERT : LSTOPO_ORIENT_RECT;
	char *tmp = argv[0] + ((argv[0][2] == 'h') ? 8 : 7);
	while (tmp) {
	  char *end = strchr(tmp, ',');
	  hwloc_obj_type_t type;
	  if (end)
	    *end = '\0';
	  if (hwloc_obj_type_sscanf(tmp, &type, NULL, NULL, 0) < 0)
	    fprintf(stderr, "Unsupported type `%s' passed to %s, ignoring.\n", tmp, argv[0]);
	  else
	    force_orient[type] = orient;
	  if (!end)
	    break;
	  tmp = end+1;
        }
      }

      else if (!strcmp (argv[0], "--fontsize")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	fontsize = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--gridsize")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	gridsize = atoi(argv[1]);
	opt = 1;
      }
      else if (!strcmp (argv[0], "--no-legend")) {
	loutput.legend = 0;
      }
      else if (!strcmp (argv[0], "--append-legend")) {
	char **tmp;
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	tmp = realloc(lstopo_append_legends, (lstopo_append_legends_nr+1) * sizeof(*lstopo_append_legends));
	if (!tmp) {
	  fprintf(stderr, "Failed to realloc legend append array, legend ignored.\n");
	} else {
	  lstopo_append_legends = tmp;
	  lstopo_append_legends[lstopo_append_legends_nr] = strdup(argv[1]);
	  lstopo_append_legends_nr++;
	}
	opt = 1;
      }

      else if (hwloc_utils_lookup_input_option(argv, argc, &opt,
					       &input, &input_format,
					       callname)) {
	/* nothing to do anymore */

      } else if (!strcmp (argv[0], "--pid")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	lstopo_pid_number = atoi(argv[1]); opt = 1;
      } else if (!strcmp (argv[0], "--ps") || !strcmp (argv[0], "--top"))
        top = 1;
      else if (!strcmp (argv[0], "--version")) {
          printf("%s %s\n", callname, HWLOC_VERSION);
          exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[0], "--output-format") || !strcmp (argv[0], "--of")) {
	if (argc < 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        output_format = parse_output_format(argv[1], callname);
        opt = 1;
      } else {
	if (filename) {
	  fprintf (stderr, "Unrecognized option: %s\n", argv[0]);
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	} else
	  filename = argv[0];
      }
      argc -= opt+1;
      argv += opt+1;
    }

  if (lstopo_show_only != (hwloc_obj_type_t)-1)
    merge = 0;

  err = hwloc_topology_set_flags(topology, flags);
  if (err < 0) {
    fprintf(stderr, "Failed to set flags %lx (%s).\n", flags, strerror(errno));
    return EXIT_FAILURE;
  }

  if (ignorecache > 1) {
    hwloc_topology_ignore_type(topology, HWLOC_OBJ_CACHE);
  } else if (ignorecache) {
    hwloc_topology_ignore_type_keep_structure(topology, HWLOC_OBJ_CACHE);
  }
  if (merge)
    hwloc_topology_ignore_all_keep_structure(topology);

  if (input) {
    err = hwloc_utils_enable_input_format(topology, input, &input_format, loutput.verbose_mode > 1, callname);
    if (err)
      return err;
  }

  if (lstopo_pid_number > 0) {
    lstopo_pid = hwloc_pid_from_number(lstopo_pid_number, 0);
    if (hwloc_topology_set_pid(topology, lstopo_pid)) {
      perror("Setting target pid");
      return EXIT_FAILURE;
    }
  }

  /* if the output format wasn't enforced, look at the filename */
  if (filename && output_format == LSTOPO_OUTPUT_DEFAULT) {
    if (!strcmp(filename, "-")
	|| !strcmp(filename, "/dev/stdout")) {
      output_format = LSTOPO_OUTPUT_CONSOLE;
    } else {
      char *dot = strrchr(filename, '.');
      if (dot)
        output_format = parse_output_format(dot+1, callname);
      else {
	fprintf(stderr, "Cannot infer output type for file `%s' without any extension, using default output.\n", filename);
	filename = NULL;
      }
    }
  }

  /* if  the output format wasn't enforced, think a bit about what the user probably want */
  if (output_format == LSTOPO_OUTPUT_DEFAULT) {
    if (lstopo_show_cpuset
        || lstopo_show_only != (hwloc_obj_type_t)-1
        || loutput.verbose_mode != LSTOPO_VERBOSE_MODE_DEFAULT)
      output_format = LSTOPO_OUTPUT_CONSOLE;
  }

  if (input_format == HWLOC_UTILS_INPUT_XML
      && output_format == LSTOPO_OUTPUT_XML) {
    /* must be after parsing output format and before loading the topology */
    putenv("HWLOC_XML_USERDATA_NOT_DECODED=1");
    hwloc_topology_set_userdata_import_callback(topology, hwloc_utils_userdata_import_cb);
    hwloc_topology_set_userdata_export_callback(topology, hwloc_utils_userdata_export_cb);
  }

#ifdef HAVE_CLOCK_GETTIME
  if (measure_load_time)
    clock_gettime(CLOCK_MONOTONIC, &ts1);
#endif

  err = hwloc_topology_load (topology);
  if (err) {
    fprintf(stderr, "hwloc_topology_load() failed (%s).\n", strerror(errno));
    return EXIT_FAILURE;
  }

#ifdef HAVE_CLOCK_GETTIME
  if (measure_load_time) {
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    ms = (ts2.tv_nsec-ts1.tv_nsec)/1000000+(ts2.tv_sec-ts1.tv_sec)*1000UL;
    printf("hwloc_topology_load() took %lu ms\n", ms);
  }
#endif

  if (top)
    add_process_objects(topology);

  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    if (!strcmp (restrictstring, "binding")) {
      if (lstopo_pid_number > 0)
	hwloc_get_proc_cpubind(topology, lstopo_pid, restrictset, HWLOC_CPUBIND_PROCESS);
      else
	hwloc_get_cpubind(topology, restrictset, HWLOC_CPUBIND_PROCESS);
    } else {
      hwloc_bitmap_sscanf(restrictset, restrictstring);
    }
    err = hwloc_topology_restrict (topology, restrictset, restrict_flags);
    if (err) {
      perror("Restricting the topology");
      /* fallthrough */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }

  if (loutput.logical == -1) {
    if (output_format == LSTOPO_OUTPUT_CONSOLE)
      loutput.logical = 1;
    else if (output_format != LSTOPO_OUTPUT_DEFAULT)
      loutput.logical = 0;
  }

  loutput.topology = topology;
  loutput.file = NULL;

  lstopo_populate_userdata(hwloc_get_root_obj(topology));

  if (output_format != LSTOPO_OUTPUT_XML && lstopo_collapse)
    lstopo_add_collapse_attributes(topology);

  switch (output_format) {
    case LSTOPO_OUTPUT_DEFAULT:
#ifdef LSTOPO_HAVE_GRAPHICS
#if CAIRO_HAS_XLIB_SURFACE && defined HWLOC_HAVE_X11_KEYSYM
      if (getenv("DISPLAY")) {
        if (loutput.logical == -1)
          loutput.logical = 0;
        output_x11(&loutput, NULL);
      } else
#endif /* CAIRO_HAS_XLIB_SURFACE */
#ifdef HWLOC_WIN_SYS
      {
        if (loutput.logical == -1)
          loutput.logical = 0;
        output_windows(&loutput, NULL);
      }
#endif
#endif /* !LSTOPO_HAVE_GRAPHICS */
#if !defined HWLOC_WIN_SYS || !defined LSTOPO_HAVE_GRAPHICS
      {
        if (loutput.logical == -1)
          loutput.logical = 1;
        output_console(&loutput, NULL);
      }
#endif
      break;

    case LSTOPO_OUTPUT_CONSOLE:
      output_console(&loutput, filename);
      break;
    case LSTOPO_OUTPUT_SYNTHETIC:
      output_synthetic(&loutput, filename);
      break;
    case LSTOPO_OUTPUT_ASCII:
      output_ascii(&loutput, filename);
      break;
    case LSTOPO_OUTPUT_FIG:
      output_fig(&loutput, filename);
      break;
#ifdef LSTOPO_HAVE_GRAPHICS
# if CAIRO_HAS_PNG_FUNCTIONS
    case LSTOPO_OUTPUT_PNG:
      output_png(&loutput, filename);
      break;
# endif /* CAIRO_HAS_PNG_FUNCTIONS */
# if CAIRO_HAS_PDF_SURFACE
    case LSTOPO_OUTPUT_PDF:
      output_pdf(&loutput, filename);
      break;
# endif /* CAIRO_HAS_PDF_SURFACE */
# if CAIRO_HAS_PS_SURFACE
    case LSTOPO_OUTPUT_PS:
      output_ps(&loutput, filename);
      break;
#endif /* CAIRO_HAS_PS_SURFACE */
#if CAIRO_HAS_SVG_SURFACE
    case LSTOPO_OUTPUT_SVG:
      output_svg(&loutput, filename);
      break;
#endif /* CAIRO_HAS_SVG_SURFACE */
#endif /* LSTOPO_HAVE_GRAPHICS */
    case LSTOPO_OUTPUT_XML:
      output_xml(&loutput, filename);
      break;
    default:
      fprintf(stderr, "file format not supported\n");
      usage(callname, stderr);
      exit(EXIT_FAILURE);
  }

  lstopo_destroy_userdata(hwloc_get_root_obj(topology));
  hwloc_utils_userdata_free_recursive(hwloc_get_root_obj(topology));
  hwloc_topology_destroy (topology);

  for(i=0; i<lstopo_append_legends_nr; i++)
    free(lstopo_append_legends[i]);
  free(lstopo_append_legends);

  return EXIT_SUCCESS;
}
