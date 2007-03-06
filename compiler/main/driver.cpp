#define EXTERN
#include "chpl.h"
#include "arg.h"
#include "countTokens.h"
#include "driver.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "runpasses.h"
#include "stmt.h"
#include "stringutil.h"
#include "version.h"
#include "log.h"
#include "primitive.h"


static void printVersionAndExit(ArgumentState *arg_state, char *arg_unused);
static void printHelpAndExit(ArgumentState *arg_state, char *arg_unused);
static void printCopyrightAndExit(ArgumentState *arg_state, char *arg_unused);
static void printLicenseAndExit(ArgumentState* arg_state, char* arg_unused);
static void handleLibrary(ArgumentState* arg_state, char* arg_unused);
static void handleLibPath(ArgumentState* arg_state, char* arg_unused);

FILE* html_index_file = NULL;

int fdump_html = 0;
static char libraryFilename[FILENAME_MAX] = "";
static char log_flags[512] = "";
static bool rungdb = false;
bool no_codegen = false;
int debugParserLevel = 0;
bool developer = false;
bool ignore_errors = false;
int trace_level = 0;
int fcg = 0;
bool fScalarReplaceTuples = false;
bool fDisableScalarReplaceArrayWrappers = false;
bool fDisableIteratorPropagation = false;
bool unoptimized = false;
bool no_bounds_checking = false;
bool no_gc = false;
bool no_inline = false;
bool report_inlining = false;
char chplhome[FILENAME_MAX] = ".";
char fPrintStatistics[256] = "";
bool fPrintDispatch = false;
bool fWarnPromotion = false;
bool fWarnTemporary = false;
int fnostdincs = 0;
int num_constants_per_variable = 1;
char defaultDistribution[256] = "SingleLocaleDistribution";
int instantiation_limit = 256;
bool parallelPass = true;

