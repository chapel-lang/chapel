#include "defs.h"
#include "arg.h"
#include "version.h"
#include "log.h"

static  int fhtml = 0;
static  int fgraph = 0;
static int fgraph_vcg = 0;
static char log_flags[512] = "";

static void help(ArgumentState *arg_state, char *arg_unused);
static void copyright(ArgumentState *arg_state, char *arg_unused);

static ArgumentDescription arg_desc[] = {
 {"html", 't', "Write Program in HTML", "T", &fhtml, "IFA_HTML", NULL},
 {"graph", 'G', "Write Program Graphs", "T", &fgraph, "IFA_GRAPH", NULL},
 {"graph-format", ' ', "GraphViz = 0, VCG = 1", "I", &fgraph_vcg, "IFA_GRAPH_FORMAT", NULL},
 {"log-dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'd', "Debug Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"debug", 'd', "Debug", "+", &ifa_debug, "IFA_DEBUG", NULL},
 {"verbose", 'v', "Verbose", "+", &ifa_verbose, "IFA_VERBOSE", NULL},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, copyright},
 {"help", 'h', "Help", NULL, NULL, NULL, help},
 {0}
};

static ArgumentState arg_state = {
  0, 0,
  "program", 
  "path",
  arg_desc
};

static void
copyright(ArgumentState *arg_state, char *arg_unused) {
  fprintf(stderr, "\n"
#include "COPYRIGHT"
          "\n"
          );
  exit(0);
}

static void
short_copyright() {
  fprintf(stderr, "Copyright (c) 1992-2006 John Plevyak, 2003-2006 Cray Inc.\n");  
}

static void
help(ArgumentState *arg_state, char *arg_unused) {
  char ver[30];
  ifa_version(ver);
  fprintf(stderr, "IFA Version %s ", ver);  
  short_copyright();
  usage(arg_state, arg_unused);
}

int
main(int argc, char *argv[]) {
  MEM_INIT();
  process_args(&arg_state, argc, argv);
  if (arg_state.nfile_arguments < 1)
    help(&arg_state, NULL);
  graph_type = !fgraph_vcg ? GraphViz : VCG;
  if (fhtml || log_flags[0])
    init_logs();
  return 0;
}
