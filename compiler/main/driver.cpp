/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define EXTERN
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "driver.h"

#include "arg.h"
#include "chpl.h"
#include "commonFlags.h"
#include "config.h"
#include "countTokens.h"
#include "docsDriver.h"
#include "files.h"
#include "library.h"
#include "log.h"
#include "ModuleSymbol.h"
#include "misc.h"
#include "mysystem.h"
#include "parser.h"
#include "PhaseTracker.h"
#include "primitive.h"
#include "runpasses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "timer.h"
#include "version.h"

#include <inttypes.h>
#include <string>
#include <sstream>
#include <map>

std::map<std::string, const char*> envMap;

char CHPL_HOME[FILENAME_MAX+1] = "";

// These are more specific than CHPL_HOME, to work in
// settings where Chapel is installed.
char CHPL_RUNTIME_LIB[FILENAME_MAX+1] = "";
char CHPL_RUNTIME_INCL[FILENAME_MAX+1] = "";
char CHPL_THIRD_PARTY[FILENAME_MAX+1] = "";

const char* CHPL_HOST_PLATFORM = NULL;
const char* CHPL_HOST_ARCH = NULL;
const char* CHPL_HOST_COMPILER = NULL;
const char* CHPL_HOST_CPU = NULL;
const char* CHPL_TARGET_PLATFORM = NULL;
const char* CHPL_TARGET_ARCH = NULL;
const char* CHPL_TARGET_CPU = NULL;
const char* CHPL_RUNTIME_CPU = NULL;
const char* CHPL_TARGET_BACKEND_CPU = NULL;
const char* CHPL_TARGET_CPU_FLAG = NULL;
const char* CHPL_TARGET_COMPILER = NULL;
const char* CHPL_ORIG_TARGET_COMPILER = NULL;
const char* CHPL_LOCALE_MODEL = NULL;
const char* CHPL_COMM = NULL;
const char* CHPL_COMM_SUBSTRATE = NULL;
const char* CHPL_GASNET_SEGMENT = NULL;
const char* CHPL_TASKS = NULL;
const char* CHPL_LAUNCHER = NULL;
const char* CHPL_TIMERS = NULL;
const char* CHPL_MEM = NULL;
const char* CHPL_MAKE = NULL;
const char* CHPL_ATOMICS = NULL;
const char* CHPL_NETWORK_ATOMICS = NULL;
const char* CHPL_GMP = NULL;
const char* CHPL_HWLOC = NULL;
const char* CHPL_REGEXP = NULL;
const char* CHPL_LLVM = NULL;
const char* CHPL_AUX_FILESYS = NULL;
const char* CHPL_UNWIND = NULL;
const char* CHPL_LIB_PIC = NULL;

const char* CHPL_RUNTIME_SUBDIR = NULL;
const char* CHPL_LAUNCHER_SUBDIR = NULL;
const char* CHPL_LLVM_UNIQ_CFG_PATH = NULL;

static char libraryFilename[FILENAME_MAX] = "";
static char incFilename[FILENAME_MAX] = "";
static bool fBaseline = false;

// TODO: Should --library automatically generate all supported
// interfaces (C, Fortran, Python)? Then there'd be no need to
// specify each one separately.
//
bool fLibraryCompile = false;
bool fLibraryFortran = false;
bool fLibraryMakefile = false;
bool fLibraryPython = false;
bool fMultiLocaleInterop = false;
bool fMultiLocaleLibraryDebug = false;

bool no_codegen = false;
int  debugParserLevel = 0;
bool fVerify = false;
bool ignore_errors = false;
bool ignore_user_errors = false;
bool ignore_errors_for_pass = false;
bool ignore_warnings = false;
int  fcg = 0;
bool fCacheRemote = false;
bool fFastFlag = false;
bool fUseNoinit = true;
bool fNoCopyPropagation = false;
bool fNoDeadCodeElimination = false;
bool fNoScalarReplacement = false;
bool fNoTupleCopyOpt = false;
bool fNoRemoteValueForwarding = false;
bool fNoInferConstRefs = false;
bool fNoRemoteSerialization = false;
bool fNoRemoveCopyCalls = false;
bool fNoOptimizeRangeIteration = false;
bool fNoOptimizeLoopIterators = false;
bool fNoVectorize = false; // adjusted in postVectorize
static bool fYesVectorize = false;
bool fForceVectorize = false;
bool fNoGlobalConstOpt = false;
bool fNoFastFollowers = false;
bool fNoInlineIterators = false;
bool fNoLiveAnalysis = false;
bool fNoBoundsChecks = false;
bool fNoDivZeroChecks = false;
bool fNoFormalDomainChecks = false;
bool fNoLocalChecks = false;
bool fNoNilChecks = false;
bool fLegacyClasses = false;
bool fIgnoreNilabilityErrors = false;
bool fOverloadSetsChecks = true;
bool fNoStackChecks = false;
bool fNoInferLocalFields = false;
bool fReplaceArrayAccessesWithRefTemps = false;
bool fUserSetStackChecks = false;
bool fNoCastChecks = false;
bool fMungeUserIdents = true;
bool fEnableTaskTracking = false;

bool  printPasses     = false;
FILE* printPassesFile = NULL;

// flag for llvmWideOpt
bool fLLVMWideOpt = false;

bool fWarnConstLoops = true;
bool fWarnUnstable = false;

// Enable all extra special warnings
static bool fNoWarnSpecial = true;
static bool fNoWarnDomainLiteral = true;
static bool fNoWarnTupleIteration = true;

bool fNoLoopInvariantCodeMotion = false;
bool fNoInterproceduralAliasAnalysis = true;
bool fNoChecks = false;
bool fNoInline = false;
bool fNoPrivatization = false;
bool fNoOptimizeOnClauses = false;
bool fNoRemoveEmptyRecords = true;
bool fRemoveUnreachableBlocks = true;
bool fMinimalModules = false;
bool fIncrementalCompilation = false;
bool fNoOptimizeForallUnordered = false;

int optimize_on_clause_limit = 20;
int scalar_replace_limit = 8;
int inline_iter_yield_limit = 10;
int tuple_copy_limit = scalar_replace_limit;
bool fGenIDS = false;
int fLinkStyle = LS_DEFAULT; // use backend compiler's default
bool fUserSetLocal = false;
bool fLocal;   // initialized in postLocal()
bool fIgnoreLocalClasses = false;
bool fNoLifetimeChecking = false;
bool fNoSplitInit = false;
bool fNoEarlyDeinit = false;
bool fNoCopyElision = false;
bool fCompileTimeNilChecking = true;
bool fOverrideChecking = true;
bool fieeefloat = false;
int ffloatOpt = 0; // 0 -> backend default; -1 -> strict; 1 -> opt
bool report_inlining = false;
char fExplainCall[256] = "";
int explainCallID = -1;
int breakOnResolveID = -1;
bool fDenormalize = true;
char fExplainInstantiation[256] = "";
bool fExplainVerbose = false;
bool fParseOnly = false;
bool fPrintCallGraph = false;
bool fPrintAllCandidates = false;
bool fPrintCallStackOnError = false;
bool fPrintIDonError = false;
bool fPrintModuleResolution = false;
bool fPrintEmittedCodeSize = false;
char fPrintStatistics[256] = "";
bool fPrintDispatch = false;
bool fPrintUnusedFns = false;
bool fPrintUnusedInternalFns = false;
bool fReportAliases = false;
bool fReportBlocking = false;
bool fReportOptimizedLoopIterators = false;
bool fReportInlinedIterators = false;
bool fReportVectorizedLoops = false;
bool fReportOptimizedOn = false;
bool fReportOptimizeForallUnordered = false;
bool fReportPromotion = false;
bool fReportScalarReplace = false;
bool fReportDeadBlocks = false;
bool fReportDeadModules = false;
bool fPermitUnhandledModuleErrors = false;
#ifdef HAVE_LLVM_RV
bool fRegionVectorizer = true;
#else
bool fRegionVectorizer = false;
#endif
bool printCppLineno = false;
bool userSetCppLineno = false;
int num_constants_per_variable = 1;
char defaultDist[256] = "DefaultDist";
int instantiation_limit = 256;
bool printSearchDirs = false;
bool printModuleFiles = false;
bool llvmCodegen = false;
#ifdef HAVE_LLVM
bool externC = true;
#else
bool externC = false;
#endif
char breakOnCodegenCname[256] = "";
int breakOnCodegenID = 0;

bool debugCCode = false;
bool optimizeCCode = false;
bool specializeCCode = false;

bool fNoMemoryFrees = false;
int numGlobalsOnHeap = 0;
bool preserveInlinedLineNumbers = false;

char stopAfterPass[128];

const char* compileCommand = NULL;
char compileVersion[64];

std::string llvmFlags;

static
bool fPrintChplSettings = false;

/* Note -- LLVM provides a way to get the path to the executable...
// This function isn't referenced outside its translation unit, but it
// can't use the "static" keyword because its address is used for
// GetMainExecutable (since some platforms don't support taking the
// address of main, and some platforms can't implement GetMainExecutable
// without being given the address of a function in the main executable).
llvm::sys::Path GetExecutablePath(const char *Argv0) {
  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *MainAddr = (void*) (intptr_t) GetExecutablePath;
  return llvm::sys::Path::GetMainExecutable(Argv0, MainAddr);
}
*/

static bool isMaybeChplHome(const char* path)
{
  bool  ret  = false;
  char* real = dirHasFile(path, "util/chplenv");

  if (real)
    ret = true;

  free(real);

  return ret;
}

static void setChplHomeDerivedVars() {
  int rc;
  rc = snprintf(CHPL_RUNTIME_LIB, FILENAME_MAX, "%s/%s",
                CHPL_HOME, "lib");
  if ( rc >= FILENAME_MAX ) USR_FATAL("CHPL_HOME pathname too long");
  rc = snprintf(CHPL_RUNTIME_INCL, FILENAME_MAX, "%s/%s",
                CHPL_HOME, "runtime/include");
  if ( rc >= FILENAME_MAX ) USR_FATAL("CHPL_HOME pathname too long");
  rc = snprintf(CHPL_THIRD_PARTY, FILENAME_MAX, "%s/%s",
                CHPL_HOME, "third-party");
  if ( rc >= FILENAME_MAX ) USR_FATAL("CHPL_HOME pathname too long");
}

