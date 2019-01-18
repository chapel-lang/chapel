/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudprun.c $
 * Description: Stand-alone AMUDP job launcher
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef AMUDP_BLCR_ENABLED /* bug 3328: avoid broken clang/Cygwin header */
#include <dirent.h>
#endif

#ifdef HAVE_GASNET_TOOLS
#define GASNETT_LITE_MODE /* preserves AMUDP's threading neutrality */
#include <gasnet_tools.h> /* for ctype.h wrappers */
#else
#include <ctype.h>
#endif

#include <amudp.h>
#include <amudp_spmd.h>

static const char *argvzero;
static void Usage(const char *msg) {
  int i;
  if (msg) fprintf(stderr, "ERROR: %s\n\n", msg);
  fprintf(stderr, 
    "%s, version %s\n"
    "Usage: %s [<options>] <program_name> [<program arguments>...]\n\n"
    "  -np N     Spawn N processes (may also be spelled -N or -n)\n"
    "  -spawn F  Use spawning function F\n"
    "  -depth D  Use network depth D\n"
    "  -v        Enable verbose mode spawn\n"
    "  -h        Show this help\n\n"
    , argvzero, AMUDP_LIBRARY_VERSION_STR, argvzero);
#ifdef AMUDP_BLCR_ENABLED
  fprintf(stderr, 
    "Usage: %s -restart <checkpoint_directory>\n\n"
    , argvzero);
#endif
  fprintf (stderr, "Available spawn functions:\n");
  for (i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
    fprintf(stderr, "    '%c'  %s\n",  
          toupper(AMUDP_Spawnfn_Desc[i].abbrev), AMUDP_Spawnfn_Desc[i].desc);
  }
  fprintf(stderr, "\n");
  exit(1);
}

static amudp_spawnfn_t spawnfn = NULL;
static char spawnfn_abbr = '-';
static const char *spawnfn_str = "<default>";
static void set_spawnfn(const char *str) {
  int i;
  spawnfn_abbr = toupper(str[0]);
  if (str[1]) Usage("Invalid spawn function");
  for (i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
    if (spawnfn_abbr == toupper(AMUDP_Spawnfn_Desc[i].abbrev)) {
      spawnfn = AMUDP_Spawnfn_Desc[i].fnptr;
      spawnfn_str = AMUDP_Spawnfn_Desc[i].desc;
      break;
    }
  }
  if (!spawnfn) Usage("Unknown spawn value for -spawn");
}

