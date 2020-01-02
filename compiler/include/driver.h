/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _driver_H_
#define _driver_H_

#include "chpl.h"
#include "map.h"

#include <cstdio>
#include <map>
#include <string>

class Timer;

extern int  instantiation_limit;

// optimization control flags
extern bool fFastFlag;
extern bool fNoBoundsChecks;
extern bool fNoCopyPropagation;
extern bool fNoDeadCodeElimination;
extern bool fNoGlobalConstOpt;
extern bool fNoFastFollowers;
extern bool fNoInlineIterators;
extern bool fNoLoopInvariantCodeMotion;
extern bool fNoInterproceduralAliasAnalysis;
extern bool fNoInline;
extern bool fNoLiveAnalysis;
extern bool fNoFormalDomainChecks;
extern bool fNoLocalChecks;
extern bool fNoNilChecks;
extern bool fLegacyClasses;
extern bool fIgnoreNilabilityErrors;
extern bool fOverloadSetsChecks;
extern bool fNoStackChecks;
extern bool fNoCastChecks;
extern bool fNoDivZeroChecks;
extern bool fMungeUserIdents;
extern bool fEnableTaskTracking;
extern bool fLLVMWideOpt;

extern bool fNoRemoteValueForwarding;
extern bool fNoInferConstRefs;
extern bool fNoRemoteSerialization;
extern bool fNoRemoveCopyCalls;
extern bool fNoScalarReplacement;
extern bool fNoTupleCopyOpt;
extern bool fNoOptimizeRangeIteration;
extern bool fNoOptimizeLoopIterators;
extern bool fNoVectorize;
extern bool fForceVectorize;
extern bool fNoPrivatization;
extern bool fNoOptimizeOnClauses;
extern bool fNoRemoveEmptyRecords;
extern bool fNoInferLocalFields;
extern bool fRemoveUnreachableBlocks;
extern bool fReplaceArrayAccessesWithRefTemps;
extern int  optimize_on_clause_limit;
extern int  scalar_replace_limit;
extern int  inline_iter_yield_limit;
extern int  tuple_copy_limit;

extern bool fNoOptimizeForallUnordered;
extern bool fReportOptimizeForallUnordered;

extern bool report_inlining;

// Chapel Envs
bool useDefaultEnv(std::string key);

extern std::map<std::string, const char*> envMap;

extern char CHPL_HOME[FILENAME_MAX+1];
extern char CHPL_RUNTIME_LIB[FILENAME_MAX+1];
extern char CHPL_RUNTIME_INCL[FILENAME_MAX+1];
extern char CHPL_THIRD_PARTY[FILENAME_MAX+1];

extern const char* CHPL_HOST_PLATFORM;
extern const char* CHPL_HOST_ARCH;
extern const char* CHPL_HOST_COMPILER;
extern const char* CHPL_TARGET_PLATFORM;
extern const char* CHPL_TARGET_ARCH;
extern const char* CHPL_TARGET_CPU;
extern const char* CHPL_RUNTIME_CPU;
extern const char* CHPL_TARGET_BACKEND_CPU;
extern const char* CHPL_TARGET_CPU_FLAG;
extern const char* CHPL_TARGET_COMPILER;
extern const char* CHPL_ORIG_TARGET_COMPILER;
extern const char* CHPL_LOCALE_MODEL;
extern const char* CHPL_COMM;
extern const char* CHPL_COMM_SUBSTRATE;
extern const char* CHPL_GASNET_SEGMENT;
extern const char* CHPL_TASKS;
extern const char* CHPL_LAUNCHER;
extern const char* CHPL_TIMERS;
extern const char* CHPL_MEM;
extern const char* CHPL_MAKE;
extern const char* CHPL_ATOMICS;
extern const char* CHPL_NETWORK_ATOMICS;
extern const char* CHPL_GMP;
extern const char* CHPL_HWLOC;
extern const char* CHPL_REGEXP;
extern const char* CHPL_LLVM;
extern const char* CHPL_AUX_FILESYS;
extern const char* CHPL_UNWIND;
extern const char* CHPL_LIB_PIC;
extern const char* CHPL_RUNTIME_SUBDIR;
extern const char* CHPL_LAUNCHER_SUBDIR;
extern const char* CHPL_LLVM_UNIQ_CFG_PATH;