static void saveChplHomeDerivedInEnv() {
  int rc;
  envMap["CHPL_RUNTIME_LIB"] = strdup(CHPL_RUNTIME_LIB);
  rc = setenv("CHPL_RUNTIME_LIB", CHPL_RUNTIME_LIB, 1);
  if( rc ) USR_FATAL("Could not setenv CHPL_RUNTIME_LIB");
  envMap["CHPL_RUNTIME_INCL"] = strdup(CHPL_RUNTIME_INCL);
  rc = setenv("CHPL_RUNTIME_INCL", CHPL_RUNTIME_INCL, 1);
  if( rc ) USR_FATAL("Could not setenv CHPL_RUNTIME_INCL");
  envMap["CHPL_THIRD_PARTY"] = strdup(CHPL_THIRD_PARTY);
  rc = setenv("CHPL_THIRD_PARTY", CHPL_THIRD_PARTY, 1);
  if( rc ) USR_FATAL("Could not setenv CHPL_THIRD_PARTY");
}

static void setupChplHome(const char* argv0) {
  const char* chpl_home = getenv("CHPL_HOME");
  char*       guess     = NULL;
  bool        installed = false;
  char        majMinorVers[64];

  // Get major.minor version string (used below)
  get_major_minor_version(majMinorVers);

  // Get the executable path.
  guess = findProgramPath(argv0);

  if (guess) {
    // Determine CHPL_HOME based on the exe path.
    // Determined exe path, but don't have a env var set
    // Look for ../../../util/chplenv
    // Remove the /bin/some-platform/chpl part
    // from the path.
    if( guess[0] ) {
      int j = strlen(guess) - 5; // /bin and '\0'
      for( ; j >= 0; j-- ) {
        if( guess[j] == '/' &&
            guess[j+1] == 'b' &&
            guess[j+2] == 'i' &&
            guess[j+3] == 'n' ) {
          guess[j] = '\0';
          break;
        }
      }
    }

    if( isMaybeChplHome(guess) ) {
      // OK!
    } else {
      // Maybe we are in e.g. /usr/bin.
      free(guess);
      guess = NULL;
    }
  }

  if( chpl_home ) {
    if( strlen(chpl_home) > FILENAME_MAX )
      USR_FATAL("$CHPL_HOME=%s path too long", chpl_home);

    if( guess == NULL ) {
      // Could not find exe path, but have a env var set
      strncpy(CHPL_HOME, chpl_home, FILENAME_MAX);
    } else {
      // We have env var and found exe path.
      // Check that they match and emit a warning if not.

      if( ! isSameFile(chpl_home, guess) ) {
        // Not the same. Emit warning.
        USR_WARN("$CHPL_HOME=%s mismatched with executable home=%s",
                 chpl_home, guess);
      }
      // Since we have an enviro var, always use that.
      strncpy(CHPL_HOME, chpl_home, FILENAME_MAX);
    }
  } else {

    // Check in a default location too
    if( guess == NULL ) {
      char TEST_HOME[FILENAME_MAX+1] = "";

      // Check for Chapel libraries at installed prefix
      // e.g. /usr/share/chapel/<vers>
      int rc;
      rc = snprintf(TEST_HOME, FILENAME_MAX, "%s/%s/%s",
                  get_configured_prefix(), // e.g. /usr
                  "share/chapel",
                  majMinorVers);
      if ( rc >= FILENAME_MAX ) USR_FATAL("Installed pathname too long");

      if( isMaybeChplHome(TEST_HOME) ) {
        guess = strdup(TEST_HOME);

        installed = true;
      }
    }

    if( guess == NULL ) {
      // Could not find enviro var, and could not
      // guess at exe's path name.
      USR_FATAL("$CHPL_HOME must be set to run chpl");
    } else {
      int rc;

      if( strlen(guess) > FILENAME_MAX )
        USR_FATAL("chpl guessed home %s too long", guess);

      // Determined exe path, but don't have a env var set
      strncpy(CHPL_HOME, guess, FILENAME_MAX);
      // Also need to setenv in this case.
      rc = setenv("CHPL_HOME", guess, 0);
      if( rc ) USR_FATAL("Could not setenv CHPL_HOME");
    }
  }

  // Check that the resulting path is a Chapel distribution.
  if( ! isMaybeChplHome(CHPL_HOME) ) {
    // Bad enviro var.
    USR_WARN("CHPL_HOME=%s is not a Chapel distribution", CHPL_HOME);
  }

  if( guess )
    free(guess);



  if( installed ) {
    int rc;
    // E.g. /usr/lib/chapel/1.16/runtime/lib
    rc = snprintf(CHPL_RUNTIME_LIB, FILENAME_MAX, "%s/%s/%s/%s",
                  get_configured_prefix(), // e.g. /usr
                  "/lib/chapel",
                  majMinorVers,
                  "runtime/lib");
    if ( rc >= FILENAME_MAX ) USR_FATAL("Installed pathname too long");
    rc = snprintf(CHPL_RUNTIME_INCL, FILENAME_MAX, "%s/%s/%s/%s",
                  get_configured_prefix(), // e.g. /usr
                  "/lib/chapel",
                  majMinorVers,
                  "runtime/include");
    if ( rc >= FILENAME_MAX ) USR_FATAL("Installed pathname too long");
    rc = snprintf(CHPL_THIRD_PARTY, FILENAME_MAX, "%s/%s/%s/%s",
                  get_configured_prefix(), // e.g. /usr
                  "/lib/chapel",
                  majMinorVers,
                  "third-party");
    if ( rc >= FILENAME_MAX ) USR_FATAL("Installed pathname too long");

  } else {
    setChplHomeDerivedVars();
  }

  // and setenv the derived enviro vars for use by called scripts/Makefiles
  {
    int rc;
    saveChplHomeDerivedInEnv();

    if (installed) {
      char CHPL_CONFIG[FILENAME_MAX+1] = "";
      // Set an extra default CHPL_CONFIG directory
      rc = snprintf(CHPL_CONFIG, FILENAME_MAX, "%s/%s/%s",
                    get_configured_prefix(), // e.g. /usr
                    "/lib/chapel",
                    majMinorVers);
      if ( rc >= FILENAME_MAX ) USR_FATAL("Installed pathname too long");

      // Don't overwrite CHPL_CONFIG so that a user-specified
      // one would be left alone.
      rc = setenv("CHPL_CONFIG", CHPL_CONFIG, 0);
      if( rc ) USR_FATAL("Could not setenv CHPL_CONFIG");
    }
  }
}

// NOTE: We are leaking memory here by dropping astr() results on the ground.
static void recordCodeGenStrings(int argc, char* argv[]) {
  compileCommand = astr("chpl ");
  // WARNING: This does not handle arbitrary sequences of escaped characters
  //  in string arguments
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    // Handle " and \" in strings
    while (char *dq = strchr(arg, '"')) {
      char targ[strlen(argv[i])+4];
      memcpy(targ, arg, dq-arg);
      if ((dq==argv[i]) || ((dq!=argv[i]) && (*(dq-1)!='\\'))) {
        targ[dq-arg] = '\\';
        targ[dq-arg+1] = '"';
        targ[dq-arg+2] = '\0';
      } else {
        targ[dq-arg] = '"';
        targ[dq-arg+1] = '\0';
      }
      arg = dq+1;
      compileCommand = astr(compileCommand, targ);
      if (arg == NULL) break;
    }
    if (arg)
      compileCommand = astr(compileCommand, arg, " ");
  }
  get_version(compileVersion);
}

static void setHome(const ArgumentDescription* desc, const char* arg) {
  // Wipe previous CHPL_HOME when comp flag is given
  CHPL_HOME[0] = '\0';

  // Copy arg into CHPL_HOME
  size_t arglen = strlen(arg) + 1; // room for \0
  if (arglen <= sizeof(CHPL_HOME)) {
    memcpy(CHPL_HOME, arg, arglen);
    // Update envMap
    envMap["CHPL_HOME"] = CHPL_HOME;
  } else {
    USR_FATAL("CHPL_HOME argument too long");
  }

  setChplHomeDerivedVars();
  saveChplHomeDerivedInEnv();
}

static void setEnv(const ArgumentDescription* desc, const char* arg) {
    // Copy desc->env because it is 'const char *'
    std::string env = std::string(desc->env);
    // Cut off underscore prefix so we are left with variable name
    env.erase(0, 1);
    envMap[env] = strdup(arg);
}

static void setDynamicLink(const ArgumentDescription* desc, const char* arg_unused) {
  fLinkStyle = LS_DYNAMIC;
}

static void setChapelDebug(const ArgumentDescription* desc, const char* arg_unused) {
  printCppLineno = true;
}

static void setPrintIr(const ArgumentDescription* desc, const char* arg) {
  if (llvmPrintIrStageNum == llvmStageNum::NOPRINT)
    llvmPrintIrStageNum = llvmStageNum::BASIC;

  std::vector<std::string> fNames;
  splitString(std::string(arg), fNames, ",");
  for (std::size_t i = 0; i < fNames.size(); ++i) {
    addNameToPrintLlvmIr(fNames[i].c_str());
  }
}

static void verifyStageAndSetStageNum(const ArgumentDescription* desc,
                                      const char* arg)
{
  llvmStageNum_t stageNum = llvmStageNumFromLlvmStageName(arg);
  if(stageNum == llvmStageNum::NOPRINT)
    USR_FATAL("Unknown llvm-print-ir-stage argument");

  llvmPrintIrStageNum = stageNum;
}

