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
static int pre_malloc = 0;
bool formalTemps = false;
bool no_codegen = false;
int debugParserLevel = 0;
bool developer = false;
bool ignore_errors = false;
int trace_level = 0;
int fcg = 0;
bool unoptimized = false;
bool no_scalar_replacement = true;
bool no_bounds_checking = false;
bool no_inline = false;
bool report_inlining = false;
char system_dir[FILENAME_MAX] = DEFAULT_SYSTEM_DIR;
char fPrintStatistics[256] = "";
int fnostdincs = 0;
int num_constants_per_variable = 1;
int instantiation_limit = 256;
bool parallelPass = true;
int scalar_promotion = 1;
int squelch_header_errors = 0;

static ArgumentDescription arg_desc[] = {
 {"nostdincs", ' ', "No Standard Includes", "T", &fnostdincs, "CHPL_NOSTDINCS", NULL},
 {"premalloc", 'm', "Pre-Malloc", "I", &pre_malloc, "CHPL_PRE_malloc", NULL},
 {"sysdir", 'S', "System Directory", "P", system_dir, "CHPL_SYSTEM_DIR", NULL},
 {"devel", ' ', "Developer Compile", "F", &developer, "CHPL_DEVELOPER", NULL},
 {"ignore-errors", ' ', "Attempt to Ignore Errors", "F", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"instantiation-limit", ' ', "Limit Number of Instantiations", "I", &instantiation_limit, "CHPL_INSTANTIATION_LIMIT", NULL},
 {"html", 't', "Write Program in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"log-dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'd', "Debug Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"cg-cpp-lines", ' ', "Generate #line Directives", "F", &printCppLineno, "CHPL_CG_CPP_LINES", NULL},
 {"cg-chpl-lineno", ' ', "Generate CHPL Input Line Numbers", "F", &printChplLineno, "CHPL_CG_CHPL_LINENO", NULL},
 {"lib-linkage", 'l', "Library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"lib-search-path", 'L', "Library Search Path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"output", 'o', "Name of Executable Output", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"savec", ' ', "Save Intermediate C Code", "P", saveCDir, "CHPL_SAVEC_DIR", NULL},
 {"ccflags", ' ', "Specify back-end C compiler flags", "S256", ccflags, "CHPL_CC_FLAGS", NULL},
 {"gdb", ' ', "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"unoptimized", ' ', "Generate naive code (lots of temps)", "F", &unoptimized, "CHPL_UNOPTIMIZED", NULL},
 {"no-bounds-checking", ' ', "Disable bounds checking", "F", &no_bounds_checking, "CHPL_NO_BOUNDS_CHECKING", NULL},
 {"no-inline", ' ', "Do Not Inline Functions" , "F", &no_inline, NULL, NULL},
 {"no-scalar-replacement", ' ', "**INVERTED** Do not scalar replace marked records (e.g. tuples)", "T", &no_scalar_replacement, NULL, NULL},
 {"report-inlining", ' ', "Print Inlined Functions", "F", &report_inlining, NULL, NULL},
 {"scalar-promotion", ' ', "Enable Scalar Promotion", "T", &scalar_promotion, "CHPL_SCALAR_PROMOTION", NULL},
 {"formal-temps", ' ', "Insert Temps for Formals", "F", &formalTemps, "CHPL_FORMAL_TEMPS", NULL},
 {"no-codegen", ' ', "Suppress Code Generation", "F", &no_codegen, "CHPL_NO_CODEGEN", NULL},
 {"parser-debug", 'D', "Parser Debug Level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"count-tokens", ' ', "Count Tokens", "F", &countTokens, "CHPL_COUNT_TOKENS", NULL},
 {"print-tokens", ' ', "Print Tokens", "F", &printTokens, "CHPL_PRINT_TOKENS", NULL},
 {"print-commands", ' ', "Print Subprocess Commands", "F", &printSystemCommands, 
  "CHPL_PRINT_COMMANDS", NULL},
 {"parallel", 'p', "toggle threaded code generation", "T", &parallelPass, "CHPL_PARALLELIZE", NULL},
 {"print-passes", ' ', "Print Passes", "F", &printPasses, "CHPL_PRINT_PASSES", NULL},
 {"print-statistics", ' ', "Print AST statistics", "S256", fPrintStatistics, NULL, NULL},
 {"no-header-errors", ' ', "Squelch Header Errors", "T", &squelch_header_errors, "CHPL_SQUELCH_HEADER_ERRORS", NULL},
 {"version", ' ', "Show Version", NULL, NULL, NULL, printVersionAndExit},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, printCopyrightAndExit},
 {"license", ' ', "Show License", NULL, NULL, NULL, printLicenseAndExit},
 {"help", 'h', "Help (show this list)", NULL, NULL, NULL, printHelpAndExit},
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
    (void)malloc(pre_malloc);
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
  init_system();
  compile_all();
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