static ArgumentDescription arg_desc[] = {
 {"", ' ', "Compilation Traces", NULL, NULL, NULL, NULL},
 {"print-passes", ' ', "Print compiler passes", "F", &printPasses, "CHPL_PRINT_PASSES", NULL},
 {"print-commands", ' ', "Print system commands", "F", &printSystemCommands, "CHPL_PRINT_COMMANDS", NULL},

 {"", ' ', "Code Size Statistics", NULL, NULL, NULL, NULL},
 {"print-code-size", ' ', "Print code size statistics", "F", &printTokens, "CHPL_PRINT_TOKENS", NULL},
 {"count-tokens", ' ', "Count tokens", "F", &countTokens, "CHPL_COUNT_TOKENS", NULL},

 {"", ' ', "Optimization Control", NULL, NULL, NULL, NULL},
 {"no-bounds-checks", ' ', "Disable bounds checking", "F", &no_bounds_checking, "CHPL_NO_BOUNDS_CHECKING", NULL},
 {"no-gc", ' ', "Disable memory management", "F", &no_gc, "CHPL_NO_GC", NULL},
 {"no-inline", ' ', "Do not inline functions" , "F", &no_inline, NULL, NULL},
 {"unoptimized", ' ', "Generate naive code (many temps)", "F", &unoptimized, "CHPL_UNOPTIMIZED", NULL},
 {"scalar-replace-tuples", ' ', "Generate scalar components of tuples", "F", &fScalarReplaceTuples, "CHPL_SCALAR_REPLACE_TUPLES", NULL},
 {"no-iterator-propagation", ' ', "Disable propagation of iterators to avoid sequence temporaries", "F", &fDisableIteratorPropagation, "CHPL_DISABLE_ITERATOR_PROPAGATION", NULL},

 {"", ' ', "Code Generation", NULL, NULL, NULL, NULL},
 {"cg-cpp-lines", ' ', "Generate #line annotations", "F", &printCppLineno, "CHPL_CG_CPP_LINES", NULL},
 {"savec", ' ', "Save generated C code", "P", saveCDir, "CHPL_SAVEC_DIR", NULL},
 {"parallel", 'p', "Toggle threaded code generation", "T", &parallelPass, "CHPL_PARALLELIZE", NULL},

 {"", ' ', "Linker Control", NULL, NULL, NULL, NULL},
 {"output", 'o', "Name output executable", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"lib-search-path", 'L', "C library search path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"lib-linkage", 'l', "C library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"ccflags", ' ', "Back-end C compiler flags", "S256", ccflags, "CHPL_CC_FLAGS", NULL},

 {"", ' ', "Miscellaneous Flags", NULL, NULL, NULL, NULL},
 {"instantiate-max", ' ', "Limit number of instantiations", "I", &instantiation_limit, "CHPL_INSTANTIATION_LIMIT", NULL},
 {"chplhome", ' ', "Over-ride $CHPL_HOME", "P", chplhome, "CHPL_HOME", NULL},
 {"devel", ' ', "Compile as developer", "F", &developer, "CHPL_DEVELOPER", NULL},

 {"", ' ', "Compiler Information", NULL, NULL, NULL, NULL},
 {"version", ' ', "Show Version", NULL, NULL, NULL, printVersionAndExit},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, printCopyrightAndExit},
 {"license", ' ', "Show License", NULL, NULL, NULL, printLicenseAndExit},
 {"help", 'h', "Help (show this list)", NULL, NULL, NULL, printHelpAndExit},

 {"", ' ', "Developer Flags", NULL, NULL, NULL, NULL},
 {"", ' ', "Debug Output", NULL, NULL, NULL, NULL},
 {"ccwarnings", ' ', "Give warnings for generated code", "T", &ccwarnings, "CHPL_CC_WARNINGS", NULL},
 {"html", 't', "Dump IR in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"print-statistics", ' ', "Print AST statistics", "S256", fPrintStatistics, NULL, NULL},
 {"print-dispatch", ' ', "Print dynamic dispatch table", "F", &fPrintDispatch, NULL, NULL},
 {"parser-debug", 'D', "Set parser debug level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"report-inlining", ' ', "Print inlined functions", "F", &report_inlining, NULL, NULL},
 {"log", 'd', "Specify debug logs", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"log-dir", ' ', "Specify log directory", "P", log_dir, "CHPL_LOG_DIR", NULL},

 {"", ' ', "Misc. Developer Flags", NULL, NULL, NULL, NULL},
 {"gdb", ' ', "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"nostdincs", ' ', "Don't use standard modules", "T", &fnostdincs, "CHPL_NOSTDINCS", NULL},
 {"no-codegen", ' ', "Suppress code generation", "F", &no_codegen, "CHPL_NO_CODEGEN", NULL},
 {"ignore-errors", ' ', "Attempt to ignore errors", "F", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"warn-promotion", ' ', "Warn about scalar promotion", "F", &fWarnPromotion, NULL, NULL},
 {"warn-temporary", ' ', "Warn about sequence temporaries", "F", &fWarnTemporary, NULL, NULL},
 {"default-dist", ' ', "Change the default distribution", "S256", defaultDistribution, "CHPL_DEFAULT_DISTRIBUTION", NULL},
 {"no-scalar-replace-array-wrappers", ' ', "Generate explicit array wrappers", "F", &fDisableScalarReplaceArrayWrappers, "CHPL_DISABLE_SCALAR_REPLACE_ARRAY_WRAPPERS", NULL},
 {0}
};

static ArgumentState arg_state = {
  0, 0,
  "program", 
  "path",
  arg_desc
};

static void printLicenseAndExit(ArgumentState *arg_state, char *arg_unused) {
  fprintf(stderr,
#include "LICENSE"
          );
  clean_exit(0);
}

static void printCopyright(void) {
  fprintf(stderr,
#include "COPYRIGHT"
          );
}


static void printCopyrightAndExit(ArgumentState* arg_state, char* arg_unused) {
  printCopyright();
  clean_exit(0);
}

static void printVersion(ArgumentState* arg_state) {
  char ver[30];
  get_version(ver);
  fprintf(stderr, "%s Version %s\n", arg_state->program_name, ver);  
  printCopyright();
}

static void printVersionAndExit(ArgumentState* arg_state, char* arg_unused) {
  printVersion(arg_state);
  clean_exit(0);
}

static void printHelpAndExit(ArgumentState *arg_state, char *arg_unused) {
  printVersion(arg_state);
  usage(arg_state, arg_unused);
  clean_exit(0);
}

static void 
handleLibrary(ArgumentState* arg_state, char* arg_unused) {
  addLibInfo(stringcat("-l", libraryFilename));
}

static void 
handleLibPath(ArgumentState* arg_state, char* arg_unused) {
  addLibInfo(stringcat("-L", libraryFilename));
}

static void
compile_all(void) {
  initPrimitive();
  initPrimitiveTypes();
  testInputFiles(arg_state.nfile_arguments, arg_state.file_argument);
  runPasses();
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
  snprintf(chplhome, FILENAME_MAX, "%s", stringcat(*loc, "/../.."));
}


void runCompilerInGDB(int argc, char* argv[]) {
  char* gdbCommandFilename = createGDBFile(argc, argv);
  char* command = stringcat("gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", 0);

  clean_exit(status);
}

int
main(int argc, char *argv[]) {
  compute_program_name_loc(argv[0], &(arg_state.program_name),
                           &(arg_state.program_loc));
  process_args(&arg_state, argc, argv);
  startCatchingSignals();
  if (arg_state.nfile_arguments < 1)
    printHelpAndExit(&arg_state, NULL);
  if (rungdb)
    runCompilerInGDB(argc, argv);
  if (fdump_html || strcmp(log_flags, ""))
    init_logs();
  compile_all();
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
