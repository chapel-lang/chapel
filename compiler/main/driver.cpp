#define EXTERN
#include "geysa.h"
#include "analysis.h"
#include "arg.h"
#include "countTokens.h"
#include "driver.h"
#include "files.h"
#include "../ifa/ifa.h"
#include "misc.h"
#include "mysystem.h"
#include "runpasses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "version.h"


static void version(ArgumentState *arg_state, char *arg_unused);
static void help(ArgumentState *arg_state, char *arg_unused);
static void copyright(ArgumentState *arg_state, char *arg_unused);
static void handleLibrary(ArgumentState* arg_state, char* arg_unused);
static void handleLibPath(ArgumentState* arg_state, char* arg_unused);

FILE* html_index_file = NULL;

int fdump_html = 0;
static char passlist_filename[FILENAME_MAX] = "";
static char libraryFilename[FILENAME_MAX] = "";
static char log_flags[512] = "";
static bool rungdb = false;
static int pre_malloc = 0;
bool use_class_init = false;
bool formalTemps = false;
bool checkAnalysisTypeinfo = false;
bool preinstantiate = true;
bool no_infer = false;
bool suppressCodegen = false;
int debugParserLevel = 0;
bool developer = false;
bool ignore_errors = false;
bool _adhoc_to_uniform_mangling = false;
int fnewvardef = 1;
int fdce_if1 = 1;
int fgraph = 0;
int fgraph_constants = 0;
int fgraph_frequencies = 0;
int fgraph_pass_contours = 0;
int fgraph_vcg = 0;
int fcg = 0;
bool no_inline = false;
bool report_inlining = false;
char system_dir[FILENAME_MAX] = DEFAULT_SYSTEM_DIR;
int print_call_depth = 2;
int scoping_test = 0;
int f_equal_method = 0;
int fanalysis_errors = 0;
int fnostdincs = 0;
int num_constants_per_variable = 1;

