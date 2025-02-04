/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _driver_H_
#define _driver_H_

#include "arg.h"
#include "chpl.h"
#include "map.h"

#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <unordered_set>

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
extern bool fIgnoreNilabilityErrors;
extern bool fOverloadSetsChecks;
extern bool fNoStackChecks;
extern bool fNoCastChecks;
extern bool fNoConstArgChecks;
extern bool fNoDivZeroChecks;
extern bool fMungeUserIdents;
extern bool fEnableTaskTracking;
extern bool fEnableMemInterleaving;
extern bool fLLVMWideOpt;

extern bool fAutoLocalAccess;
extern bool fDynamicAutoLocalAccess;
extern bool fOffsetAutoLocalAccess;
extern bool fReportAutoLocalAccess;

extern bool fAutoAggregation;
extern bool fReportAutoAggregation;

extern bool fArrayViewElision;
extern bool fReportArrayViewElision;

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
bool useDefaultEnv(std::string key, bool isCrayPrgEnv);

extern std::map<std::string, const char*> envMap;

extern std::string CHPL_HOME;
extern std::string CHPL_RUNTIME_LIB;
extern std::string CHPL_RUNTIME_INCL;
extern std::string CHPL_THIRD_PARTY;

extern const char* CHPL_HOST_PLATFORM;
extern const char* CHPL_HOST_ARCH;
extern const char* CHPL_HOST_COMPILER;
extern const char* CHPL_TARGET_PLATFORM;
extern const char* CHPL_TARGET_ARCH;
extern const char* CHPL_TARGET_CPU;
extern const char* CHPL_RUNTIME_CPU;
extern const char* CHPL_LLVM_TARGET_CPU;
extern const char* CHPL_TARGET_CPU_FLAG;
extern const char* CHPL_TARGET_COMPILER;
extern const char* CHPL_TARGET_COMPILER_PRGENV;
extern const char* CHPL_LOCALE_MODEL;
extern const char* CHPL_COMM;
extern const char* CHPL_COMM_SUBSTRATE;
extern const char* CHPL_GASNET_SEGMENT;
extern const char* CHPL_LIBFABRIC;
extern const char* CHPL_COMM_OFI_OOB;
extern const char* CHPL_TASKS;
extern const char* CHPL_LAUNCHER;
extern const char* CHPL_TIMERS;
extern const char* CHPL_TARGET_MEM;
extern const char* CHPL_MAKE;
extern const char* CHPL_ATOMICS;
extern const char* CHPL_NETWORK_ATOMICS;
extern const char* CHPL_GMP;
extern const char* CHPL_HWLOC;
extern const char* CHPL_RE2;
extern const char* CHPL_LLVM;
extern const char* CHPL_AUX_FILESYS;
extern const char* CHPL_UNWIND;
extern const char* CHPL_LIB_PIC;
extern const char* CHPL_RUNTIME_SUBDIR;
extern const char* CHPL_LAUNCHER_SUBDIR;
extern const char* CHPL_SYS_MODULES_SUBDIR;
extern const char* CHPL_LLVM_UNIQ_CFG_PATH;
extern const char* CHPL_LLVM_CLANG_C;
extern const char* CHPL_LLVM_CLANG_CXX;

extern const char* CHPL_TARGET_BUNDLED_COMPILE_ARGS;
extern const char* CHPL_TARGET_SYSTEM_COMPILE_ARGS;
extern const char* CHPL_TARGET_LD;
extern const char* CHPL_TARGET_BUNDLED_LINK_ARGS;
extern const char* CHPL_TARGET_SYSTEM_LINK_ARGS;

extern const char* CHPL_CUDA_LIBDEVICE_PATH;
extern const char* CHPL_ROCM_LLVM_PATH;
extern const char* CHPL_ROCM_AMDGCN_PATH;
extern const char* CHPL_GPU;
extern const char* CHPL_GPU_ARCH;

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
// begin compiler driver control flags
extern bool fDriverDoMonolithic;
extern bool fDriverCompilationPhase;
extern bool fDriverMakeBinaryPhase;
extern std::string driverTmpDir;
// end compiler driver control flags
extern bool fExitLeaks;
extern bool fPrintAllCandidates;
extern bool fPrintCallGraph;
extern bool fPrintCallStackOnError;
extern bool fAutoPrintCallStackOnError;
extern bool fPrintIDonError;
extern bool fPrintModuleResolution;
extern bool fPrintEmittedCodeSize;
extern char fPrintStatistics[256];
extern bool fPrintDispatch;
extern bool fPrintUnusedFns;
extern bool fPrintUnusedInternalFns;
extern bool fRegionVectorizer;
extern bool fDetectColorTerminal;
extern bool fUseColorTerminal;
extern bool fGenIDS;
extern bool fLocal;
extern bool fIgnoreLocalClasses;
extern bool fAllowNoinitArrayNotPod;
extern bool fNoLifetimeChecking;
extern bool fNoSplitInit;
extern bool fNoEarlyDeinit;
extern bool fNoCopyElision;
extern bool fCompileTimeNilChecking;
extern bool fInferImplementsStmts;
extern bool fIteratorContexts;
extern bool fReturnByRef;
extern bool fOverrideChecking;
extern int  ffloatOpt;
extern int  fMaxCIdentLen;

