#define EXTERN
#include "geysa.h"
#include "parse.h"
#include "if1.h"
#include "analysis.h"
#include "arg.h"
#include "cg.h"
#include "clone.h"
#include "dead.h"
#include "dom.h"
#include "driver.h"
#include "dump.h"
#include "files.h"
#include "fa.h"
#include "fun.h"
#include "graph.h"
#include "inline.h"
#include "misc.h"
#include "mysystem.h"
#include "pdb.h"
#include "runpasses.h"
#include "stringutil.h"
#include "symtab.h"
#include "version.h"


static void version(ArgumentState *arg_state, char *arg_unused);
static void help(ArgumentState *arg_state, char *arg_unused);
static void copyright(ArgumentState *arg_state, char *arg_unused);
static void handleLibrary(ArgumentState* arg_state, char* arg_unused);
static void handleLibPath(ArgumentState* arg_state, char* arg_unused);


static int fdump_html = 0;
char prelude_filename[FILENAME_MAX] = "prelude";
static char passlist_filename[FILENAME_MAX] = "";
static char libraryFilename[FILENAME_MAX] = "";
static char log_flags[512] = "";
extern int d_verbose_level;
int parser_verbose_non_prelude = 0;
static bool rungdb = false;
static int pre_malloc = 0;
bool checkAnalysisTypeinfo = false;
bool applyGettersSetters = false;
bool analyzeAST = true;
bool suppressCodegen = false;
int debugParserLevel = 0;
bool developer = false;
bool ignore_errors = false;
int fnewvardef = 0;
int fdce_if1 = 1;
int finline = 0;
int fsimple_inline = 0;
int fgraph = 0;
int fgraph_constants = 0;
int fgraph_frequencies = 0;
int fgraph_vcg = 0;
int fcg = 0;
char system_dir[FILENAME_MAX] = DEFAULT_SYSTEM_DIR;
int print_call_depth = 2;
int scoping_test = 0;

