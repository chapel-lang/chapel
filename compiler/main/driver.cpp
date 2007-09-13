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


static void setChapelDebug(ArgumentState* arg_state, char* arg_unused);
static void verifySaveCDir(ArgumentState* arg_state, char* arg_unused);
static void verifyIntSizes(ArgumentState* arg_state, char* arg_unused);
static void handleLibrary(ArgumentState* arg_state, char* arg_unused);
static void handleLibPath(ArgumentState* arg_state, char* arg_unused);
static void readConfigParam(ArgumentState* arg_state, char* arg_unused);

FILE* html_index_file = NULL;

int fdump_html = 0;
static char libraryFilename[FILENAME_MAX] = "";
static char log_flags[512] = "";
static bool rungdb = false;
bool no_codegen = false;
int debugParserLevel = 0;
bool developer = false;
bool ignore_errors = false;
bool ignore_warnings = false;
int trace_level = 0;
int fcg = 0;
bool fBaseline = false;
bool fNoCopyPropagation = false;
bool fNoScalarReplaceArrayWrappers = false;
bool fNoScalarReplacement = false;
bool fNoSingleLoopIteratorOpt = false;
bool fNoExpandIteratorsInlineOpt = false;
bool fNoLiveVariablesIteratorClassOpt = false;
bool fNoFlowAnalysis = false;
bool fNoBoundsChecks = false;
bool fNoInline = false;
bool fShortNames = false;
bool fSerial = false;
bool fLocal = false;
bool report_inlining = false;
char chplhome[FILENAME_MAX] = ".";
char chplmake[256] = "";
char fExplainCall[256] = "";
char fPrintStatistics[256] = "";
bool fPrintDispatch = false;
bool fWarnPromotion = false;
bool fNullTemps = false;
bool copyCollect = false;
int fNoStdIncs = 0;
int num_constants_per_variable = 1;
char defaultDistribution[256] = "SingleLocaleDistribution";
int instantiation_limit = 256;
//char configParamString[FILENAME_MAX] = "";
char mainModuleName[256] = "";

Map<const char*, const char*> configParamMap;
bool debugCCode = false;
bool optimizeCCode = false;

bool fEnableTimers = false;
Timer timer1;
Timer timer2;
Timer timer3;
Timer timer4;
Timer timer5;

static bool printCopyright = false;
static bool printHelp = false;
static bool printLicense = false;
static bool printVersion = false;
static bool testIntSizes = false;

