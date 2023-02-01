/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "misc.h"

#include "astlocs.h"
#include "baseAST.h"
#include "chpl.h"
#include "chpl/framework/Context.h"
#include "chpl/util/terminal.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "parser.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "version.h"

#include <csignal>
#include <fcntl.h>
#include <sys/stat.h>

static bool forceWidePtrs();

GpuCodegenType getGpuCodegenType() {
  static const GpuCodegenType cached = []() {
    INT_ASSERT(usingGpuLocaleModel());
    if (0 == strcmp(CHPL_GPU_CODEGEN, "cuda")) {
      return GpuCodegenType::GPU_CG_NVIDIA_CUDA;
    } else {
      INT_ASSERT(!strcmp(CHPL_GPU_CODEGEN, "rocm"));
      return GpuCodegenType::GPU_CG_AMD_HIP;
    }
  }();
  return cached;
}

// Return true if the current locale model needs GPU code generation
bool usingGpuLocaleModel() {
  return 0 == strcmp(CHPL_LOCALE_MODEL, "gpu");
}

bool forceWidePtrsForLocal() {
  // For the gpu (for now) we don't want wide pointers inside kernel
  // functions (which we consider a local block) but we still want
  // to force wide pointers outside of there so we have `forceWidePtrs`
  // return true but don't want it to kick in for local blocks
  if(usingGpuLocaleModel()) {
    return false;
  }

  return fLocal && forceWidePtrs();
}

bool requireWideReferences() {
  return !fLocal || forceWidePtrs();
}

bool requireOutlinedOn() {
  return requireWideReferences();
}

const char* cleanFilename(const char* name) {
  static int  chplHomeLen = strlen(CHPL_HOME);
  const char* retval      = NULL;

  if (strncmp(name, CHPL_HOME, chplHomeLen) == 0) {
    retval = astr("$CHPL_HOME", name + chplHomeLen);
  } else {
    retval = name;
  }

  return retval;
}

const char* cleanFilename(const BaseAST* ast) {
  const char* retval = NULL;

  if (const char* astFname = ast->fname()) {
    retval = cleanFilename(astFname);

  } else if (yyfilename != NULL) {
    retval = cleanFilename(yyfilename);

  } else {
    retval = astr("<unknown>");
  }

  return retval;
}

static void cleanup_for_exit() {
  closeCodegenFiles();

  // Currently, gpu code generation is done in on forked process. This
  // forked process produces some files in the tmp directory that are
  // later read by the main process, so we want the main process
  // to clean up the temp dir and not the forked process.
  if (!gCodegenGPU) {
    deleteTmpDir();
  }
  stopCatchingSignals();
}

//
// Chances are that all non-flat locale models will require wide
// pointers.  Ultimately, we'd like to have such decisions be made by
// param fields/methods within the locale models themselves, but that
// would require a fairly large refactoring, so for now, we
// special-case 'flat' with the expectation that most other locale
// models will not be flat.
//
static bool forceWidePtrs() {
  return (strcmp(CHPL_LOCALE_MODEL, "flat") != 0);
}

void printCallStackCalls() {
  printf("\n" "callStack %d elms\n\n", callStack.n);

  for (int i = 0; i < callStack.n; i++) {
    CallExpr* call = callStack.v[i];
    FnSymbol* cfn  = call->resolvedFunction();

    printf("%d  %d %s  <-  %d %s\n",
           i,
           cfn  ? cfn->id  : 0, cfn  ? cfn->name         : "<no callee>",
           call ? call->id : 0, call ? call->stringLoc() : "<no call>");
  }

  printf("\n");
}

static void handleInterrupt(int sig) {
  stopCatchingSignals();
  fprintf(stderr, "error: received interrupt\n");
  fflush(stdout);
  fflush(stderr);

  clean_exit(1);
}

static void handleSegFault(int sig) {
  stopCatchingSignals();
  // TODO: Probably unwise to use this machinery in a signal handler...
  INT_FATAL("seg fault");
}

void startCatchingSignals() {
  signal(SIGINT,  handleInterrupt);
  signal(SIGTERM, handleInterrupt);
  signal(SIGHUP,  handleInterrupt);
  signal(SIGSEGV, handleSegFault);
}


void stopCatchingSignals() {
  signal(SIGINT,  SIG_DFL);
  signal(SIGTERM, SIG_DFL);
  signal(SIGHUP,  SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
}

//
// Put this last to minimize the amount of code affected by this #undef
//
#ifdef exit
#undef exit
#endif

void clean_exit(int status) {
  if (status != 0) {
    gdbShouldBreakHere();
  }

  cleanup_for_exit();

  delete gContext;
  gContext = nullptr;

  if (gGenInfo) {
    delete gGenInfo;
    gGenInfo = nullptr;
  }

  exit(status);
}
