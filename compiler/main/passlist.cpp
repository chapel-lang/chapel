#include "passes.h"

#include "../passes/applyGettersSetters.h"
#include "../passes/buildDefaultFunctions.h"
#include "../passes/codegen.h"
#include "../passes/createEntryPoint.h"
#include "../passes/filesToAST.h"
#include "../passes/functionResolution.h"
#include "../passes/preAnalysisCleanup.h"
#include "../passes/printProgram.h"
#include "../passes/runAnalysis.h"
#include "../passes/runInterpreter.h"

#include "../symtab/codegenOne.h"
#include "../symtab/copyPropagation.h"
#include "../symtab/printSymtab.h"
#include "../symtab/resolveTypes.h"

#include "../traversals/buildClassHierarchy.h"
#include "../traversals/createConfigVarTable.h"
#include "../traversals/createNestedFuncIterators.h"
#include "../traversals/findUnknownTypes.h"
#include "../traversals/fixup.h"
#include "../traversals/getstuff.h"
#include "../traversals/htmlview.h"
#include "../traversals/inlineFunctions.h"
#include "../traversals/insertFunctionTemps.h"
#include "../traversals/insertVariableInitializations.h"
#include "../traversals/instantiate.h"
#include "../traversals/printAST.h"
#include "../traversals/processImportExprs.h"
#include "../traversals/processParameters.h"
#include "../traversals/removeNamedParameters.h"
#include "../traversals/removeNestedFunctions.h"
#include "../traversals/resolveSymbols.h"
#include "../traversals/transformLetExprs.h"
#include "../traversals/verify.h"
#include "../traversals/view.h"

/* any Passes or Traversals that are expected to be parsed as part of
   a command-line specified passlist.h file must be listed here in the
   format shown.  Make sure to also #include the header file defining
   the pass above.  */

START_PASSLIST_REGISTRATION
REGISTER(applyGettersSetters);
REGISTER(buildClassHierarchy);
REGISTER(buildDefaultFunctions);
REGISTER(check_parsed);
REGISTER(check_normalized);
REGISTER(check_resolved);
REGISTER(cleanup);
REGISTER(codegen);
REGISTER(codegenOne);
REGISTER(copyPropagation);
REGISTER(createConfigVarTable);
REGISTER(createEntryPoint);
REGISTER(createNestedFuncIterators);
REGISTER(fixup);
REGISTER(functionResolution);
REGISTER(getStuff);
REGISTER(htmlView);
REGISTER(inlineFunctions);
REGISTER(insertFunctionTemps);
REGISTER(insertVariableInitializations);
REGISTER(normalize);
REGISTER(parse);
REGISTER(passlistTest);
REGISTER(preAnalysisCleanup);
REGISTER(pre_instantiate);
REGISTER(printAST);        // BLC: pretty-prints all or part of the AST
REGISTER(printProgram);    // BLC: pretty-prints the whole program
REGISTER(printSymtab);
REGISTER(processImportExprs);
REGISTER(processParameters); // BLC: handle parameter intents
REGISTER(removeNamedParameters);
REGISTER(removeNestedFunctions);
REGISTER(removeTypeVariableActuals);
REGISTER(removeTypeVariableFormals);
REGISTER(resolve_analyzed);
REGISTER(resolveSymbols);       // SJD: Resolve symbols after analysis
REGISTER(resolveTypes);
REGISTER(runAnalysis);
REGISTER(runInterpreter);
REGISTER(scopeResolve);
REGISTER(transformLetExprs);
REGISTER(verify);
REGISTER(view);
STOP_PASSLIST_REGISTRATION
