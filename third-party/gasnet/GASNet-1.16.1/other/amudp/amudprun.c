/*   $Source: /var/local/cvs/gasnet/other/amudp/amudprun.c,v $
 *     $Date: 2006/05/23 12:42:29 $
 * $Revision: 1.2 $
 * Description: Stand-alone AMUDP job launcher
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <amudp.h>
#include <amudp_spmd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    "  -h        Show this help\n"
    "\n"
    , argvzero, _STRINGIFY(AMUDP_LIBRARY_VERSION), argvzero);
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