static void warnUponLegacyClasses(const ArgumentDescription* desc,
                                  const char* arg)
{
  USR_WARN("'--legacy-classes' option has been deprecated"
           " and will be removed in the next Chapel release;"
           " it no longer affects compilation");
}

// In order to handle accumulating ccflags arguments, the argument
// processing calls this function. This function appends the flags
// to the ccflags variable, so that multiple --ccflags arguments
// all end up together in the ccflags variable (and will end up
// being passed to the backend C compiler).
static void setCCFlags(const ArgumentDescription* desc, const char* arg) {
  // Append arg to the end of ccflags.

  // add a space if there are already arguments here
  if( ccflags.length() > 0 )
    ccflags += ' ';

  ccflags += arg;
}

// similar to setCCFlags
static void setLDFlags(const ArgumentDescription* desc, const char* arg) {
  // Append arg to the end of ldflags.

  // add a space if there are already arguments here
  if( ldflags.length() > 0 )
    ldflags += ' ';

  ldflags += arg;
}

// similar to setCCFlags
static void setLLVMFlags(const ArgumentDescription* desc, const char* arg) {
  // Append arg to the end of llvmFlags.

  // add a space if there are already arguments here
  if( llvmFlags.length() > 0 )
    llvmFlags += ' ';

  llvmFlags += arg;
}


static void handleLibrary(const ArgumentDescription* desc, const char* arg_unused) {
 addLibFile(libraryFilename);
}

static void handleLibPath(const ArgumentDescription* desc, const char* arg_unused) {
  addLibPath(libraryFilename);
}

static void handleIncDir(const ArgumentDescription* desc, const char* arg_unused) {
  addIncInfo(incFilename);
}

static void runCompilerInGDB(int argc, char* argv[]) {
  const char* gdbCommandFilename = createDebuggerFile("gdb", argc, argv);
  const char* command = astr("gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", false);

  clean_exit(status);
}


static void runCompilerInLLDB(int argc, char* argv[]) {
  const char* lldbCommandFilename = createDebuggerFile("lldb", argc, argv);
  const char* command = astr("lldb -s ", lldbCommandFilename, " ", argv[0]);
  int status = mysystem(command, "running lldb", false);

  clean_exit(status);
}


static void readConfig(const ArgumentDescription* desc, const char* arg_unused) {
  // Expect arg_unused to be a string of either of these forms:
  // 1. name=value -- set the config "name" to "value"
  // 2. name       -- set the config "name" to 'true'

  char *name = strdup(arg_unused);
  char *value;
  value = strstr(name, "=");
  if (value) {
    *value = '\0';
    value++;
    if (value[0]) {
      // arg_unused was name=value
      parseCmdLineConfig(name, value);
    } else {
      // arg_unused was name=  <blank>
      USR_FATAL("Missing config value");
    }
  } else {
    // arg_unused was just name
    parseCmdLineConfig(name, "");
  }
}

static void addModulePath(const ArgumentDescription* desc, const char* newpath) {
  addFlagModulePath(newpath);
}

static void noteCppLinesSet(const ArgumentDescription* desc, const char* unused) {
  userSetCppLineno = true;
}

static void verifySaveCDir(const ArgumentDescription* desc, const char* unused) {
  if (saveCDir[0] == '-') {
    USR_FATAL("--savec takes a directory name as its argument\n"
              "       (you specified '%s', assumed to be another flag)",
              saveCDir);
  }
}

static void setLibmode(const ArgumentDescription* desc, const char* unused);

static void verifySaveLibDir(const ArgumentDescription* desc, const char* unused) {
  if (libDir[0] == '-') {
    USR_FATAL("--library-dir takes a directory name as its argument\n"
              "       (you specified '%s', assumed to be another flag)",
              libDir);
  }
  setLibmode(desc, unused);
}

static void setVectorize(const ArgumentDescription* desc, const char* unused)
{
  if (fNoVectorize)
    fYesVectorize = false;
  else
    fYesVectorize = true;
}

static void turnOffChecks(const ArgumentDescription* desc, const char* unused) {
  fNoNilChecks    = true;
  fNoBoundsChecks = true;
  fNoFormalDomainChecks = true;
  fNoLocalChecks  = true;
  fNoStackChecks  = true;
  fNoCastChecks = true;
  fNoDivZeroChecks = true;
}

static void setFastFlag(const ArgumentDescription* desc, const char* unused) {
  //
  // Enable all compiler optimizations, disable all runtime checks
  //
  fBaseline = false;
  // don't set fieeefloat since it can change program behavior.
  // instead, we rely on the backend C compiler to choose
  // an appropriate level of optimization.
  fNoCopyPropagation = false;
  fNoDeadCodeElimination = false;
  fNoFastFollowers = false;
  fNoLoopInvariantCodeMotion= false;
  fNoInterproceduralAliasAnalysis = false;
  fNoInline = false;
  fNoInlineIterators = false;
  fNoOptimizeRangeIteration = false;
  fNoOptimizeLoopIterators = false;
  fNoLiveAnalysis = false;
  fNoInferConstRefs = false;
  fNoRemoteValueForwarding = false;
  fNoRemoteSerialization = false;
  fNoRemoveCopyCalls = false;
  fNoScalarReplacement = false;
  fNoTupleCopyOpt = false;
  fNoPrivatization = false;
  fNoChecks = true;
  fNoInferLocalFields = false;
  fIgnoreLocalClasses = false;
  fNoOptimizeOnClauses = false;
  //fReplaceArrayAccessesWithRefTemps = true; // don't tie this to --fast yet
  fNoOptimizeForallUnordered = false;
  optimizeCCode = true;
  specializeCCode = true;
  turnOffChecks(desc, unused);
}

static void setFloatOptFlag(const ArgumentDescription* desc, const char* unused) {
  // It would be nicer if arg.cpp could handle
  // 3-value variables like this (set to false, set to true, not set)
  // But if this is the only such case, having a set function is an OK plan.

  // ffloatOpt defaults to 0 -> backend default
  if( fieeefloat ) {
    // IEEE strict
    ffloatOpt = -1;
  } else {
    // lax IEEE, optimize
    ffloatOpt = 1;
  }
}

static void setBaselineFlag(const ArgumentDescription* desc, const char* unused) {
  //
  // disable all chapel compiler optimizations
  //
  fBaseline = true;                   // --baseline

  fNoCopyPropagation = true;          // --no-copy-propagation
  fNoDeadCodeElimination = true;      // --no-dead-code-elimination
  fNoFastFollowers = true;            // --no-fast-followers
  fNoLoopInvariantCodeMotion = true;  // --no-loop-invariant-code-motion
                                      // --no-interprocedural-alias-analysis
  fNoInterproceduralAliasAnalysis = true;
  fNoInline = true;                   // --no-inline
  fNoInlineIterators = true;          // --no-inline-iterators
  fNoLiveAnalysis = true;             // --no-live-analysis
  fNoOptimizeRangeIteration = true;   // --no-optimize-range-iteration
  fNoOptimizeLoopIterators = true;    // --no-optimize-loop-iterators
  fNoVectorize = true;                // --no-vectorize
  fNoInferConstRefs = true;           // --no-infer-const-refs
  fNoRemoteValueForwarding = true;    // --no-remote-value-forwarding
  fNoRemoteSerialization = true;      // --no-remote-serialization
  fNoRemoveCopyCalls = true;          // --no-remove-copy-calls
  fNoScalarReplacement = true;        // --no-scalar-replacement
  fNoTupleCopyOpt = true;             // --no-tuple-copy-opt
  fNoPrivatization = true;            // --no-privatization
  fNoOptimizeOnClauses = true;        // --no-optimize-on-clauses
  fIgnoreLocalClasses = true;         // --ignore-local-classes
  fNoInferLocalFields = true;         // --no-infer-local-fields
  //fReplaceArrayAccessesWithRefTemps = false; // don't tie this to --baseline yet
  fDenormalize = false;               // --no-denormalize
  fNoOptimizeForallUnordered = true;  // --no-optimize-forall-unordered-ops
}

static void setCacheEnable(const ArgumentDescription* desc, const char* unused) {
  const char *val = fCacheRemote ? "true" : "false";
  parseCmdLineConfig("CHPL_CACHE_REMOTE", val);
}

static void setHtmlUser(const ArgumentDescription* desc, const char* unused) {
  fdump_html = true;
  fdump_html_include_system_modules = false;
}

static void setWarnTupleIteration(const ArgumentDescription* desc, const char* unused) {
  const char *val = fNoWarnTupleIteration ? "false" : "true";
  parseCmdLineConfig("CHPL_WARN_TUPLE_ITERATION", astr("\"", val, "\""));
}

static void setWarnDomainLiteral(const ArgumentDescription* desc, const char* unused) {
  const char *val = fNoWarnDomainLiteral ? "false" : "true";
  parseCmdLineConfig("CHPL_WARN_DOMAIN_LITERAL", astr("\"", val, "\""));
}

static void setWarnSpecial(const ArgumentDescription* desc, const char* unused) {
  fNoWarnSpecial = false;

  fNoWarnDomainLiteral = false;
  setWarnDomainLiteral(desc, unused);

  fNoWarnTupleIteration = false;
  setWarnTupleIteration(desc, unused);
}

static void setLogDir(const ArgumentDescription* desc, const char* arg) {
  fLogDir = true;
}

static void setLogPass(const ArgumentDescription* desc, const char* arg) {
  logSelectPass(arg);
}

static void setPrintPassesFile(const ArgumentDescription* desc, const char* fileName) {
  printPassesFile = fopen(fileName, "w");

  if (printPassesFile == NULL) {
    USR_WARN("Error opening printPassesFile: %s.", fileName);
  }
}

static void setLocal (const ArgumentDescription* desc, const char* unused) {
  // Used in postLocal() to set fLocal if user threw flag
  fUserSetLocal = true;
}

static void setStackChecks (const ArgumentDescription* desc, const char* unused) {
  // Used in postStackChecks() to set fNoStackChecks if user threw flag
  fUserSetStackChecks= true;
}

