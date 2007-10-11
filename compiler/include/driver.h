/* -*-Mode: c++;-*-
*/
#ifndef _driver_H_
#define _driver_H_

#include <stdio.h>
#include "map.h"
#include "chpl.h"

extern FILE* html_index_file;

extern Map<const char*, const char*> configParamMap;

extern int instantiation_limit;

extern int fdump_html;
extern int fNoStdIncs;

extern int trace_level;

// optimization control flags
extern bool fBaseline;
extern bool fNoBoundsChecks;
extern bool fNoCopyPropagation;
extern bool fNoExpandIteratorsInlineOpt;
extern bool fNoFlowAnalysis;
extern bool fNoInline;
extern bool fNoLiveVariablesIteratorClassOpt;
extern bool fNoScalarReplaceArrayWrappers;
extern bool fNoScalarReplacement;
extern bool fNoSingleLoopIteratorOpt;

extern bool report_inlining;
extern char chplhome[FILENAME_MAX];
extern char chplmake[256];
extern char fExplainCall[256];

extern char fPrintStatistics[256];
extern bool fPrintDispatch;
extern bool fShortNames;
extern bool fSerial;
extern bool fLocal;

extern int debugParserLevel;
extern bool no_codegen;
extern bool developer;
extern int num_constants_per_variable;
extern bool printCppLineno;

extern char defaultDistribution[256];
extern char mainModuleName[256];
extern bool ignore_warnings;
extern bool ignore_errors;
extern int squelch_header_errors;

extern bool fWarnPromotion;
extern bool fCopyCollect;

extern bool debugCCode, optimizeCCode;

extern bool fEnableTimers;
extern Timer timer1;
extern Timer timer2;
extern Timer timer3;
extern Timer timer4;
extern Timer timer5;

#endif
