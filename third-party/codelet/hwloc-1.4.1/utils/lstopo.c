/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2011 inria.  All rights reserved.
 * Copyright © 2009-2011 Université Bordeaux 1
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
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>

#ifdef HWLOC_HAVE_CAIRO
#include <cairo.h>
#endif

#ifdef HAVE_SETLOCALE
#include <locale.h>
#endif

#include "lstopo.h"
#include "misc.h"

int logical = -1;
hwloc_obj_type_t show_only = (hwloc_obj_type_t) -1;
int show_cpuset = 0;
int taskset = 0;
unsigned int fontsize = 10;
unsigned int gridsize = 10;
enum lstopo_orient_e force_orient[HWLOC_OBJ_TYPE_MAX];
unsigned int legend = 1;
unsigned int top = 0;
hwloc_pid_t pid = (hwloc_pid_t) -1;

FILE *open_file(const char *filename, const char *mode)
{
  const char *extn;

  if (!filename)
    return stdout;

  extn = strrchr(filename, '.');
  if (filename[0] == '-' && extn == filename + 1)
    return stdout;

  return fopen(filename, mode);
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
  }

  return obj;
}

static void add_process_objects(hwloc_topology_t topology)
{
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

  if (!support->cpubind->get_thisproc_cpubind)
    return;

  dir  = opendir("/proc");
  if (!dir)
    return;
  cpuset = hwloc_bitmap_alloc();
#ifdef HWLOC_LINUX_SYS
  task_cpuset = hwloc_bitmap_alloc();
#endif /* HWLOC_LINUX_SYS */

  while ((dirent = readdir(dir))) {
    long local_pid;
    char *end;
    char name[64];
    int proc_cpubind;

    local_pid = strtol(dirent->d_name, &end, 10);
    if (*end)
      /* Not a number */
      continue;

    snprintf(name, sizeof(name), "%ld", local_pid);

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
        snprintf(name, sizeof(name), "%ld %s", local_pid, cmd);
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
}

void usage(const char *name, FILE *where)
{
  fprintf (where, "Usage: %s [ options ] ... [ filename.format ]\n\n", name);
  fprintf (where, "See lstopo(1) for more details.\n\n");
  fprintf (where, "Supported output file formats: console, txt, fig"
#ifdef HWLOC_HAVE_CAIRO
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
#endif /* HWLOC_HAVE_CAIRO */
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
  fprintf (where, "  --merge               Do not show levels that do not have a hierarchical\n"
                  "                        impact\n");
  fprintf (where, "  --restrict <cpuset>   Restrict the topology to processors listed in <cpuset>\n");
  fprintf (where, "  --restrict binding    Restrict the topology to the current process binding\n");
#ifdef HWLOC_HAVE_LIBPCI
  fprintf (where, "  --no-io               Do not show any I/O device or bridge\n");
  fprintf (where, "  --no-bridges          Do not any I/O bridge except hostbridges\n");
  fprintf (where, "  --whole-io            Show all I/O devices and bridges\n");
#endif
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
  fprintf (where, "  --no-legend           Remove the text legend at the bottom\n");
  fprintf (where, "Miscellaneous options:\n");
  fprintf (where, "  --ps --top            Display processes within the hierarchy\n");
  fprintf (where, "  --version             Report version and exit\n");
}

enum output_format {
  LSTOPO_OUTPUT_DEFAULT,
  LSTOPO_OUTPUT_CONSOLE,
  LSTOPO_OUTPUT_SYNTHETIC,
  LSTOPO_OUTPUT_TEXT,
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
  if (!strncasecmp(name, "default", 3))
    return LSTOPO_OUTPUT_DEFAULT;
  else if (!strncasecmp(name, "console", 3))
    return LSTOPO_OUTPUT_CONSOLE;
  else if (!strcasecmp(name, "synthetic"))
    return LSTOPO_OUTPUT_SYNTHETIC;
  else if (!strcasecmp(name, "txt"))
    return LSTOPO_OUTPUT_TEXT;
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
  int verbose_mode = LSTOPO_VERBOSE_MODE_DEFAULT;
  hwloc_topology_t topology;
  const char *filename = NULL;
  unsigned long flags = HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES;
  int merge = 0;
  int ignorecache = 0;
  char * callname;
  char * input = NULL;
  enum hwloc_utils_input_format input_format = HWLOC_UTILS_INPUT_DEFAULT;
  enum output_format output_format = LSTOPO_OUTPUT_DEFAULT;
  char *restrictstring = NULL;
  int opt;
  unsigned i;

  for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
    force_orient[i] = LSTOPO_ORIENT_NONE;
  force_orient[HWLOC_OBJ_PU] = LSTOPO_ORIENT_HORIZ;
  force_orient[HWLOC_OBJ_CACHE] = LSTOPO_ORIENT_HORIZ;
  force_orient[HWLOC_OBJ_NODE] = LSTOPO_ORIENT_HORIZ;

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

  err = hwloc_topology_init (&topology);
  if (err)
    return EXIT_FAILURE;

  while (argc >= 2)
    {
      opt = 0;
      if (!strcmp (argv[1], "-v") || !strcmp (argv[1], "--verbose")) {
	verbose_mode++;
      } else if (!strcmp (argv[1], "-s") || !strcmp (argv[1], "--silent")) {
	verbose_mode--;
      } else if (!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help")) {
	usage(callname, stdout);
        exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[1], "-l") || !strcmp (argv[1], "--logical"))
	logical = 1;
      else if (!strcmp (argv[1], "-p") || !strcmp (argv[1], "--physical"))
	logical = 0;
      else if (!strcmp (argv[1], "-c") || !strcmp (argv[1], "--cpuset"))
	show_cpuset = 1;
      else if (!strcmp (argv[1], "-C") || !strcmp (argv[1], "--cpuset-only"))
	show_cpuset = 2;
      else if (!strcmp (argv[1], "--taskset")) {
	taskset = 1;
	if (!show_cpuset)
	  show_cpuset = 1;
      } else if (!strcmp (argv[1], "--only")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        show_only = hwloc_obj_type_of_string(argv[2]);
	opt = 1;
      }
      else if (!strcmp (argv[1], "--ignore")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        hwloc_topology_ignore_type(topology, hwloc_obj_type_of_string(argv[2]));
	opt = 1;
      }
      else if (!strcmp (argv[1], "--no-caches"))
	ignorecache = 2;
      else if (!strcmp (argv[1], "--no-useless-caches"))
	ignorecache = 1;
      else if (!strcmp (argv[1], "--whole-system"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_SYSTEM;
      else if (!strcmp (argv[1], "--no-io"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_DEVICES | HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[1], "--no-bridges"))
	flags &= ~(HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
      else if (!strcmp (argv[1], "--whole-io"))
	flags |= HWLOC_TOPOLOGY_FLAG_WHOLE_IO;
      else if (!strcmp (argv[1], "--merge"))
	merge = 1;
      else if (!strcmp (argv[1], "--thissystem"))
	flags |= HWLOC_TOPOLOGY_FLAG_IS_THISSYSTEM;
      else if (!strcmp (argv[1], "--restrict")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	restrictstring = strdup(argv[2]);
	opt = 1;
      }

      else if (!strcmp (argv[1], "--horiz"))
	for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
	  force_orient[i] = LSTOPO_ORIENT_HORIZ;
      else if (!strcmp (argv[1], "--vert"))
	for(i=0; i<HWLOC_OBJ_TYPE_MAX; i++)
	  force_orient[i] = LSTOPO_ORIENT_VERT;
      else if (!strncmp (argv[1], "--horiz=", 8)
	       || !strncmp (argv[1], "--vert=", 7)) {
	enum lstopo_orient_e orient = (argv[1][2] == 'h') ? LSTOPO_ORIENT_HORIZ : LSTOPO_ORIENT_VERT;
	char *tmp = argv[1] + ((argv[1][2] == 'h') ? 8 : 7);
	while (tmp) {
	  char *end = strchr(tmp, ',');
	  hwloc_obj_type_t type;
	  if (end)
	    *end = '\0';
	  type = hwloc_obj_type_of_string(tmp);
	  if (type != (hwloc_obj_type_t) -1)
	    force_orient[type] = orient;
	  if (!end)
	    break;
	  tmp = end+1;
        }
      }

      else if (!strcmp (argv[1], "--fontsize")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	fontsize = atoi(argv[2]);
	opt = 1;
      }
      else if (!strcmp (argv[1], "--gridsize")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	gridsize = atoi(argv[2]);
	opt = 1;
      }
      else if (!strcmp (argv[1], "--no-legend")) {
	legend = 0;
      }

      else if (hwloc_utils_lookup_input_option(argv+1, argc-1, &opt,
					       &input, &input_format,
					       callname)) {
	/* nothing to do anymore */

      } else if (!strcmp (argv[1], "--pid")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
	pid = atoi(argv[2]); opt = 1;
      } else if (!strcmp (argv[1], "--ps") || !strcmp (argv[1], "--top"))
        top = 1;
      else if (!strcmp (argv[1], "--version")) {
          printf("%s %s\n", callname, VERSION);
          exit(EXIT_SUCCESS);
      } else if (!strcmp (argv[1], "--output-format") || !strcmp (argv[1], "--of")) {
	if (argc <= 2) {
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	}
        output_format = parse_output_format(argv[2], callname);
        opt = 1;
      } else {
	if (filename) {
	  fprintf (stderr, "Unrecognized option: %s\n", argv[1]);
	  usage (callname, stderr);
	  exit(EXIT_FAILURE);
	} else
	  filename = argv[1];
      }
      argc -= opt+1;
      argv += opt+1;
    }

  if (show_only != (hwloc_obj_type_t)-1)
    merge = 0;

  hwloc_topology_set_flags(topology, flags);

  if (ignorecache > 1) {
    hwloc_topology_ignore_type(topology, HWLOC_OBJ_CACHE);
  } else if (ignorecache) {
    hwloc_topology_ignore_type_keep_structure(topology, HWLOC_OBJ_CACHE);
  }
  if (merge)
    hwloc_topology_ignore_all_keep_structure(topology);

  if (input) {
    err = hwloc_utils_enable_input_format(topology, input, input_format, verbose_mode > 1, callname);
    if (err)
      return err;
  }

  if (pid != (hwloc_pid_t) -1 && pid != 0) {
    if (hwloc_topology_set_pid(topology, pid)) {
      perror("Setting target pid");
      return EXIT_FAILURE;
    }
  }

  err = hwloc_topology_load (topology);
  if (err)
    return EXIT_FAILURE;

  if (top)
    add_process_objects(topology);

  if (restrictstring) {
    hwloc_bitmap_t restrictset = hwloc_bitmap_alloc();
    if (!strcmp (restrictstring, "binding")) {
      if (pid != (hwloc_pid_t) -1 && pid != 0)
	hwloc_get_proc_cpubind(topology, pid, restrictset, HWLOC_CPUBIND_PROCESS);
      else
	hwloc_get_cpubind(topology, restrictset, HWLOC_CPUBIND_PROCESS);
    } else {
      hwloc_bitmap_sscanf(restrictset, restrictstring);
    }
    err = hwloc_topology_restrict (topology, restrictset, 0);
    if (err) {
      perror("Restricting the topology");
      /* fallthrough */
    }
    hwloc_bitmap_free(restrictset);
    free(restrictstring);
  }

  if (!filename && !strcmp(callname,"hwloc-info")) {
    /* behave kind-of plpa-info */
    filename = "-";
    verbose_mode--;
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
    }
  }

  /* if  the output format wasn't enforced, think a bit about what the user probably want */
  if (output_format == LSTOPO_OUTPUT_DEFAULT) {
    if (show_cpuset
        || show_only != (hwloc_obj_type_t)-1
        || verbose_mode != LSTOPO_VERBOSE_MODE_DEFAULT)
      output_format = LSTOPO_OUTPUT_CONSOLE;
  }

  if (logical == -1) {
    if (output_format == LSTOPO_OUTPUT_CONSOLE)
      logical = 1;
    else if (output_format != LSTOPO_OUTPUT_DEFAULT)
      logical = 0;
  }

  switch (output_format) {
    case LSTOPO_OUTPUT_DEFAULT:
#ifdef HWLOC_HAVE_CAIRO
#if CAIRO_HAS_XLIB_SURFACE && defined HWLOC_HAVE_X11
      if (getenv("DISPLAY")) {
        if (logical == -1)
          logical = 0;
        output_x11(topology, NULL, logical, legend, verbose_mode);
      } else
#endif /* CAIRO_HAS_XLIB_SURFACE */
#endif /* HWLOC_HAVE_CAIRO */
#ifdef HWLOC_WIN_SYS
      {
        if (logical == -1)
          logical = 0;
        output_windows(topology, NULL, logical, legend, verbose_mode);
      }
#else
      {
        if (logical == -1)
          logical = 1;
        output_console(topology, NULL, logical, legend, verbose_mode);
      }
#endif
      break;

    case LSTOPO_OUTPUT_CONSOLE:
      output_console(topology, filename, logical, legend, verbose_mode);
      break;
    case LSTOPO_OUTPUT_SYNTHETIC:
      output_synthetic(topology, filename, logical, legend, verbose_mode);
      break;
    case LSTOPO_OUTPUT_TEXT:
      output_text(topology, filename, logical, legend, verbose_mode);
      break;
    case LSTOPO_OUTPUT_FIG:
      output_fig(topology, filename, logical, legend, verbose_mode);
      break;
#ifdef HWLOC_HAVE_CAIRO
# if CAIRO_HAS_PNG_FUNCTIONS
    case LSTOPO_OUTPUT_PNG:
      output_png(topology, filename, logical, legend, verbose_mode);
      break;
# endif /* CAIRO_HAS_PNG_FUNCTIONS */
# if CAIRO_HAS_PDF_SURFACE
    case LSTOPO_OUTPUT_PDF:
      output_pdf(topology, filename, logical, legend, verbose_mode);
      break;
# endif /* CAIRO_HAS_PDF_SURFACE */
# if CAIRO_HAS_PS_SURFACE
    case LSTOPO_OUTPUT_PS:
      output_ps(topology, filename, logical, legend, verbose_mode);
      break;
#endif /* CAIRO_HAS_PS_SURFACE */
#if CAIRO_HAS_SVG_SURFACE
    case LSTOPO_OUTPUT_SVG:
      output_svg(topology, filename, logical, legend, verbose_mode);
      break;
#endif /* CAIRO_HAS_SVG_SURFACE */
#endif /* HWLOC_HAVE_CAIRO */
    case LSTOPO_OUTPUT_XML:
      output_xml(topology, filename, logical, legend, verbose_mode);
      break;
    default:
      fprintf(stderr, "file format not supported\n");
      usage(callname, stderr);
      exit(EXIT_FAILURE);
  }

  hwloc_topology_destroy (topology);

  return EXIT_SUCCESS;
}