static void setLibmode(const ArgumentDescription* desc, const char* unused) {
  fLibraryCompile = true;
}

static void setFortranAndLibmode(const ArgumentDescription* desc,
                                 const char* unused) {
  setLibmode(desc, unused);
  fLibraryFortran = true;
}

static void setPythonAndLibmode(const ArgumentDescription* desc,
                                const char* unused) {
  setLibmode(desc, unused);
  fLibraryPython = true;
}

/*
Flag types:

  I = int
  P = path
  S = string
  D = double
  f = set to false
  F = set to true
  + = increment
  T = toggle
  L = int64 (long)
  N = --no-... flag, --no version sets to false
  n = --no-... flag, --no version sets to true

Record components:
 {"long option" (or "" for separators), 'short option', "description of option argument(s), if any", "option description", "option type", &affectedVariable, "environment variable name", setter_function},
*/

// The setEnv args use _ variable prefix (_CHPL_HOME) to ensure that setEnv is
// only called when a flag is passed - otherwise arg functions are  called if
// their environment variable is set

static ArgumentDescription arg_desc[] = {
 {"", ' ', NULL, "Module Processing Options", NULL, NULL, NULL, NULL},
 {"count-tokens", ' ', NULL, "[Don't] count tokens in main modules", "N", &countTokens, "CHPL_COUNT_TOKENS", NULL},
 {"main-module", ' ', "<module>", "Specify entry point module", "S256", NULL, NULL, ModuleSymbol::mainModuleNameSet },
 {"module-dir", 'M', "<directory>", "Add directory to module search path", "P", NULL, NULL, addModulePath},
 {"print-code-size", ' ', NULL, "[Don't] print code size of main modules", "N", &printTokens, "CHPL_PRINT_TOKENS", NULL},
 {"print-module-files", ' ', NULL, "Print module file locations", "F", &printModuleFiles, NULL, NULL},
 {"print-search-dirs", ' ', NULL, "[Don't] print module search path", "N", &printSearchDirs, "CHPL_PRINT_SEARCH_DIRS", NULL},

 {"", ' ', NULL, "Warning and Language Control Options", NULL, NULL, NULL, NULL},
 {"permit-unhandled-module-errors", ' ', NULL, "Permit unhandled errors in explicit modules; such errors halt at runtime", "N", &fPermitUnhandledModuleErrors, "CHPL_PERMIT_UNHANDLED_MODULE_ERRORS", NULL},
 {"warn-unstable", ' ', NULL, "Enable [disable] warnings for uses of language features that are in flux", "N", &fWarnUnstable, "CHPL_WARN_UNSTABLE", NULL},
 {"warnings", ' ', NULL, "Enable [disable] output of warnings", "n", &ignore_warnings, "CHPL_DISABLE_WARNINGS", NULL},

 {"", ' ', NULL, "Parallelism Control Options", NULL, NULL, NULL, NULL},
 {"local", ' ', NULL, "Target one [many] locale[s]", "N", &fLocal, "CHPL_LOCAL", setLocal},

 {"", ' ', NULL, "Optimization Control Options", NULL, NULL, NULL, NULL},
 {"baseline", ' ', NULL, "Disable all Chapel optimizations", "F", &fBaseline, "CHPL_BASELINE", setBaselineFlag},
 {"cache-remote", ' ', NULL, "[Don't] enable cache for remote data", "N", &fCacheRemote, "CHPL_CACHE_REMOTE", setCacheEnable},
 {"copy-propagation", ' ', NULL, "Enable [disable] copy propagation", "n", &fNoCopyPropagation, "CHPL_DISABLE_COPY_PROPAGATION", NULL},
 {"dead-code-elimination", ' ', NULL, "Enable [disable] dead code elimination", "n", &fNoDeadCodeElimination, "CHPL_DISABLE_DEAD_CODE_ELIMINATION", NULL},
 {"fast", ' ', NULL, "Disable checks; optimize/specialize code", "F", &fFastFlag, "CHPL_FAST", setFastFlag},
 {"fast-followers", ' ', NULL, "Enable [disable] fast followers", "n", &fNoFastFollowers, "CHPL_DISABLE_FAST_FOLLOWERS", NULL},
 {"ieee-float", ' ', NULL, "Generate code that is strict [lax] with respect to IEEE compliance", "N", &fieeefloat, "CHPL_IEEE_FLOAT", setFloatOptFlag},
 {"ignore-local-classes", ' ', NULL, "Disable [enable] local classes", "N", &fIgnoreLocalClasses, NULL, NULL},
 {"inline", ' ', NULL, "Enable [disable] function inlining", "n", &fNoInline, NULL, NULL},
 {"inline-iterators", ' ', NULL, "Enable [disable] iterator inlining", "n", &fNoInlineIterators, "CHPL_DISABLE_INLINE_ITERATORS", NULL},
 {"inline-iterators-yield-limit", ' ', "<limit>", "Limit number of yields permitted in inlined iterators", "I", &inline_iter_yield_limit, "CHPL_INLINE_ITER_YIELD_LIMIT", NULL},
 {"live-analysis", ' ', NULL, "Enable [disable] live variable analysis", "n", &fNoLiveAnalysis, "CHPL_DISABLE_LIVE_ANALYSIS", NULL},
 {"loop-invariant-code-motion", ' ', NULL, "Enable [disable] loop invariant code motion", "n", &fNoLoopInvariantCodeMotion, NULL, NULL},
 {"optimize-forall-unordered-ops", ' ', NULL, "Enable [disable] optimization of foralls to unordered operations", "n", &fNoOptimizeForallUnordered, "CHPL_DISABLE_OPTIMIZE_FORALL_UNORDERED_OPS", NULL},
 {"optimize-range-iteration", ' ', NULL, "Enable [disable] optimization of iteration over anonymous ranges", "n", &fNoOptimizeRangeIteration, "CHPL_DISABLE_OPTIMIZE_RANGE_ITERATION", NULL},
 {"optimize-loop-iterators", ' ', NULL, "Enable [disable] optimization of iterators composed of a single loop", "n", &fNoOptimizeLoopIterators, "CHPL_DISABLE_OPTIMIZE_LOOP_ITERATORS", NULL},
 {"optimize-on-clauses", ' ', NULL, "Enable [disable] optimization of on clauses", "n", &fNoOptimizeOnClauses, "CHPL_DISABLE_OPTIMIZE_ON_CLAUSES", NULL},
 {"optimize-on-clause-limit", ' ', "<limit>", "Limit recursion depth of on clause optimization search", "I", &optimize_on_clause_limit, "CHPL_OPTIMIZE_ON_CLAUSE_LIMIT", NULL},
 {"privatization", ' ', NULL, "Enable [disable] privatization of distributed arrays and domains", "n", &fNoPrivatization, "CHPL_DISABLE_PRIVATIZATION", NULL},
 {"remote-value-forwarding", ' ', NULL, "Enable [disable] remote value forwarding", "n", &fNoRemoteValueForwarding, "CHPL_DISABLE_REMOTE_VALUE_FORWARDING", NULL},
 {"remote-serialization", ' ', NULL, "Enable [disable] serialization for remote consts", "n", &fNoRemoteSerialization, "CHPL_DISABLE_REMOTE_SERIALIZATION", NULL},
 {"remove-copy-calls", ' ', NULL, "Enable [disable] remove copy calls", "n", &fNoRemoveCopyCalls, "CHPL_DISABLE_REMOVE_COPY_CALLS", NULL},
 {"scalar-replacement", ' ', NULL, "Enable [disable] scalar replacement", "n", &fNoScalarReplacement, "CHPL_DISABLE_SCALAR_REPLACEMENT", NULL},
 {"scalar-replace-limit", ' ', "<limit>", "Limit on the size of tuples being replaced during scalar replacement", "I", &scalar_replace_limit, "CHPL_SCALAR_REPLACE_TUPLE_LIMIT", NULL},
 {"tuple-copy-opt", ' ', NULL, "Enable [disable] tuple (memcpy) optimization", "n", &fNoTupleCopyOpt, "CHPL_DISABLE_TUPLE_COPY_OPT", NULL},
 {"tuple-copy-limit", ' ', "<limit>", "Limit on the size of tuples considered for optimization", "I", &tuple_copy_limit, "CHPL_TUPLE_COPY_LIMIT", NULL},
 {"use-noinit", ' ', NULL, "Enable [disable] ability to skip default initialization through the keyword noinit", "N", &fUseNoinit, NULL, NULL},
 {"infer-local-fields", ' ', NULL, "Enable [disable] analysis to infer local fields in classes and records (experimental)", "n", &fNoInferLocalFields, "CHPL_DISABLE_INFER_LOCAL_FIELDS", NULL},
 {"vectorize", ' ', NULL, "Enable [disable] generation of vectorization hints", "n", &fNoVectorize, "CHPL_DISABLE_VECTORIZATION", setVectorize},

 {"", ' ', NULL, "Run-time Semantic Check Options", NULL, NULL, NULL, NULL},
 {"no-checks", ' ', NULL, "Disable all following run-time checks", "F", &fNoChecks, "CHPL_NO_CHECKS", turnOffChecks},
 {"bounds-checks", ' ', NULL, "Enable [disable] bounds checking", "n", &fNoBoundsChecks, "CHPL_NO_BOUNDS_CHECKING", NULL},
 {"cast-checks", ' ', NULL, "Enable [disable] safeCast() value checks", "n", &fNoCastChecks, NULL, NULL},
 {"div-by-zero-checks", ' ', NULL, "Enable [disable] divide-by-zero checks", "n", &fNoDivZeroChecks, NULL, NULL},
 {"formal-domain-checks", ' ', NULL, "Enable [disable] formal domain checking", "n", &fNoFormalDomainChecks, NULL, NULL},
 {"local-checks", ' ', NULL, "Enable [disable] local block checking", "n", &fNoLocalChecks, NULL, NULL},
 {"nil-checks", ' ', NULL, "Enable [disable] runtime nil checking", "n", &fNoNilChecks, "CHPL_NO_NIL_CHECKS", NULL},
 {"stack-checks", ' ', NULL, "Enable [disable] stack overflow checking", "n", &fNoStackChecks, "CHPL_STACK_CHECKS", setStackChecks},

 {"", ' ', NULL, "C Code Generation Options", NULL, NULL, NULL, NULL},
 {"codegen", ' ', NULL, "[Don't] Do code generation", "n", &no_codegen, "CHPL_NO_CODEGEN", NULL},
 {"cpp-lines", ' ', NULL, "[Don't] Generate #line annotations", "N", &printCppLineno, "CHPL_CG_CPP_LINES", noteCppLinesSet},
 {"max-c-ident-len", ' ', NULL, "Maximum length of identifiers in generated code, 0 for unlimited", "I", &fMaxCIdentLen, "CHPL_MAX_C_IDENT_LEN", NULL},
 {"munge-user-idents", ' ', NULL, "[Don't] Munge user identifiers to avoid naming conflicts with external code", "N", &fMungeUserIdents, "CHPL_MUNGE_USER_IDENTS"},
 {"savec", ' ', "<directory>", "Save generated C code in directory", "P", saveCDir, "CHPL_SAVEC_DIR", verifySaveCDir},

 {"", ' ', NULL, "C Code Compilation Options", NULL, NULL, NULL, NULL},
 {"ccflags", ' ', "<flags>", "Back-end C compiler flags (can be specified multiple times)", "S", NULL, "CHPL_CC_FLAGS", setCCFlags},
 {"debug", 'g', NULL, "[Don't] Support debugging of generated C code", "N", &debugCCode, "CHPL_DEBUG", setChapelDebug},
 {"dynamic", ' ', NULL, "Generate a dynamically linked binary", "F", &fLinkStyle, NULL, setDynamicLink},
 {"hdr-search-path", 'I', "<directory>", "C header search path", "P", incFilename, "CHPL_INCLUDE_PATH", handleIncDir},
 {"ldflags", ' ', "<flags>", "Back-end C linker flags (can be specified multiple times)", "S", NULL, "CHPL_LD_FLAGS", setLDFlags},
 {"lib-linkage", 'l', "<library>", "C library linkage", "P", libraryFilename, "CHPL_LIB_NAME", handleLibrary},
 {"lib-search-path", 'L', "<directory>", "C library search path", "P", libraryFilename, "CHPL_LIB_PATH", handleLibPath},
 {"optimize", 'O', NULL, "[Don't] Optimize generated C code", "N", &optimizeCCode, "CHPL_OPTIMIZE", NULL},
 {"specialize", ' ', NULL, "[Don't] Specialize generated C code for CHPL_TARGET_CPU", "N", &specializeCCode, "CHPL_SPECIALIZE", NULL},
 {"output", 'o', "<filename>", "Name output executable", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"static", ' ', NULL, "Generate a statically linked binary", "F", &fLinkStyle, NULL, NULL},

 {"", ' ', NULL, "LLVM Code Generation Options", NULL, NULL, NULL, NULL},
 {"llvm", ' ', NULL, "[Don't] use the LLVM code generator", "N", &llvmCodegen, "CHPL_LLVM_CODEGEN", NULL},
 {"llvm-wide-opt", ' ', NULL, "Enable [disable] LLVM wide pointer optimizations", "N", &fLLVMWideOpt, "CHPL_LLVM_WIDE_OPTS", NULL},
 {"mllvm", ' ', "<flags>", "LLVM flags (can be specified multiple times)", "S", NULL, "CHPL_MLLVM", setLLVMFlags},

 {"", ' ', NULL, "Compilation Trace Options", NULL, NULL, NULL, NULL},
 {"print-commands", ' ', NULL, "[Don't] print system commands", "N", &printSystemCommands, "CHPL_PRINT_COMMANDS", NULL},
 {"print-passes", ' ', NULL, "[Don't] print compiler passes", "N", &printPasses, "CHPL_PRINT_PASSES", NULL},
 {"print-passes-file", ' ', "<filename>", "Print compiler passes to <filename>", "S", NULL, "CHPL_PRINT_PASSES_FILE", setPrintPassesFile},

 {"", ' ', NULL, "Miscellaneous Options", NULL, NULL, NULL, NULL},
// Support for extern { c-code-here } blocks could be toggled with this
// flag, but instead we just leave it on if the compiler can do it.
// {"extern-c", ' ', NULL, "Enable [disable] extern C block support", "f", &externC, "CHPL_EXTERN_C", NULL},
 DRIVER_ARG_DEVELOPER,
 {"explain-call", ' ', "<call>[:<module>][:<line>]", "Explain resolution of call", "S256", fExplainCall, NULL, NULL},
 {"explain-instantiation", ' ', "<function|type>[:<module>][:<line>]", "Explain instantiation of type", "S256", fExplainInstantiation, NULL, NULL},
 {"explain-verbose", ' ', NULL, "Enable [disable] tracing of disambiguation with 'explain' options", "N", &fExplainVerbose, "CHPL_EXPLAIN_VERBOSE", NULL},
 {"instantiate-max", ' ', "<max>", "Limit number of instantiations", "I", &instantiation_limit, "CHPL_INSTANTIATION_LIMIT", NULL},
 {"print-all-candidates", ' ', NULL, "[Don't] print all candidates for a resolution failure", "N", &fPrintAllCandidates, "CHPL_PRINT_ALL_CANDIDATES", NULL},
 {"print-callgraph", ' ', NULL, "[Don't] print a representation of the callgraph for the program", "N", &fPrintCallGraph, "CHPL_PRINT_CALLGRAPH", NULL},
 {"print-callstack-on-error", ' ', NULL, "[Don't] print the Chapel call stack leading to each error or warning", "N", &fPrintCallStackOnError, "CHPL_PRINT_CALLSTACK_ON_ERROR", NULL},
 {"print-unused-functions", ' ', NULL, "[Don't] print the name and location of unused functions", "N", &fPrintUnusedFns, NULL, NULL},
 {"set", 's', "<name>[=<value>]", "Set config value", "S", NULL, NULL, readConfig},
 {"task-tracking", ' ', NULL, "Enable [disable] runtime task tracking", "N", &fEnableTaskTracking, "CHPL_TASK_TRACKING", NULL},

 {"", ' ', NULL, "Compiler Configuration Options", NULL, NULL, NULL, NULL},
 {"home", ' ', "<path>", "Path to Chapel's home directory", "S", NULL, "_CHPL_HOME", setHome},
 {"atomics", ' ', "<atomics-impl>", "Specify atomics implementation", "S", NULL, "_CHPL_ATOMICS", setEnv},
 {"network-atomics", ' ', "<network>", "Specify network atomics implementation", "S", NULL, "_CHPL_NETWORK_ATOMICS", setEnv},
 {"aux-filesys", ' ', "<aio-system>", "Specify auxiliary I/O system", "S", NULL, "_CHPL_AUX_FILESYS", setEnv},
 {"comm", ' ', "<comm-impl>", "Specify communication implementation", "S", NULL, "_CHPL_COMM", setEnv},
 {"comm-substrate", ' ', "<conduit>", "Specify communication conduit", "S", NULL, "_CHPL_COMM_SUBSTRATE", setEnv},
 {"gasnet-segment", ' ', "<segment>", "Specify GASNet memory segment", "S", NULL, "_CHPL_GASNET_SEGMENT", setEnv},
 {"gmp", ' ', "<gmp-version>", "Specify GMP library", "S", NULL, "_CHPL_GMP", setEnv},
 {"hwloc", ' ', "<hwloc-impl>", "Specify whether to use hwloc", "S", NULL, "_CHPL_HWLOC", setEnv},
 {"launcher", ' ', "<launcher-system>", "Specify how to launch programs", "S", NULL, "_CHPL_LAUNCHER", setEnv},
 {"locale-model", ' ', "<locale-model>", "Specify locale model to use", "S", NULL, "_CHPL_LOCALE_MODEL", setEnv},
 {"make", ' ', "<make utility>", "Make utility for generated code", "S", NULL, "_CHPL_MAKE", setEnv},
 {"mem", ' ', "<mem-impl>", "Specify the memory manager", "S", NULL, "_CHPL_MEM", setEnv},
 {"regexp", ' ', "<regexp>", "Specify whether to use regexp support", "S", NULL, "_CHPL_REGEXP", setEnv},
 {"target-arch", ' ', "<architecture>", "Target architecture / machine type", "S", NULL, "_CHPL_TARGET_ARCH", setEnv},
 {"target-compiler", ' ', "<compiler>", "Compiler for generated code", "S", NULL, "_CHPL_TARGET_COMPILER", setEnv},
 {"target-cpu", ' ', "<cpu>", "Target cpu model for specialization", "S", NULL, "_CHPL_TARGET_CPU", setEnv},
 {"target-platform", ' ', "<platform>", "Platform for cross-compilation", "S", NULL, "_CHPL_TARGET_PLATFORM", setEnv},
 {"tasks", ' ', "<task-impl>", "Specify tasking implementation", "S", NULL, "_CHPL_TASKS", setEnv},
 {"timers", ' ', "<timer-impl>", "Specify timer implementation", "S", NULL, "_CHPL_TIMERS", setEnv},

 {"", ' ', NULL, "Compiler Information Options", NULL, NULL, NULL, NULL},
 DRIVER_ARG_COPYRIGHT,
 DRIVER_ARG_HELP,
 DRIVER_ARG_HELP_ENV,
 DRIVER_ARG_HELP_SETTINGS,
 DRIVER_ARG_LICENSE,
 DRIVER_ARG_VERSION,

 // NOTE: Developer flags should not have 1-character equivalents
 //       (so that they are available for user flags)
 {"", ' ', NULL, "Developer Flags -- Debug Output", NULL, NULL, NULL, NULL},
 {"cc-warnings", ' ', NULL, "[Don't] Give warnings for generated code", "N", &ccwarnings, "CHPL_CC_WARNINGS", NULL},
 {"gen-ids", ' ', NULL, "[Don't] pepper generated code with BaseAST::ids", "N", &fGenIDS, "CHPL_GEN_IDS", NULL},
 {"html", ' ', NULL, "Dump IR in HTML format (toggle)", "T", &fdump_html, "CHPL_HTML", NULL},
 {"html-user", ' ', NULL, "Dump IR in HTML for user module(s) only (toggle)", "T", &fdump_html, "CHPL_HTML_USER", setHtmlUser},
 {"html-wrap-lines", ' ', NULL, "[Don't] allow wrapping lines in HTML dumps", "N", &fdump_html_wrap_lines, "CHPL_HTML_WRAP_LINES", NULL},
 {"html-print-block-ids", ' ', NULL, "[Don't] print block IDs in HTML dumps", "N", &fdump_html_print_block_IDs, "CHPL_HTML_PRINT_BLOCK_IDS", NULL},
 {"html-chpl-home", ' ', NULL, "Path to use instead of CHPL_HOME in HTML dumps", "P", fdump_html_chpl_home, "CHPL_HTML_CHPL_HOME", NULL},
 {"log", ' ', NULL, "Dump IR in text format.", "F", &fLog, "CHPL_LOG", NULL},
 {"log-dir", ' ', "<path>", "Specify log directory", "P", log_dir, "CHPL_LOG_DIR", setLogDir},
 {"log-ids", ' ', NULL, "[Don't] include BaseAST::ids in log files", "N", &fLogIds, "CHPL_LOG_IDS", NULL},
 {"log-module", ' ', "<module-name>", "Restrict IR dump to the named module", "S256", log_module, "CHPL_LOG_MODULE", NULL},
 {"log-pass", ' ', "<passname>", "Restrict IR dump to the named pass. Can be specified multiple times", "S", NULL, "CHPL_LOG_PASS", setLogPass},
 {"log-node", ' ', NULL, "Dump IR using AstDumpToNode", "F", &fLogNode, "CHPL_LOG_NODE", NULL},
// {"log-symbol", ' ', "<symbol-name>", "Restrict IR dump to the named symbol(s)", "S256", log_symbol, "CHPL_LOG_SYMBOL", NULL}, // This doesn't work yet.
 {"llvm-print-ir", ' ', "<name>", "Dump LLVM Intermediate Representation of given function to stdout", "S", NULL, "CHPL_LLVM_PRINT_IR", &setPrintIr},
 {"llvm-print-ir-stage", ' ', "<stage>", "Specifies from which LLVM optimization stage to print function: none, basic, full", "S", NULL, "CHPL_LLVM_PRINT_IR_STAGE", &verifyStageAndSetStageNum},
 {"verify", ' ', NULL, "Run consistency checks during compilation", "N", &fVerify, "CHPL_VERIFY", NULL},
 {"parse-only", ' ', NULL, "Stop compiling after 'parse' pass for syntax checking", "N", &fParseOnly, NULL, NULL},
 {"parser-debug", ' ', NULL, "Set parser debug level", "+", &debugParserLevel, "CHPL_PARSER_DEBUG", NULL},
 {"debug-short-loc", ' ', NULL, "Display long [short] location in certain debug outputs", "N", &debugShortLoc, "CHPL_DEBUG_SHORT_LOC", NULL},
 {"print-emitted-code-size", ' ', NULL, "Print emitted code size", "F", &fPrintEmittedCodeSize, NULL, NULL},
 {"print-module-resolution", ' ', NULL, "Print name of module being resolved", "F", &fPrintModuleResolution, "CHPL_PRINT_MODULE_RESOLUTION", NULL},
 {"print-dispatch", ' ', NULL, "Print dynamic dispatch table", "F", &fPrintDispatch, NULL, NULL},
 {"print-statistics", ' ', "[n|k|t]", "Print AST statistics", "S256", fPrintStatistics, NULL, NULL},
 {"report-aliases", ' ', NULL, "Report aliases in user code", "N", &fReportAliases, NULL, NULL},
 {"report-blocking", ' ', NULL, "Report blocking functions in user code", "N", &fReportBlocking, NULL, NULL},
 {"report-inlining", ' ', NULL, "Print inlined functions", "F", &report_inlining, NULL, NULL},
 {"report-dead-blocks", ' ', NULL, "Print dead block removal stats", "F", &fReportDeadBlocks, NULL, NULL},
 {"report-dead-modules", ' ', NULL, "Print dead module removal stats", "F", &fReportDeadModules, NULL, NULL},
 {"report-optimized-loop-iterators", ' ', NULL, "Print stats on optimized single loop iterators", "F", &fReportOptimizedLoopIterators, NULL, NULL},
 {"report-inlined-iterators", ' ', NULL, "Print stats on inlined iterators", "F", &fReportInlinedIterators, NULL, NULL},
 {"report-vectorized-loops", ' ', NULL, "Show which loops have vectorization hints", "F", &fReportVectorizedLoops, NULL, NULL},
 {"report-optimized-on", ' ', NULL, "Print information about on clauses that have been optimized for potential fast remote fork operation", "F", &fReportOptimizedOn, NULL, NULL},
 {"report-optimized-forall-unordered-ops", ' ', NULL, "Show which statements in foralls have been converted to unordered operations", "F", &fReportOptimizeForallUnordered, NULL, NULL},
 {"report-promotion", ' ', NULL, "Print information about scalar promotion", "F", &fReportPromotion, NULL, NULL},
 {"report-scalar-replace", ' ', NULL, "Print scalar replacement stats", "F", &fReportScalarReplace, NULL, NULL},

 {"", ' ', NULL, "Developer Flags -- Miscellaneous", NULL, NULL, NULL, NULL},
 DRIVER_ARG_BREAKFLAGS_COMMON,
 {"break-on-codegen", ' ', NULL, "Break when function cname is code generated", "S256", &breakOnCodegenCname, "CHPL_BREAK_ON_CODEGEN", NULL},
 {"break-on-codegen-id", ' ', NULL, "Break when id is code generated", "I", &breakOnCodegenID, "CHPL_BREAK_ON_CODEGEN_ID", NULL},
 {"default-dist", ' ', "<distribution>", "Change the default distribution", "S256", defaultDist, "CHPL_DEFAULT_DIST", NULL},
 {"explain-call-id", ' ', "<call-id>", "Explain resolution of call by ID", "I", &explainCallID, NULL, NULL},
 {"break-on-resolve-id", ' ', NULL, "Break when function call with AST id is resolved", "I", &breakOnResolveID, "CHPL_BREAK_ON_RESOLVE_ID", NULL},
 {"denormalize", ' ', NULL, "Enable [disable] denormalization", "N", &fDenormalize, "CHPL_DENORMALIZE", NULL},
 DRIVER_ARG_DEBUGGERS,
 {"interprocedural-alias-analysis", ' ', NULL, "Enable [disable] interprocedural alias analysis", "n", &fNoInterproceduralAliasAnalysis, NULL, NULL},
 {"lifetime-checking", ' ', NULL, "Enable [disable] lifetime checking pass", "n", &fNoLifetimeChecking, NULL, NULL},
 {"split-initialization", ' ', NULL, "Enable [disable] support for split initialization", "n", &fNoSplitInit, NULL, NULL},
 {"early-deinit", ' ', NULL, "Enable [disable] support for early deinit based upon expiring value analysis", "n", &fNoEarlyDeinit, NULL, NULL},
 {"copy-elision", ' ', NULL, "Enable [disable] copy elision based upon expiring value analysis", "n", &fNoCopyElision, NULL, NULL},
 {"legacy-classes", ' ', NULL, "Deprecated flag - does not affect compilation", "N", &fLegacyClasses, NULL, &warnUponLegacyClasses},
 {"ignore-nilability-errors", ' ', NULL, "Allow compilation to continue by coercing away nilability", "N", &fIgnoreNilabilityErrors, NULL, NULL},
 {"overload-sets-checks", ' ', NULL, "Report potentially hijacked calls", "N", &fOverloadSetsChecks, NULL, NULL},
 {"compile-time-nil-checking", ' ', NULL, "Enable [disable] compile-time nil checking", "N", &fCompileTimeNilChecking, "CHPL_NO_COMPILE_TIME_NIL_CHECKS", NULL},
 {"ignore-errors", ' ', NULL, "[Don't] attempt to ignore errors", "N", &ignore_errors, "CHPL_IGNORE_ERRORS", NULL},
 {"ignore-user-errors", ' ', NULL, "[Don't] attempt to ignore user errors", "N", &ignore_user_errors, "CHPL_IGNORE_USER_ERRORS", NULL},
 {"ignore-errors-for-pass", ' ', NULL, "[Don't] attempt to ignore errors until the end of the pass in which they occur", "N", &ignore_errors_for_pass, "CHPL_IGNORE_ERRORS_FOR_PASS", NULL},
 {"infer-const-refs", ' ', NULL, "Enable [disable] inferring const refs", "n", &fNoInferConstRefs, NULL, NULL},
 {"library", ' ', NULL, "Generate a Chapel library file", "F", &fLibraryCompile, NULL, NULL},
 {"library-dir", ' ', "<directory>", "Save generated library helper files in directory", "P", libDir, "CHPL_LIB_SAVE_DIR", verifySaveLibDir},
 {"library-header", ' ', "<filename>", "Name generated header file", "P", libmodeHeadername, NULL, setLibmode},
 {"library-makefile", ' ', NULL, "Generate a makefile to help use the generated library", "F", &fLibraryMakefile, NULL, setLibmode},
 {"library-fortran", ' ', NULL, "Generate a module compatible with Fortran", "F", &fLibraryFortran, NULL, setLibmode},
 {"library-fortran-name", ' ', "<modulename>", "Name generated Fortran module", "P", fortranModulename, NULL, setFortranAndLibmode},
 {"library-python", ' ', NULL, "Generate a module compatible with Python", "F", &fLibraryPython, NULL, setLibmode},
 {"library-python-name", ' ', "<filename>", "Name generated Python module", "P", pythonModulename, NULL, setPythonAndLibmode},
 {"library-ml-debug", ' ', NULL, "Enable [disable] generation of debug messages in multi-locale libraries", "N", &fMultiLocaleLibraryDebug, NULL, NULL},
 {"localize-global-consts", ' ', NULL, "Enable [disable] optimization of global constants", "n", &fNoGlobalConstOpt, "CHPL_DISABLE_GLOBAL_CONST_OPT", NULL},
 {"local-temp-names", ' ', NULL, "[Don't] Generate locally-unique temp names", "N", &localTempNames, "CHPL_LOCAL_TEMP_NAMES", NULL},
 {"log-deleted-ids-to", ' ', "<filename>", "Log AST id and memory address of each deleted node to the specified file", "P", deletedIdFilename, "CHPL_DELETED_ID_FILENAME", NULL},
 {"memory-frees", ' ', NULL, "Enable [disable] memory frees in the generated code", "n", &fNoMemoryFrees, "CHPL_DISABLE_MEMORY_FREES", NULL},
 {"override-checking", ' ', NULL, "[Don't] check use of override keyword", "N", &fOverrideChecking, NULL, NULL},
 {"prepend-internal-module-dir", ' ', "<directory>", "Prepend directory to internal module search path", "P", NULL, NULL, addInternalModulePath},
 {"prepend-standard-module-dir", ' ', "<directory>", "Prepend directory to standard module search path", "P", NULL, NULL, addStandardModulePath},
 {"preserve-inlined-line-numbers", ' ', NULL, "[Don't] Preserve file names/line numbers in inlined code", "N", &preserveInlinedLineNumbers, "CHPL_PRESERVE_INLINED_LINE_NUMBERS", NULL},
 {"print-id-on-error", ' ', NULL, "[Don't] print AST id in error messages", "N", &fPrintIDonError, "CHPL_PRINT_ID_ON_ERROR", NULL},
 {"print-unused-internal-functions", ' ', NULL, "[Don't] print names and locations of unused internal functions", "N", &fPrintUnusedInternalFns, NULL, NULL},
 {"region-vectorizer", ' ', NULL, "Enable [disable] region vectorizer", "N", &fRegionVectorizer, NULL, NULL},
 {"remove-empty-records", ' ', NULL, "Enable [disable] empty record removal", "n", &fNoRemoveEmptyRecords, "CHPL_DISABLE_REMOVE_EMPTY_RECORDS", NULL},
 {"remove-unreachable-blocks", ' ', NULL, "[Don't] remove unreachable blocks after resolution", "N", &fRemoveUnreachableBlocks, "CHPL_REMOVE_UNREACHABLE_BLOCKS", NULL},
 {"replace-array-accesses-with-ref-temps", ' ', NULL, "Enable [disable] replacing array accesses with reference temps (experimental)", "N", &fReplaceArrayAccessesWithRefTemps, NULL, NULL },
 {"incremental", ' ', NULL, "Enable [disable] using incremental compilation", "N", &fIncrementalCompilation, "CHPL_INCREMENTAL_COMP", NULL},
 {"minimal-modules", ' ', NULL, "Enable [disable] using minimal modules",               "N", &fMinimalModules, "CHPL_MINIMAL_MODULES", NULL},
 {"print-chpl-settings", ' ', NULL, "Print current chapel settings and exit", "F", &fPrintChplSettings, NULL,NULL},
 {"stop-after-pass", ' ', "<passname>", "Stop compilation after reaching this pass", "S128", &stopAfterPass, "CHPL_STOP_AFTER_PASS", NULL},
 {"force-vectorize", ' ', NULL, "Ignore vectorization hazards when vectorizing loops", "N", &fForceVectorize, NULL, NULL},
 {"warn-const-loops", ' ', NULL, "Enable [disable] warnings for some 'while' loops with constant conditions", "N", &fWarnConstLoops, "CHPL_WARN_CONST_LOOPS", NULL},
 {"warn-domain-literal", ' ', NULL, "Enable [disable] old domain literal syntax warnings", "n", &fNoWarnDomainLiteral, "CHPL_WARN_DOMAIN_LITERAL", setWarnDomainLiteral},
 {"warn-tuple-iteration", ' ', NULL, "Enable [disable] warnings for tuple iteration", "n", &fNoWarnTupleIteration, "CHPL_WARN_TUPLE_ITERATION", setWarnTupleIteration},
 {"warn-special", ' ', NULL, "Enable [disable] special warnings", "n", &fNoWarnSpecial, "CHPL_WARN_SPECIAL", setWarnSpecial},

 DRIVER_ARG_PRINT_CHPL_HOME,
 DRIVER_ARG_LAST
};