static ArgumentDescription arg_desc[] = {
 {"", ' ', NULL, "Compilation Traces", NULL, NULL, NULL, NULL},
 {"print-commands", ' ', NULL, "Print system commands", "F", &printSystemCommands, "CHPL_PRINT_COMMANDS", NULL},
 {"print-passes", ' ', NULL, "Print compiler passes", "F", &printPasses, "CHPL_PRINT_PASSES", NULL},

 {"", ' ', NULL, "Code Size Statistics", NULL, NULL, NULL, NULL},
 {"count-tokens", ' ', NULL, "Count tokens", "F", &countTokens, "CHPL_COUNT_TOKENS", NULL},
 {"print-code-size", ' ', NULL, "Print code size statistics", "F", &printTokens, "CHPL_PRINT_TOKENS", NULL},

 {"", ' ', NULL, "Optimization Control", NULL, NULL, NULL, NULL},
 {"baseline", ' ', NULL, "Disable almost all optimizations", "F", &fBaseline, "CHPL_BASELINE", NULL},
 {"local", ' ', NULL, "Compile program for a single locale", "F", &fLocal, "CHPL_LOCAL", NULL},
 {"no-bounds-checks", ' ', NULL, "Disable bounds checking", "F", &fNoBoundsChecks, "CHPL_NO_BOUNDS_CHECKING", NULL},
 {"no-copy-propagation", ' ', NULL, "Disable copy propagation", "F", &fNoCopyPropagation, "CHPL_DISABLE_COPY_PROPAGATION", NULL},
 {"no-expand-iterators-inline-opt", ' ', NULL, "Disable the expansion of iterators inlined around loop bodies", "F", &fNoExpandIteratorsInlineOpt, "CHPL_DISABLE_EXPAND_ITERATORS_INLINE_OPT", NULL},
 {"no-flow-analysis", ' ', NULL, "Disable optimizations requiring flow analysis", "F", &fNoFlowAnalysis, "CHPL_NO_FLOW_ANALYSIS", NULL},
 {"no-inline", ' ', NULL, "Do not inline functions", "F", &fNoInline, NULL, NULL},
 {"no-single-loop-iterator-opt", ' ', NULL, "Disable the optimization of iterators composed of a single loop", "F", &fNoSingleLoopIteratorOpt, "CHPL_DISABLE_SINGLE_LOOP_ITERATOR_OPT", NULL},
 {"serial", ' ', NULL, "Serialize program", "F", &fSerial, "CHPL_SERIAL", NULL},

 {"", ' ', NULL, "Code Generation", NULL, NULL, NULL, NULL},
 {"cg-cpp-lines", ' ', NULL, "Generate #line annotations", "N", &printCppLineno, "CHPL_CG_CPP_LINES", NULL},
 {"debug", 'g', NULL, "Allow debugging of generated C code", "N", &debugCCode, "CHPL_DEBUG", setChapelDebug},
 {"optimize", 'O', NULL, "Optimize generated C code", "N", &optimizeCCode, "CHPL_OPTIMIZE", NULL},
 {"savec", ' ', "<directory>", "Save generated C code in directory", "P", saveCDir, "CHPL_SAVEC_DIR", verifySaveCDir},
 {"short-names", ' ', NULL, "Use short names", "F", &fShortNames, "CHPL_SHORT_NAMES", NULL},
 {"make", ' ', NULL, "Make utility for generated code", "S256", &chplmake, "CHPL_MAKE", NULL},

 {"", ' ', NULL, "Linker Control", NULL, NULL, NULL, NULL},
 {"ccflags", ' ', "<flags>", "Back-end C compiler flags", "S256", ccflags, "CHPL_CC_FLAGS", NULL},
 {"ldflags", ' ', "<flags>", "Back-end C linker flags", "S256", ldflags, "CHPL_LD_FLAGS", NULL},
 {"lib-linkage", 'l', "<library>", "C library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"lib-search-path", 'L', "<directory>", "C library search path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"output", 'o', "<filename>", "Name output executable", "P", executableFilename, "CHPL_EXE_NAME", NULL},

 {"", ' ', NULL, "Miscellaneous Flags", NULL, NULL, NULL, NULL},
 {"chplhome", ' ', "<directory>", "Over-ride $CHPL_HOME", "P", chplhome, "CHPL_HOME", NULL},
 {"devel", ' ', NULL, "Compile as developer", "N", &developer, "CHPL_DEVELOPER", NULL},
 {"explain-call", ' ', "<call>[:<module>][:<line>]", "Explain resolution of call", "S256", fExplainCall, NULL, NULL},
 {"instantiate-max", ' ', "<max>", "Limit number of instantiations", "I", &instantiation_limit, "CHPL_INSTANTIATION_LIMIT", NULL},
 {"main-module", ' ', NULL, "Specify module where main is located", "S256", mainModuleName, NULL, NULL},
 {"no-warnings", ' ', NULL, "Disable output of warnings", "F", &ignore_warnings, "CHPL_DISABLE_WARNINGS", NULL},
 {"set", 's', "<name>[=<value>]", "Set config param value", "S", NULL, NULL, readConfigParam},

 {"", ' ', NULL, "Compiler Information", NULL, NULL, NULL, NULL},
 {"copyright", ' ', NULL, "Show copyright", "F", &printCopyright, NULL},
 {"help", 'h', NULL, "Help (show this list)", "F", &printHelp, NULL},
 {"license", ' ', NULL, "Show license", "F", &printLicense, NULL},
 {"version", ' ', NULL, "Show version", "F", &printVersion, NULL},

 {"", ' ', NULL, "Developer Flags", NULL, NULL, NULL, NULL},
 {"", ' ', NULL, "Debug Output", NULL, NULL, NULL, NULL},
 {"ccwarnings", ' ', NULL, "Give warnings for generated code", "T", &ccwarnings, "CHPL_CC_WARNINGS", NULL},
 {"html", 't', NULL, "Dump IR in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"log", 'd', "[a|i|F|d|s]", "Specify debug logs", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"log-dir", ' ', "<path>", "Specify log directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"parser-debug", 'D', NULL, "Set parser debug level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"print-dispatch", ' ', NULL, "Print dynamic dispatch table", "F", &fPrintDispatch, NULL, NULL},
 {"print-statistics", ' ', "[n|k]", "Print AST statistics", "S256", fPrintStatistics, NULL, NULL},
 {"report-inlining", ' ', NULL, "Print inlined functions", "F", &report_inlining, NULL, NULL},

 {"", ' ', NULL, "Misc. Developer Flags", NULL, NULL, NULL, NULL},
 {"copy-collect", ' ', NULL, "Turn on copying garbage-collection", "F", &copyCollect, NULL, NULL},
 {"default-dist", ' ', "<distribution>", "Change the default distribution", "S256", defaultDistribution, "CHPL_DEFAULT_DISTRIBUTION", NULL},
 {"gdb", ' ', NULL, "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"ignore-errors", ' ', NULL, "Attempt to ignore errors", "F", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"no-codegen", ' ', NULL, "Suppress code generation", "F", &no_codegen, "CHPL_NO_CODEGEN", NULL},
 {"no-live-variables-iterator-class-opt", ' ', NULL, "Do not use live variable analysis for iterator class construction", "F", &fNoLiveVariablesIteratorClassOpt, "CHPL_DISABLE_LIVE_VARIABLE_ITERATOR_CLASS_OPT", NULL},
 {"no-scalar-replace-array-wrappers", ' ', NULL, "Generate explicit array wrappers", "F", &fNoScalarReplaceArrayWrappers, "CHPL_DISABLE_SCALAR_REPLACE_ARRAY_WRAPPERS", NULL},
 {"no-scalar-replacement", ' ', NULL, "Disable scalar replacement", "F", &fNoScalarReplacement, "CHPL_DISABLE_SCALAR_REPLACEMENT", NULL},
 {"nostdincs", ' ', NULL, "Don't use standard modules", "T", &fNoStdIncs, "CHPL_NOSTDINCS", NULL},
 {"null-temps", ' ', NULL, "Initialize nullable compiler temporaries to null", "F", &fNullTemps, "CHPL_NULL_TEMPS", NULL},
 {"test-int-sizes", ' ', NULL, "Test compiler's internal integer sizes", "F", &testIntSizes, "CHPL_TEST_INT_SIZES", verifyIntSizes},
 {"timers", ' ', NULL, "Enable general timers one to five", "F", &fEnableTimers, "CHPL_ENABLE_TIMERS", NULL},
 {"warn-promotion", ' ', NULL, "Warn about scalar promotion", "F", &fWarnPromotion, NULL, NULL},
 {0}
};


static ArgumentState arg_state = {
  0, 0,
  "program", 
  "path",
  arg_desc
};

static void setChapelDebug(ArgumentState* arg_state, char* arg_unused) {
  printCppLineno = true;
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
compute_program_name_loc(char* orig_argv0, const char** name, const char** loc) {
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
  const char* gdbCommandFilename = createGDBFile(argc, argv);
  char* command = stringcat("gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", 0);

  clean_exit(status);
}

static void readConfigParam(ArgumentState* arg_state, char* arg_unused) {
  // Expect arg_unused to be a string of either of these forms:
  // 1. name=value -- set the config param "name" to "value"
  // 2. name       -- set the boolean config param "name" to NOT("name")
  //                  if name is not type bool, set it to 0.

  char *name = stringcpy(arg_unused);
  char *value;
  value = strstr(name, "=");
  if (value) {
    *value = '\0';
    value++;
    if (value[0]) {
      // arg_unused was name=value
      configParamMap.put(canonicalize_string(name), value);
    } else {
      // arg_unused was name=  <blank>
      USR_FATAL("Missing config param value");
    }
  } else {
    // arg_unused was just name
    configParamMap.put(canonicalize_string(name), "");
  }
}


static void printStuff(void) {
  bool shouldExit = false;
  bool printedSomething = false;

  if (printVersion) {
    char ver[64];
    get_version(ver);
    fprintf(stdout, "%s Version %s\n", arg_state.program_name, ver);
    printCopyright = true;
    printedSomething = true;
    shouldExit = true;
  }
  if (printLicense) {
    fprintf(stdout,
#include "LICENSE"
            );
    printCopyright = false;
    shouldExit = true;
    printedSomething = true;
  }
  if (printCopyright) {
    fprintf(stdout,
#include "COPYRIGHT"
            );
    printedSomething = true;
  }
  if (printHelp || (!printedSomething && arg_state.nfile_arguments < 1)) {
    if (printedSomething) printf("\n");
    usage(&arg_state, (printHelp == false));
    shouldExit = true;
    printedSomething = true;
  }
  if (printedSomething && arg_state.nfile_arguments < 1) {
    shouldExit = true;
  }
  if (shouldExit) {
    clean_exit(0);
  }
}


static void verifySaveCDir(ArgumentState* arg, char* unused) {
  if (saveCDir[0] == '-') {
    USR_FATAL("--savec takes a directory name as its argument\n"
              "       (you specified '%s', assumed to be another flag)",
              saveCDir);
  }
}


static void verifyIntSizes(ArgumentState* arg, char* unused) {
  if (sizeof(int8) != 1 ||
      sizeof(int16) != 2 ||
      sizeof(int32) != 4 ||
      sizeof(int64) != 8 ||
      sizeof(uint8) != 1 ||
      sizeof(uint16) != 2 ||
      sizeof(uint32) != 4 ||
      sizeof(uint64) != 8) {
    INT_FATAL("compiler integer types not set up properly");
  }
}


int main(int argc, char *argv[]) {
  compute_program_name_loc(argv[0], &(arg_state.program_name),
                           &(arg_state.program_loc));
  process_args(&arg_state, argc, argv);
  startCatchingSignals();
  printStuff();
  if (rungdb)
    runCompilerInGDB(argc, argv);
  if (fdump_html || strcmp(log_flags, ""))
    init_logs();
  compile_all();
  if (fEnableTimers) {
    printf("timer 1: %8.3lf\n", timer1.elapsed());
    printf("timer 2: %8.3lf\n", timer2.elapsed());
    printf("timer 3: %8.3lf\n", timer3.elapsed());
    printf("timer 4: %8.3lf\n", timer4.elapsed());
    printf("timer 5: %8.3lf\n", timer5.elapsed());
  }
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