static ArgumentDescription arg_desc[] = {
 {"nostdincs", ' ', "No Standard Includes", "T", &fnostdincs, "CHPL_NOSTDINCS", NULL},
 {"analysis-errors", ' ', "Pass Back Analysis Errors", "T", &fanalysis_errors, "CHPL_ANALYSIS_ERRORS", NULL},
 {"premalloc", 'm', "Pre-Malloc", "I", &pre_malloc, "CHPL_PRE_MALLOC", NULL},
 {"passlist", ' ', "Passlist Filename", "P", passlist_filename, "CHPL_PASSLIST", NULL},
 {"sysdir", 'S', "System Directory", "P", system_dir, "CHPL_SYSTEM_DIR", NULL},
 {"scoping", ' ', "Scoping Test", "T", &scoping_test, "CHPL_SCOPING_TEST", NULL},
 {"if1-dce", ' ', "Dead Code Elimination on IF1", "T", &fdce_if1, "CHPL_DCE_IF1", NULL},
 {"print-call-depth", 'C', "Print Calls to this Depth", "I", &print_call_depth, "CHPL_PRINT_CALL_DEPTH", NULL},
 {"constant-limit", ' ', "# of Constants Propagated", "I", &num_constants_per_variable, 
  "CHPL_CONSTANTS_PER_VARIABLE", NULL},
 {"devel", ' ', "Developer Compile", "F", &developer, "CHPL_DEVELOPER", NULL},
 {"ignore-errors", ' ', "Attempt to ignore errors", "F", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"html", 't', "Write Program in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"lowlevel-cg", 'g', "Low Level Code Generation", "T", &fcg, "CHPL_CG", NULL},
 {"graph", 'G', "Write Program Graphs", "T", &fgraph, "CHPL_GRAPH", NULL},
 {"graph-format", ' ', "GraphViz = 0, VCG = 1", "I", &fgraph_vcg, "CHPL_GRAPHFORMAT", NULL},
 {"graph-constants", ' ', "Graph Constants", "T", &fgraph_constants, 
  "CHPL_GRAPH_CONSTANTS", NULL},
 {"graph-frequencies", ' ', "Graph Frequencies", "T", &fgraph_frequencies, 
  "CHPL_GRAPH_FREQUENCIES", NULL},
 {"graph-contours", ' ', "Graph Analysis Pass Contours", "T", &fgraph_pass_contours, 
  "CHPL_GRAPH_CONTOURS", NULL},
 {"log-dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'd', "Debug Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"cg-cpp-lines", ' ', "Generate #line directives", "F", &printCppLineno, "CHPL_CG_CPP_LINES", NULL},
 {"cg-chpl-lineno", ' ', "Generate chpl input line numbers", "F", &printChplLineno, "CHPL_CG_CHPL_LINENO", NULL},
 {"lib-linkage", 'l', "Library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"lib-search-path", 'L', "Library search path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"output", 'o', "Name of Executable Output", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"savec", ' ', "Save Intermediate C Code", "P", saveCDir, "CHPL_SAVEC_DIR", NULL},
 {"gdb", ' ', "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"no-infer", 'b', "Do not run analysis, inference", "F", &no_infer, NULL, NULL},
 {"no-inline", ' ', "No inlining of functions" , "F", &no_inline, NULL, NULL},
 {"report-inlining", ' ', "Print inlined functions", "F", &report_inlining, NULL, NULL},
 {"formal-temps", ' ', "Insert temps for formals", "F", &formalTemps, "CHPL_FORMAL_TEMPS", NULL},
 {"use-class-init", ' ', "Use class init for DefExprs", "F", &use_class_init, "USE_CLASS_INIT", NULL},
 {"check-typeinfo", ' ', "Check result of type_info and return_type_info", "F", &checkAnalysisTypeinfo, NULL, NULL},
 {"nopreinstantiate", ' ', "Do not instantiate before analysis", "f", &preinstantiate, NULL, NULL},
 {"tmpMangling", ' ', "Name mangling toggle", "F", &_adhoc_to_uniform_mangling, "CHPL_ADHOC_TO_UNIFORMA_MANGLING", NULL},
 {"no-codegen", ' ', "Suppress code generation", "F", &suppressCodegen, "CHPL_NO_CODEGEN", NULL},
 {"newvardef", ' ', "New Var Def code", "T", &fnewvardef, "CHPL_NEWVARDEF", NULL},
 {"equal-method", ' ', "= is a method", "T", &f_equal_method, "CHPL_EQUAL_METHOD", NULL},
 {"parser-debug", 'D', "Parser Debug Level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"count-tokens", ' ', "Count Tokens", "F", &countTokens, "CHPL_COUNT_TOKENS", NULL},
 {"print-tokens", ' ', "Print Tokens", "F", &printTokens, "CHPL_PRINT_TOKENS", NULL},
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
  addLibInfo(stringcat("-l", libraryFilename));
}


static void handleLibPath(ArgumentState* arg_state, char* arg_unused) {
  addLibInfo(stringcat("-L", libraryFilename));
}

void
do_analysis(char *fn) {
  if (ifa_analyze(fn) < 0)
    fail("program does not type");
  if (fgraph)
    ifa_graph(fn);
  if (fdump_html)
    ifa_html(fn);
  if (fcg) {
    ifa_cg(fn);
    ifa_compile(fn);
  }
  return;
}

static void
compile_all(void) {
  Symboltable::init();
  initType(); // BLC : clean these up
  initSymbol();
  initExpr();
  testInputFiles(arg_state.nfile_arguments, arg_state.file_argument);
  runPasses(passlist_filename);
}

static void
init_system() {
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
  char* argv0 = stringcpy(orig_argv0);
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
  char* command = stringcat("gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", 0);

  clean_exit(status);
}

int
main(int argc, char *argv[]) {
  if (pre_malloc)
    (void)MALLOC(pre_malloc);
  compute_program_name_loc(argv[0], &(arg_state.program_name),
                           &(arg_state.program_loc));
  process_args(&arg_state, argc, argv);
  startCatchingSignals();
  if (arg_state.nfile_arguments < 1)
    help(&arg_state, NULL);
  graph_type = !fgraph_vcg ? GraphViz : VCG;
  if (rungdb)
    runCompilerInGDB(argc, argv);
  if (fdump_html || strcmp(log_flags, ""))
    init_logs();
  init_system();
  init_chapel_ifa();
  compile_all();
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