static ArgumentState sArgState = {
  0,
  0,
  "program",
  "path",
  NULL
};

static void printStuff(const char* argv0) {
  bool shouldExit       = false;
  bool printedSomething = false;

  if (fPrintVersion) {
    fprintf(stdout, "%s version %s\n", sArgState.program_name, compileVersion);

    fPrintCopyright  = true;
    printedSomething = true;
    shouldExit       = true;
  }

  if (fPrintLicense) {
    fprintf(stdout,
#include "LICENSE"
            );

    fPrintCopyright  = false;
    shouldExit       = true;
    printedSomething = true;
  }

  if (fPrintCopyright) {
    fprintf(stdout,
#include "COPYRIGHT"
            );

    printedSomething = true;
  }
  if( fPrintChplHome ) {
    char* guess = findProgramPath(argv0);

    printf("%s\t%s\n", CHPL_HOME, guess);
    const char* prefix = get_configured_prefix();
    if (prefix != NULL && prefix[0] != '\0' )
      printf("# configured prefix  %s\n", prefix);

    free(guess);

    printedSomething = true;
  }

  if( fPrintChplSettings ) {
    char buf[FILENAME_MAX+1] = "";
    printf("CHPL_HOME: %s\n", CHPL_HOME);
    printf("CHPL_RUNTIME_LIB: %s\n", CHPL_RUNTIME_LIB);
    printf("CHPL_RUNTIME_INCL: %s\n", CHPL_RUNTIME_INCL);
    printf("CHPL_THIRD_PARTY: %s\n", CHPL_THIRD_PARTY);
    printf("\n");
    int wanted_to_write = snprintf(buf, sizeof(buf),
                                   "%s/util/printchplenv --all", CHPL_HOME);
    if (wanted_to_write < 0) {
      USR_FATAL("character encoding error in CHPL_HOME path name");
    } else if ((size_t)wanted_to_write >= sizeof(buf)) {
      USR_FATAL("CHPL_HOME path name is too long");
    }
    int status = mysystem(buf, "running printchplenv", false);
    clean_exit(status);
  }

  if (fPrintHelp || (!printedSomething && sArgState.nfile_arguments < 1)) {
    if (printedSomething) printf("\n");

    usage(&sArgState, !fPrintHelp, fPrintEnvHelp, fPrintSettingsHelp);

    shouldExit       = true;
    printedSomething = true;
  }

  if (printedSomething && sArgState.nfile_arguments < 1) {
    shouldExit       = true;
  }

  if (shouldExit) {
    clean_exit(0);
  }
}

