#ifndef _driver_H_
#define _driver_H_

#include "chpl.h"
#include "map.h"

#include <cstdio>

class Timer;


// For versions of bison beyond 2.3, this struct and the following
// two defines should be moved back into chapel.ypp into a %code requires
// block so that YYLTYPE is near the YYLLOC_DEFAULT definition.
typedef struct YYLTYPE {    
       int first_line;      
       int first_column;    
       int last_line;       
       int last_column;     
       char *comment;       
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1

extern const char *chplBinaryName;

extern int  instantiation_limit;

// optimization control flags
extern bool fFastFlag;
extern int  fConditionalDynamicDispatchLimit;
extern bool fNoBoundsChecks;
extern bool fNoCopyPropagation;
extern bool fNoDeadCodeElimination;
extern bool fNoGlobalConstOpt;
extern bool fNoFastFollowers;
extern bool fNoInlineIterators;
extern bool fNoloopInvariantCodeMotion;
extern bool fNoInline;
extern bool fNoLiveAnalysis;
extern bool fNoLocalChecks;
extern bool fNoNilChecks;
extern bool fNoStackChecks;
extern bool fLLVMWideOpt;

extern bool fNoRemoteValueForwarding;
extern bool fNoRemoveCopyCalls;
extern bool fNoScalarReplacement;
extern bool fNoTupleCopyOpt;
extern bool fNoOptimizeLoopIterators;
extern bool fNoPrivatization;
extern bool fNoOptimizeOnClauses;
extern bool fNoRemoveEmptyRecords;
extern int  optimize_on_clause_limit;
extern int  scalar_replace_limit;
extern int  tuple_copy_limit;

extern bool report_inlining;
extern char CHPL_HOME[FILENAME_MAX+1];

extern const char* CHPL_HOST_PLATFORM;
extern const char* CHPL_HOST_COMPILER;
extern const char* CHPL_TARGET_PLATFORM;
extern const char* CHPL_TARGET_COMPILER;
extern const char* CHPL_TARGET_ARCH;
extern const char* CHPL_LOCALE_MODEL;
extern const char* CHPL_COMM;
extern const char* CHPL_COMM_SUBSTRATE;
extern const char* CHPL_GASNET_SEGMENT;
extern const char* CHPL_TASKS;
extern const char* CHPL_THREADS;
extern const char* CHPL_LAUNCHER;
extern const char* CHPL_TIMERS;
extern const char* CHPL_MEM;
extern const char* CHPL_MAKE;
extern const char* CHPL_ATOMICS;
extern const char* CHPL_NETWORK_ATOMICS;
extern const char* CHPL_GMP;
extern const char* CHPL_HWLOC;
extern const char* CHPL_REGEXP;
extern const char* CHPL_WIDE_POINTERS;
extern const char* CHPL_LLVM;
extern const char* CHPL_AUX_FILESYS;
extern int num_chpl_env_vars;
extern const char* chpl_env_vars[];
extern const char* chpl_env_var_names[];

extern bool  printPasses;
extern FILE* printPassesFile; 

// Set true if CHPL_WIDE_POINTERS==struct.
// In that case, the code generator emits structures
// for wide pointers. Otherwise, wide pointers are
// packed into a wide pointer type.
extern bool widePointersStruct;

extern char fExplainCall[256];
extern int  explainCallID;
extern char fExplainInstantiation[256];
/// If true, then print additional (disambiguation) information about
/// resolution.
extern bool fExplainVerbose;
extern bool fPrintCallStackOnError;
extern bool fPrintIDonError;
extern bool fPrintModuleResolution;
extern bool fCLineNumbers;
extern bool fPrintEmittedCodeSize;
extern char fPrintStatistics[256];
extern bool fPrintDispatch;
extern bool fGenIDS;
extern bool fLocal;
extern bool fHeterogeneous;
extern bool fieeefloat;
extern int  fMaxCIdentLen;

extern bool llvmCodegen;

// Is the cache for remote data enabled?
extern bool fCacheRemote;

// externC allows blocks like extern { } to be parsed
// with clang and then added to the enclosing module's scope
extern bool externC;
extern char breakOnCodegenCname[256];

enum { LS_DEFAULT=0, LS_STATIC, LS_DYNAMIC };

extern int  fLinkStyle;

extern int  debugParserLevel;
extern int  debugShortLoc;
extern bool fLibraryCompile;
extern bool no_codegen;
extern bool developer;
extern bool fVerify;
extern int  num_constants_per_variable;
extern bool printCppLineno;

extern bool fDocs;
extern bool fDocsAlphabetize;
extern char fDocsCommentLabel[256];
extern char fDocsFolder[256];
extern bool fDocsTextOnly;
extern char defaultDist[256];
extern char mainModuleName[256];
extern bool printSearchDirs;
extern bool printModuleFiles;
extern bool ignore_warnings;
extern bool ignore_errors;
extern bool ignore_errors_for_pass;
extern int  squelch_header_errors;
extern bool fWarnConstLoops;

extern bool fReportOptimizedLoopIterators;
extern bool fReportOptimizedOn;
extern bool fReportPromotion;
extern bool fReportScalarReplace;
extern bool fReportDeadBlocks;
extern bool fReportDeadModules;

extern bool debugCCode, optimizeCCode, specializeCCode;

extern bool fEnableTimers;
extern Timer timer1;
extern Timer timer2;
extern Timer timer3;
extern Timer timer4;
extern Timer timer5;

extern bool fNoMemoryFrees;
extern int numGlobalsOnHeap;
extern bool preserveInlinedLineNumbers;

extern int breakOnID;
extern int breakOnDeleteID;

// code generation strings
extern const char* compileCommand;
extern char compileVersion[64];

// This flag is useful for testing
// the compiler but breaks the language!
extern bool fMinimalModules;

#endif