int main(int argc, char **argv) {
  int verbose = 0;
  int nproc = 0;
  int networkdepth = 0;

#ifdef AMUDP_BLCR_ENABLED
  int procid = AMUDP_SPMDRestartProcId(&argc, &argv);
  if (procid >= 0) {
    char filename[32];
    int magic_fd = -1;
    int context_fd = -1;

    /* Save opaque "magic" (argv[2]) in a temporary file */
    { size_t len;
      char *tempname;
      const char *tmpdir;
      const char *magic = argv[2];
      int rc;

      tmpdir = getenv("TMPDIR");
      if (!tmpdir) tmpdir = (const char *)"/tmp";
      tempname = (char *)malloc(13 + strlen(tmpdir));
      strcpy(tempname, tmpdir);
      strcat(tempname, "/amudpXXXXXX");
      magic_fd = mkstemp(tempname);
      if (magic_fd < 0) {
        perror("Failed to mkstemp()");
        exit(1);
      }
      (void)unlink(tempname);
      len = 1 + strlen(magic);
      rc = (int)write(magic_fd, magic, len);
      if (rc < (ssize_t)len) {
        perror("Failed to write() restart magic file");
        exit(1);
      }
      if ((off_t)0 != lseek(magic_fd, 0, SEEK_SET)) {
        perror("Failed to rewind restart magic file");
        exit(1);
      }
    }

    /* Open context file and dup() magic_fd to its reserved fd */
    snprintf(filename, sizeof(filename), "context.%d", (int)procid);
    { int special_fd;
      ssize_t bytesread;

      if (0 != chdir(argv[1])) {
        fprintf(stderr, "ERROR: failed to change to restart directory '%s'\n", argv[1]);
        return 1;
      }

      context_fd = open(filename, O_RDONLY);
      if (context_fd < 0) {
        perror("Failed to open() context file");
        exit(1);
      }
      bytesread = read(context_fd, &special_fd, sizeof(int));
      if (bytesread != (ssize_t)sizeof(int)) {
        perror("Failed to read() context file");
        exit(1);
      }

      if (context_fd == special_fd) {
        int tmp = dup(context_fd);
        if (0 > tmp) {
          perror("Failed to dup() context file");
          exit(1);
        }
        close(context_fd);
        context_fd = tmp;
      }
      if (magic_fd != special_fd) {
        if (0 > dup2(magic_fd, special_fd)) {
          perror("Failed to dup2() magic file");
          exit(1);
        }
        (void)close(magic_fd);
      }
    }

    /* BLCR-TODO: close any "stray" files or mark them close-on-exec */

    /* Append BLCR's bindir to PATH */
    {
      const char *bindir = BLCR_BINDIR;
      const char *path = getenv("PATH");
      const size_t len = strlen(path) + strlen(bindir) + 2; /* 2 = ':' and '\0' */
      char *new_path = malloc(len);
      strcpy(new_path, path);
      strcat(new_path, ":" BLCR_BINDIR);
      setenv("PATH", new_path, 1);
    }

    /* Now the actual restart */
    /* BLCR-TODO: use cr_request_restart()? */
    {
      char fd_arg[10];
      snprintf(fd_arg, sizeof(fd_arg), "%d", context_fd);
      execlp("cr_restart", "cr_restart", "-F", fd_arg, (char*)NULL);
    }
    perror("Failed execlp() restart command");
    exit(1);
  }
#endif /* AMUDP_BLCR_ENABLED */

  if (AMUDP_SPMDIsWorker(argv)) Usage("launcher cannot be invoked as target program");

  argvzero = argv[0];
  argv++; argc--; /* strip launcher name */

  while (argc && argv[0][0] == '-') {
    const char *arg = argv[0];
    if (arg[1] == '-') arg++;
    if (!strcmp(arg,"-v")) {
      verbose = 1;
    } else if (!strcmp(arg,"-np") || 
               !strcmp(arg,"-n") || 
               !strcmp(arg,"-N")) {
      if (argc < 2) Usage("Missing process count for -np");
      argv++; argc--;
      nproc = atoi(argv[0]);
      if (nproc < 1) Usage("Bad process count for -np");
    } else if (!strcmp(arg,"-depth")) {
      if (argc < 2) Usage("Missing depth value for -depth");
      argv++; argc--;
      networkdepth = atoi(argv[0]);
      if (networkdepth < 1) Usage("Bad depth value for -depth");
    } else if (!strcmp(arg,"-spawn")) {
      if (argc < 2) Usage("Missing spawn value for -spawn");
      argv++; argc--;
      set_spawnfn(argv[0]);
    } else if (!strcmp(arg,"-h") || 
               !strcmp(arg,"-H") || 
               !strcmp(arg,"-help")) {
      Usage(NULL);
#ifdef AMUDP_BLCR_ENABLED
    } else if (!strcmp(arg,"-restart")) {
      if (argc < 2) Usage("Missing directory name for -restart");
      { /* determine nproc by counting context files, and check for gaps */
        int rc, maxN = -1;
        DIR *dir;
        struct dirent *d;

        dir = opendir(argv[1]);
        if (NULL == dir) {
          fprintf(stderr, "ERROR: failed to open restart directory '%s'\n", argv[1]);
          return 1;
        }
        nproc = 0;
        while (NULL != (d = readdir(dir))) {
          static char pattern[] = "context.";
          const size_t len = sizeof(pattern) - 1;
          if (0 == strncmp(d->d_name, pattern, len)) {
            int id = atoi(d->d_name + len);
            char tmpname[32];
            snprintf(tmpname, sizeof(tmpname)-1, "context.%d", id);
            if (0 == strcmp(d->d_name, tmpname)) {
              ++nproc;
              maxN = (id>maxN) ? id : maxN;
            }
          }
        }
        closedir(dir);
        if (nproc == 0) {
          fprintf(stderr, "ERROR: failed to find context files in restart directory '%s'\n", argv[1]);
          return 1;
        } else if (nproc != maxN+1) {
          fprintf(stderr, "ERROR: missing one or more context files in restart directory '%s'\n", argv[1]);
          return 1;
        }
      }
      /* BLCR-TODO: need an absolute path (or should we *require* one?) */
      AMUDP_SPMDRunRestart(strdup(argvzero), argv[1], nproc);
      /* should never return */
      fprintf(stderr, "ERROR: AMUDP_SPMDRunRestart failed\n");
      return 1;
#endif /* AMUDP_BLCR_ENABLED */
    } else Usage("unknown option");
    argv++; argc--;
  }
  if (argc < 1) Usage("missing program name");

  AMX_VerboseErrors = 1;

  if (verbose) {
    int i;
    if (!getenv("AMUDP_VERBOSEENV")) putenv((char *)"AMUDP_VERBOSEENV=1");
    fprintf(stderr,"%s: Launching: ",argvzero);
    for (i = 0; i < argc; i++) {
      fprintf(stderr,"%s ", argv[i]);
    }
    fprintf(stderr,"\n");
    fprintf(stderr,"%s: nprocs=%i depth=%i spawnfn=%c(%s)\n",
      argvzero,nproc,networkdepth,spawnfn_abbr,spawnfn_str);
  }

  { /* call startup */
    eb_t eb;
    ep_t ep;
    uint64_t networkpid;
    int retval =
      AMUDP_SPMDStartup(&argc, &argv, 
                        nproc, networkdepth, 
                        spawnfn, &networkpid, &eb, &ep);
    /* should never return */
    fprintf(stderr, "ERROR: AMUDP_SPMDStartup failed: %i\n", retval);
  }
  return 1;
}