bool useDefaultEnv(std::string key) {
  // Check conditions for which default value should override argument provided

  // For Cray programming environments, we must infer CHPL_TARGET_CPU
  // Note: When CHPL_TARGET_CPU is processed, CHPL_HOST_COMPILER is already
  // set in envMap, due to the order of printchplenv output
  if (key == "CHPL_TARGET_CPU") {
    if (strstr(envMap["CHPL_TARGET_COMPILER"], "cray-prgenv") != NULL) {
      return true;
    }
  }

  // Always use default env for internal variables that could include spaces
  if (key == "CHPL_THIRD_PARTY_LINK_ARGS") {
    return true;
  }

  return false;
}

static void populateEnvMap() {
  // Destructively parses output of 'printchplenv --simple' for "key=value"
  // pairs and populates global envMap if the key has not been already set from
  // argument processing

  // Call printchplenv and pipe output into string
  std::string output = runPrintChplEnv(envMap);

  // Lines
  std::string line= "";
  std::string lineDelimiter = "\n";
  size_t linePos = 0;        // Line break position

  // Tokens
  std::string tokenDelimiter = "=";
  size_t delimiterPos = 0;    // Position of delimiter
  size_t valuePos = 0;        // Position of value

  std::string key = "";
  std::string value = "";

  while ((linePos = output.find(lineDelimiter)) != std::string::npos)
  {
    line = output.substr(0, linePos);

    // Key is substring up until "=" on a given line
    delimiterPos = line.find(tokenDelimiter);
    key = line.substr(0, delimiterPos);

    // Value is substring after "=" on a given line
    valuePos = delimiterPos + tokenDelimiter.length();
    value = line.substr(valuePos);

    // If key does not have a value in envMap, map it to the parsed value
    if (envMap.find(key) == envMap.end()) {
      envMap[key] = strdup(value.c_str());
    } else if (useDefaultEnv(key)) {
      envMap.erase(key);
      envMap[key] = strdup(value.c_str());
    }

    output.erase(0, linePos + lineDelimiter.length());
  }
}