extern bool fLlvmCodegen;

// Is the cache for remote data enabled?
extern bool fCacheRemote;

// externC allows blocks like extern { } to be parsed
// with clang and then added to the enclosing module's scope
extern bool fAllowExternC;
extern char breakOnCodegenCname[256];
extern int breakOnCodegenID;

enum { LS_DEFAULT=0, LS_STATIC, LS_DYNAMIC };

extern int  fLinkStyle;

extern int  debugParserLevel;
extern int  debugShortLoc;
extern bool fLibraryCompile;
extern bool fLibraryFortran;
extern bool fLibraryMakefile;
extern bool fLibraryCMakeLists;
extern bool fLibraryPython;

extern bool fMultiLocaleInterop;
extern bool fMultiLocaleLibraryDebug;

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

extern bool fWarnIntUint;
extern bool fWarnSmallIntegralFloat;
extern bool fWarnIntegralFloat;
extern bool fWarnFloatFloat;
extern bool fWarnIntegralIntegral;
extern bool fWarnImplicitNumericConversions;
extern bool fWarnParamImplicitNumericConversions;

extern bool fWarnConstLoops;
extern bool fWarnUnstable;
extern bool fWarnUnstableStandard;
extern bool fWarnUnstableInternal;
extern bool fWarnPotentialRaces;

extern bool fReportAliases;
extern bool fReportBlocking;
extern bool fReportOptimizedLoopIterators;
extern bool fReportInlinedIterators;
extern bool fReportVectorizedLoops;
extern bool fReportOptimizedOn;
extern bool fReportPromotion;
extern bool fReportScalarReplace;
extern bool fReportGpu;
extern bool fReportContextAdj;
extern bool fReportDeadBlocks;
extern bool fReportDeadModules;
extern bool fReportGpuTransformTime;

extern bool fPermitUnhandledModuleErrors;

extern bool debugCCode;
extern bool optimizeCCode;
extern bool specializeCCode;

extern bool fNoMemoryFrees;
extern int  numGlobalsOnHeap;
extern bool preserveInlinedLineNumbers;

extern int breakOnID;
extern int breakOnRemoveID;

extern int fGPUBlockSize;
const int gpuArchNameLen = 256;
extern char fGpuArch[gpuArchNameLen+1];
extern bool fGpuPtxasEnforceOpt;
extern bool fGpuSpecialization;
extern const char* gGpuSdkPath;
extern std::set<std::string> gpuArches;

extern char stopAfterPass[128];

// code generation strings
extern const char* compileCommandFilename;
extern const char* compileCommand;
extern char compileVersion[64];

// This flag is useful for testing
// the compiler but breaks the language!
extern bool fMinimalModules;

// This flag sets the make -j value
// <=0 == don't use -j
//        (in the future, we may want to make < 0 mean "#cores")
//  >0 == make -j <val>
extern int fParMake;

// Set to true if we want to enable incremental compilation.
extern bool fIncrementalCompilation;

// LLVM flags (-mllvm)
extern std::string llvmFlags;
extern std::string llvmRemarksFilters;
extern std::vector<std::string> llvmRemarksFunctionsToShow;
extern bool fLlvmPrintPasses;

extern bool fPrintAdditionalErrors;

extern bool fDynoResolver;
extern bool fDynoResolveOnly;
extern bool fDynoScopeResolve;
extern bool fDynoScopeProduction;
extern bool fDynoScopeBundled;
extern bool fDynoDebugTrace;
extern bool fDynoDebugPrintParsedFiles;
extern bool fDynoVerifySerialization;
extern bool fDynoGenLib;
extern bool fDynoGenStdLib;
extern bool fDynoLibGenOrUse;

extern size_t fDynoBreakOnHash;
extern bool   fDynoBreakOnHashSet;

extern bool fResolveConcreteFns;
extern bool fIdBasedMunging;

extern bool fNoIOGenSerialization;
extern bool fNoIOSerializeWriteThis;
extern bool fNoIODeserializeReadThis;

namespace chpl {
  class Context;
}

extern chpl::Context* gContext;

extern std::vector<std::pair<std::string, std::string>> gDynoParams;

extern std::vector<std::string> gDynoPrependInternalModulePaths;
extern std::vector<std::string> gDynoPrependStandardModulePaths;

extern std::string gDynoGenLibOutput;
extern std::vector<UniqueString> gDynoGenLibSourcePaths;
extern std::unordered_set<const char*> gDynoGenLibModuleNameAstrs;

extern std::string gMainModuleName;

extern bool fForeachIntents;

#endif
