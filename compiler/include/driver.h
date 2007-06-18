/* -*-Mode: c++;-*-
*/
#ifndef _driver_H_
#define _driver_H_

#include <stdio.h>
#include "map.h"

extern FILE* html_index_file;

extern Map<char*, char*> configParamMap;

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
extern bool fNoSingleLoopIteratorOpt;

extern bool report_inlining;
extern char chplhome[FILENAME_MAX];
extern char fExplainCall[256];

extern char fPrintStatistics[256];
extern bool fPrintDispatch;

extern int debugParserLevel;
extern bool no_codegen;
extern bool developer;
extern int num_constants_per_variable;

extern char defaultDistribution[256];

extern bool ignore_warnings;
extern bool ignore_errors;
extern int squelch_header_errors;

extern bool fWarnPromotion;
extern bool copyCollect;
extern bool parallelPass;

extern bool debugCCode, optimizeCCode;

#endif