static void setChapelEnvs() {
  // Update compiler global CHPL_vars with envMap values

  CHPL_HOST_PLATFORM   = envMap["CHPL_HOST_PLATFORM"];
  CHPL_HOST_ARCH       = envMap["CHPL_HOST_ARCH"];
  CHPL_HOST_COMPILER   = envMap["CHPL_HOST_COMPILER"];
  CHPL_HOST_CPU        = envMap["CHPL_HOST_CPU"];
  CHPL_TARGET_PLATFORM = envMap["CHPL_TARGET_PLATFORM"];
  CHPL_TARGET_ARCH     = envMap["CHPL_TARGET_ARCH"];
  CHPL_TARGET_CPU      = envMap["CHPL_TARGET_CPU"];
  CHPL_RUNTIME_CPU     = envMap["CHPL_RUNTIME_CPU"];
  CHPL_TARGET_BACKEND_CPU = envMap["CHPL_TARGET_BACKEND_CPU"];
  CHPL_TARGET_CPU_FLAG = envMap["CHPL_TARGET_CPU_FLAG"];
  CHPL_TARGET_COMPILER = envMap["CHPL_TARGET_COMPILER"];
  CHPL_ORIG_TARGET_COMPILER = envMap["CHPL_ORIG_TARGET_COMPILER"];
  CHPL_LOCALE_MODEL    = envMap["CHPL_LOCALE_MODEL"];
  CHPL_COMM            = envMap["CHPL_COMM"];
  CHPL_COMM_SUBSTRATE  = envMap["CHPL_COMM_SUBSTRATE"];
  CHPL_GASNET_SEGMENT  = envMap["CHPL_GASNET_SEGMENT"];
  CHPL_TASKS           = envMap["CHPL_TASKS"];
  CHPL_LAUNCHER        = envMap["CHPL_LAUNCHER"];
  CHPL_TIMERS          = envMap["CHPL_TIMERS"];
  CHPL_MEM             = envMap["CHPL_MEM"];
  CHPL_MAKE            = envMap["CHPL_MAKE"];
  CHPL_ATOMICS         = envMap["CHPL_ATOMICS"];
  CHPL_NETWORK_ATOMICS = envMap["CHPL_NETWORK_ATOMICS"];
  CHPL_GMP             = envMap["CHPL_GMP"];
  CHPL_HWLOC           = envMap["CHPL_HWLOC"];
  CHPL_REGEXP          = envMap["CHPL_REGEXP"];
  CHPL_LLVM            = envMap["CHPL_LLVM"];
  CHPL_AUX_FILESYS     = envMap["CHPL_AUX_FILESYS"];
  CHPL_UNWIND          = envMap["CHPL_UNWIND"];
  CHPL_LIB_PIC         = envMap["CHPL_LIB_PIC"];

  CHPL_RUNTIME_SUBDIR  = envMap["CHPL_RUNTIME_SUBDIR"];
  CHPL_LAUNCHER_SUBDIR = envMap["CHPL_LAUNCHER_SUBDIR"];
  CHPL_LLVM_UNIQ_CFG_PATH = envMap["CHPL_LLVM_UNIQ_CFG_PATH"];

  // Make sure there are no NULLs in envMap
  // a NULL in envMap might mean that one of the variables
  // the compiler expected printchplenv to produce was not produced.
  for (std::map<std::string, const char*>::iterator env=envMap.begin();
       env!=envMap.end(); ++env) {
    INT_ASSERT(env->second != NULL);
  }
}

