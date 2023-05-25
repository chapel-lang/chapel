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

#ifdef HAVE_GASNET_TOOLS
#define GASNETT_LITE_MODE /* preserves AMUDP's threading neutrality */
#include <gasnet_tools.h> /* for ctype.h wrappers */
#else
#include <ctype.h>
#endif

#include <amudp.h>
#include <amudp_spmd.h>

extern char *AMUDP_getenv_prefixed(const char *); // for probing settings
#ifndef AMX_ENV_PREFIX_STR
  #ifdef AMX_ENV_PREFIX
    #define AMX_ENV_PREFIX_STR AMX_STRINGIFY(AMX_ENV_PREFIX)
  #else
    #define AMX_ENV_PREFIX_STR "AMUDP_"
  #endif
#endif

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
    "  -v        Enable verbose output, repeated use increases verbosity\n"
    "  -h        Show this help\n\n"
    , argvzero, AMUDP_LIBRARY_VERSION_STR, argvzero);
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

  if (AMUDP_SPMDIsWorker(argv)) Usage("launcher cannot be invoked as target program");

  argvzero = argv[0];
  argv++; argc--; /* strip launcher name */

  while (argc && argv[0][0] == '-') {
    const char *arg = argv[0];
    if (arg[1] == '-') arg++;
    if (!strcmp(arg,"-v")) {
      verbose++;
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
    } else Usage("unknown option");
    argv++; argc--;
  }
  if (argc < 1) Usage("missing program name");

  AMX_VerboseErrors = 1;

  if (verbose >= 1) {
    if (!AMUDP_getenv_prefixed("VERBOSEENV")) {
      putenv((char *)AMX_ENV_PREFIX_STR "_VERBOSEENV=1");
    }
  }
  if (verbose >= 2) {
    int i;
    if (!AMUDP_getenv_prefixed("SPAWN_VERBOSE")) { 
      putenv((char *)AMX_ENV_PREFIX_STR "_SPAWN_VERBOSE=1");
    }
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