extern bool  printPasses;
extern FILE* printPassesFile;

extern char fExplainCall[256];
extern int  explainCallID;
extern int  breakOnResolveID;
extern bool fDenormalize;
extern char fExplainInstantiation[256];
/// If true, then print additional (disambiguation) information about
/// resolution.
extern bool fExplainVerbose;
extern bool fParseOnly;
extern bool fPrintAllCandidates;
extern bool fPrintCallGraph;
extern bool fPrintCallStackOnError;
extern bool fPrintIDonError;
extern bool fPrintModuleResolution;
extern bool fPrintEmittedCodeSize;
extern char fPrintStatistics[256];
extern bool fPrintDispatch;
extern bool fPrintUnusedFns;
extern bool fPrintUnusedInternalFns;
extern bool fRegionVectorizer;
extern bool fGenIDS;
extern bool fLocal;
extern bool fIgnoreLocalClasses;
extern bool fLifetimeChecking;
extern bool fCompileTimeNilChecking;
extern bool fOverrideChecking;
extern int  ffloatOpt;
extern int  fMaxCIdentLen;

extern bool llvmCodegen;

// Is the cache for remote data enabled?
extern bool fCacheRemote;

// externC allows blocks like extern { } to be parsed
// with clang and then added to the enclosing module's scope
extern bool externC;
extern char breakOnCodegenCname[256];
extern int breakOnCodegenID;

enum { LS_DEFAULT=0, LS_STATIC, LS_DYNAMIC };

extern int  fLinkStyle;

extern int  debugParserLevel;
extern int  debugShortLoc;
extern bool fLibraryCompile;
extern bool fLibraryFortran;
extern bool fLibraryMakefile;
extern bool fLibraryPython;

extern bool fMultiLocaleInterop;
extern bool fMultiLocaleLibraryDebug;

extern bool fUseNoinit;
extern bool no_codegen;
extern bool developer;
extern bool fVerify;
extern int  num_constants_per_variable;
extern bool printCppLineno;

extern char defaultDist[256];
extern bool printSearchDirs;
extern bool printModuleFiles;
extern bool ignore_warnings;
extern bool ignore_errors;
extern bool ignore_user_errors;
extern bool ignore_errors_for_pass;
extern int  squelch_header_errors;
extern bool fWarnConstLoops;
extern bool fWarnUnstable;

extern bool fReportAliases;
extern bool fReportBlocking;
extern bool fReportExpiring;
extern bool fReportOptimizedLoopIterators;
extern bool fReportInlinedIterators;
extern bool fReportVectorizedLoops;
extern bool fReportOptimizedOn;
extern bool fReportPromotion;
extern bool fReportScalarReplace;
extern bool fReportDeadBlocks;
extern bool fReportDeadModules;

extern bool fPermitUnhandledModuleErrors;

extern bool debugCCode;
extern bool optimizeCCode;
extern bool specializeCCode;

extern bool fNoMemoryFrees;
extern int  numGlobalsOnHeap;
extern bool preserveInlinedLineNumbers;

extern int breakOnID;
extern int breakOnRemoveID;

extern char stopAfterPass[128];

// code generation strings
extern const char* compileCommand;
extern char compileVersion[64];

// This flag is useful for testing
// the compiler but breaks the language!
extern bool fMinimalModules;

// Set to true if we want to enable incremental compilation.
extern bool fIncrementalCompilation;

// LLVM flags (-mllvm)
extern std::string llvmFlags;

#endif