static void setupChplGlobals(const char* argv0) {
  // Set CHPL_HOME, populate envMap with defaults, and set global CHPL_vars

  // Set CHPL_HOME the traditional way if it was not passed as an argument
  if (envMap.find("CHPL_HOME") == envMap.end())
  {
    setupChplHome(argv0);

    // Keep envMap updated
    envMap["CHPL_HOME"] = CHPL_HOME;
  }

  // tell printchplenv that we're doing an LLVM build
  if (llvmCodegen) {
    envMap["CHPL_LLVM_CODEGEN"] = "llvm";
  }

  // Populate envMap from printchplenv, never overwriting existing elements
  populateEnvMap();

  // Set global CHPL_vars with updated envMap values
  setChapelEnvs();
}

static void postTaskTracking() {
  if (fEnableTaskTracking) {
    if (strcmp(CHPL_TASKS, "fifo") != 0) {
      USR_WARN("Enabling task tracking with CHPL_TASKS=%s has no effect other than to slow down compilation", CHPL_TASKS);
    }
  }
}

static void postStaticLink() {
  if (!strcmp(CHPL_TARGET_PLATFORM, "darwin")) {
    if (fLinkStyle == LS_STATIC) {
      USR_WARN("Static compilation is not supported on OS X, ignoring flag.");
      fLinkStyle = fMultiLocaleInterop ? LS_DYNAMIC : LS_DEFAULT;
    }

    //
    // The default link style translates to static (at least for now), so we
    // need to account for that when building a multi-locale library or else
    // we'll get the same errors we do for static linking on `darwin`.
    //
    if (fMultiLocaleInterop && fLinkStyle == LS_DEFAULT) {
      fLinkStyle = LS_DYNAMIC;
    }
  }
}

static void postLocal() {
  if (!fUserSetLocal) fLocal = !strcmp(CHPL_COMM, "none");
}

static void postVectorize() {
  // Make sure fYesVectorize and fNoVectorize are respected
  // but if neither is set, compute the default (based on --llvm or not)
  if (fForceVectorize)
    fYesVectorize = true;

  if (fNoVectorize) {
    fYesVectorize = false;
  } else if (fYesVectorize) {
    fNoVectorize = false;
  } else if (llvmCodegen) {
    // LLVM code generator defaults to enabling vectorization
    fYesVectorize = true;
    fNoVectorize = false;
  } else {
    // C backend defaults to no vectorization
    fYesVectorize = false;
    fNoVectorize = true;
  }
}

static void setMultiLocaleInterop() {
  // We must be compiling a multi-locale library to be eligible for MLI.
  if (!fLibraryCompile || !strcmp(CHPL_COMM, "none")) {
    return;
  }

  if (llvmCodegen) {
    USR_FATAL("Multi-locale libraries do not support --llvm");
  }

  if (fLibraryFortran) {
    USR_FATAL("Multi-locale libraries do not support --library-fortran");
  }

  fMultiLocaleInterop = true;
}

static void setMaxCIndentLen() {
  bool gotPGI = !strcmp(CHPL_TARGET_COMPILER, "pgi")
             || !strcmp(CHPL_TARGET_COMPILER, "cray-prgenv-pgi");
  // conservatively how much is needed for the current PGI compiler
  if (gotPGI) fMaxCIdentLen = 1020;
}

static void setPrintCppLineno() {
  if (developer && !userSetCppLineno) printCppLineno = false;
}

static void checkLLVMCodeGen() {
#ifndef HAVE_LLVM
 if (llvmCodegen) USR_FATAL("This compiler was built without LLVM support");
#endif
}

static void checkTargetCpu() {
  if (specializeCCode && (strcmp(CHPL_TARGET_CPU, "unknown") == 0)) {
    USR_WARN("--specialize was set, but CHPL_TARGET_CPU is 'unknown'. If "
              "you want any specialization to occur please set CHPL_TARGET_CPU "
              "to a proper value.");
  }
}

static void checkIncrementalAndOptimized() {
  std::size_t optimizationsEnabled = ccflags.find("-O");
  if(fIncrementalCompilation && ( optimizeCCode ||
      optimizationsEnabled!=std::string::npos ))
    USR_WARN("Compiling with --incremental along with optimizations enabled"
              " may lead to a slower execution time compared to --fast or"
              " using -O optimizations directly.");
}

static void checkMLDebugAndLibmode(void) {

  if (!fMultiLocaleLibraryDebug) { return; }

  fLibraryCompile = true;

  if (!strcmp(CHPL_COMM, "none")) {
    fMultiLocaleLibraryDebug = false;

    const char* warning =
        "Compiling a single locale library because CHPL_COMM is none.";

    USR_WARN(warning);
  }

  return;
}

static void checkNotLibraryAndMinimalModules(void) {
  const bool isLibraryCompile = fLibraryCompile || fMultiLocaleInterop;
  if (isLibraryCompile && fMinimalModules) {
    USR_FATAL("Libraries do not currently support \'--minimal-modules\'");
  }

  return;
}

static void postprocess_args() {
  // Processes that depend on results of passed arguments or values of CHPL_vars

  setMaxCIndentLen();

  postLocal();

  postVectorize();

  postTaskTracking();

  setMultiLocaleInterop();

  postStaticLink();

  checkMLDebugAndLibmode();

  checkNotLibraryAndMinimalModules();

  setPrintCppLineno();

  checkLLVMCodeGen();

  checkTargetCpu();

  checkIncrementalAndOptimized();
}

int main(int argc, char* argv[]) {
  PhaseTracker tracker;

  startCatchingSignals();

  {
    astlocMarker markAstLoc(0, "<internal>");

    tracker.StartPhase("init");

    init_args(&sArgState, argv[0]);

    fDocs   = (strcmp(sArgState.program_name, "chpldoc")  == 0) ? true : false;

    // Initialize the arguments for argument state. If chpldoc, use the docs
    // specific arguments. Otherwise, use the regular arguments.
    if (fDocs) {
      init_arg_desc(&sArgState, docs_arg_desc);
    } else {
      init_arg_desc(&sArgState, arg_desc);
    }


    initFlags();
    initAstrConsts();
    initRootModule();
    initPrimitive();
    initPrimitiveTypes();

    initChplProgram();

    initStringLiteralModule();

    process_args(&sArgState, argc, argv);

    setupChplGlobals(argv[0]);

    postprocess_args();

    initCompilerGlobals(); // must follow argument parsing

    setupModulePaths();

    recordCodeGenStrings(argc, argv);
  } // astlocMarker scope

  printStuff(argv[0]);

  if (fRungdb)
    runCompilerInGDB(argc, argv);

  if (fRunlldb)
    runCompilerInLLDB(argc, argv);

  addSourceFiles(sArgState.nfile_arguments, sArgState.file_argument);

  runPasses(tracker, fDocs);

  tracker.StartPhase("driverCleanup");

  free_args(&sArgState);

  tracker.Stop();

  if (printPasses == true || printPassesFile != NULL) {
    tracker.ReportPass();
    tracker.ReportTotal();
    tracker.ReportRollup();
  }

  if (printPassesFile != NULL) {
    fclose(printPassesFile);
  }

  clean_exit(0);

  return 0;
}