static ArgumentDescription arg_desc[] = {
 {"premalloc", 'm', "Pre-Malloc", "I", &pre_malloc, "CHPL_PRE_MALLOC", NULL},
 {"prelude", 'p', "Prelude Filename", "P", prelude_filename, "CHPL_PRELUDE", NULL},
 {"passlist", ' ', "Passlist Filename", "P", passlist_filename, "CHPL_PASSLIST", NULL},
 {"sysdir", 'S', "System Directory", "P", system_dir, "CHPL_SYSTEM_DIR", NULL},
 {"scoping", ' ', "Scoping Test", "T", &scoping_test, "CHPL_SCOPING_TEST", NULL},
 {"dce_if1", ' ', "Dead Code Elimination on IF1", "T", &fdce_if1, "CHPL_DCE_IF1", NULL},
 {"print-call-depth", 'C', "Print Call Depth", "I", &print_call_depth, "CHPL_PRINT_CALL_DEPTH", NULL},
 {"constants_per_var", ' ', "# of Constants per Var", "I", &num_constants_per_variable, 
  "CHPL_CONSTANTS_PER_VARIABLE", NULL},
 {"devel", ' ', "Developer Compile", "F", &developer, "CHPL_DEVELOPER", NULL},
 {"ignore_errors", ' ', "Attempt to ignore errors", "F", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"inline", ' ', "Inlining", "T", &finline, "CHPL_INLINE", NULL},
 {"simple_inline", ' ', "Simple Inlining", "T", &fsimple_inline, "CHPL_SIMPLE_INLINE", NULL},
 {"html", 't', "Dump Program in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"lowlevel_cg", 'g', "Low Level Code Generation", "T", &fcg, "CHPL_CG", NULL},
 {"graph", 'G', "Dump Program Graphs", "T", &fgraph, "CHPL_GRAPH", NULL},
 {"graph_var", ' ', "Limit Graph to Var", "S80", graph_var, "CHPL_GRAPH_VAR", NULL},
 {"graph_fun", ' ', "Limit Graph to Fun", "S80", graph_fun, "CHPL_GRAPH_FUN", NULL},
 {"graph_vcg", ' ', "VCG Output", "T", &fgraph_vcg, "CHPL_GRAPHVCG", NULL},
 {"graph_constants", ' ', "Graph Constants", "T", &fgraph_constants, 
  "CHPL_GRAPH_CONSTANTS", NULL},
 {"graph_frequencies", ' ', "Graph Frequencies", "T", &fgraph_frequencies, 
  "CHPL_GRAPH_FREQUENCIES", NULL},
 {"log_dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'd', "Debug Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"", 'l', "Library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"", 'L', "Library search path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"output", 'o', "Name of Executable Output", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"savec", ' ', "Save Intermediate C Code", "P", saveCDir, "CHPL_SAVEC_DIR", NULL},
 {"gdb", ' ', "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"noanalysis", ' ', "Skip analysis of AST", "f", &analyzeAST, NULL, NULL},
 {"checkAnalysisTypeinfo", ' ', "Check result of type_info and return_type_info", "F", &checkAnalysisTypeinfo, NULL, NULL},
 {"apply-getters-setters", ' ', "Apply getters and setters preanalysis", "F", &applyGettersSetters, NULL, NULL},
 {"tmpRoxIndexType", ' ', "Roxana's index toggle", "F", &_dtinteger_IndexType_switch, "CHPL_DTINTEGER_INDEXTYPE_SWITCH", NULL},
 {"no-codegen", ' ', "Suppress code generation", "F", &suppressCodegen, "CHPL_NO_CODEGEN", NULL},
 {"newvardef", ' ', "New Var Def code", "T", &fnewvardef, "CHPL_NEWVARDEF", NULL},
 {"parser_verbose_np", ' ', "Parser Verbose Non-Prelude", "+", 
  &parser_verbose_non_prelude, "CHPL_PARSER_VERBOSE_NON_PRELUDE", NULL},
 {"parser_verbose", 'V', "Parser Verbose Level", "+", &d_verbose_level, 
   "CHPL_PARSER_VERBOSE", NULL},
 {"parser_debug", 'D', "Parser Debug Level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"verbose", 'v', "Verbose Level", "+", &verbose_level, "CHPL_VERBOSE", NULL},
 {"print-commands", ' ', "Print Subprocess Commands", "F", &printSystemCommands, 
  "CHPL_PRINT_COMMANDS", NULL},
 {"print-passes", ' ', "Print Passes", "F", &printPasses, "CHPL_PRINT_PASSES", 
  NULL},
 {"version", ' ', "Show Version", NULL, NULL, NULL, version},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, copyright},
 {"help", 'h', "Help (show this list)", NULL, NULL, NULL, help},
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
#include "LICENSE"
          "\n"
          );
  clean_exit(0);
}


static void printShortCopyright(void) {
  fprintf(stderr, "\n"
#include "COPYRIGHT"
          );
}


static void printVersion(ArgumentState* arg_state) {
  char ver[30];
  get_version(ver);
  fprintf(stderr, "%s Version %s", arg_state->program_name, ver);  
  printShortCopyright();
}

static void version(ArgumentState* arg_state, char* arg_unused) {
  printVersion(arg_state);
  exit(0);
}

static void
help(ArgumentState *arg_state, char *arg_unused) {
  printVersion(arg_state);
  usage(arg_state, arg_unused);
}

static void handleLibrary(ArgumentState* arg_state, char* arg_unused) {
  addLibInfo(glomstrings(2, "-l", libraryFilename));
}


static void handleLibPath(ArgumentState* arg_state, char* arg_unused) {
  addLibInfo(glomstrings(2, "-L", libraryFilename));
}


void
do_analysis(char *fn) {
  if1_finalize(if1);
  if1_write_log();
  if (!fdce_if1)
    fail("unable to translate dead code... terminating");
  Sym *init = if1_get_builtin(if1, "init");
  for (int i = 0; i < if1->allclosures.n; i++) {
    Fun *f = new Fun(if1->allclosures.v[i], if1->allclosures.v[i] == init);
    if (!f)
      fail("fatal error, IF1 invalid");
    pdb->add(f);
  }
  FA *fa = pdb->fa;
  if (fa->analyze(if1->top->fun) < 0)
    goto Lfail;
  if (clone(fa, if1->top->fun) < 0)
    goto Lfail;
  if (mark_dead_code(fa, if1->top->fun) < 0)
    fail("fatal error, dead code detection failed");
  if (logging(LOG_TEST_FA))
    log_test_fa(fa);
  forv_Fun(f, fa->funs)
    build_forward_cfg_dominators(f);
  frequency_estimation(fa);
  if (fgraph)
    graph(fa, fn, !fgraph_vcg ? GraphViz : VCG);
  if (finline)
    inline_calls(fa);
  else if (fsimple_inline)
    simple_inline_calls(fa);
  if (fdump_html)
    dump_html(fa, fn);
  if (fcg) {
    cg_write_c(fa, if1->top->fun,  fn);
    cg_compile(fn);
  }
  return;
 Lfail:
  fail("fatal error, program does not type");
}

static void
compile_all(void) {
  bool noTestLangFiles = true;
  for (int i = 0; i < arg_state.nfile_arguments; i++) 
    if (is_test_lang(arg_state.file_argument[i])) noTestLangFiles = false;

  if (noTestLangFiles) {
    Symboltable::init();
    initTypes(); // BLC : clean these up
    initExpr();
    FnSymbol::init();
    testInputFiles(arg_state.nfile_arguments, arg_state.file_argument);
    runPasses(passlist_filename);
  } else
    for (int i = 0; i < arg_state.nfile_arguments; i++)
      if (compile_one_test_file(arg_state.file_argument[i])) break;
}


static void
init_system() {
  new IF1;
  new PDB(if1);
  char cwd[FILENAME_MAX];
  if (system_dir[0] == '.' && (!system_dir[1] || system_dir[1] == '/')) {
    getcwd(cwd, FILENAME_MAX);
    strcat(cwd, system_dir + 1);
    memcpy(system_dir, cwd, sizeof(system_dir));
  }
  if (system_dir[strlen(system_dir)-1] != '/') 
    strcat(system_dir, "/");
}

static void
compute_program_name_loc(char* orig_argv0, char** name, char** loc) {
  char* argv0 = copystring(orig_argv0);
  char* lastslash = strrchr(argv0, '/');
  if (lastslash == NULL) {
    *name = argv0;
    *loc = ".";   // BLC: this is inaccurate; we should search the path.  
                  // It's no less accurate than what we did previously, though.
  } else {
    *lastslash = '\0';
    *name = lastslash+1;
    *loc = argv0;
  }
  strcpy(system_dir, *loc);
}


void runCompilerInGDB(int argc, char* argv[]) {
  char* gdbCommandFilename = createGDBFile(argc, argv);
  char* command = glomstrings(4, "gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", 0);

  clean_exit(status);
}

int
main(int argc, char *argv[]) {
  if (pre_malloc)
    (void)MALLOC(pre_malloc);
  startCatchingSignals();
  
  compute_program_name_loc(argv[0], &(arg_state.program_name),
                           &(arg_state.program_loc));
  process_args(&arg_state, argc, argv);
  if (arg_state.nfile_arguments < 1)
    help(&arg_state, NULL);
  if (rungdb)
    runCompilerInGDB(argc, argv);
  if (fdump_html || strcmp(log_flags, "") || fgraph)
    init_logs();
  init_system();
  compile_all();
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
