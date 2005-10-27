#include "../passes/applyGettersSetters.h"
#include "../passes/buildBinary.h"
#include "../passes/buildDefaultFunctions.h"
#include "../passes/codegen.h"
#include "../passes/createEntryPoint.h"
#include "../passes/filesToAST.h"
#include "../passes/functionResolution.h"
#include "../passes/normalizeParsedAST.h"
#include "../passes/preAnalysisCleanup.h"
#include "../passes/printProgram.h"
#include "../passes/renameCSymbols.h"
#include "../passes/runAnalysis.h"
#include "../passes/semanticChecks.h"

#include "../symtab/codegenOne.h"
#include "../symtab/copyPropagation.h"
#include "../symtab/flatten.h"
#include "../symtab/insertDefaultInitVariables.h"
#include "../symtab/insertOutParameterInitializations.h"
#include "../symtab/normalizeFunctions.h"
#include "../symtab/postAnalysisCleanup.h"
#include "../symtab/printSymtab.h"
#include "../symtab/reconstructIterators.h"
#include "../symtab/removeDeadSymbols.h"
#include "../symtab/resolveTypes.h"

#include "../traversals/applyThisParameters.h"
#include "../traversals/buildClassHierarchy.h"
#include "../traversals/checkIDs.h"
#include "../traversals/checkTypeInfo.h"
#include "../traversals/createConfigVarTable.h"
#include "../traversals/createNestedFuncIterators.h"
#include "../traversals/buildLValueFunctions.h"
#include "../traversals/eliminateReturns.h"
#include "../traversals/findUnknownTypes.h"
#include "../traversals/findUnresolvedSymbols.h"
#include "../traversals/fixup.h"
#include "../traversals/getstuff.h"
#include "../traversals/htmlview.h"
#include "../traversals/inlineFunctions.h"
#include "../traversals/insertFunctionTemps.h"
#include "../traversals/insertLiteralTemps.h"
#include "../traversals/insertThisParameters.h"
#include "../traversals/insertVariableInitializations.h"
#include "../traversals/instantiate.h"
#include "../traversals/preAnalysisHacks.h"
#include "../traversals/printAST.h"
#include "../traversals/processImportExprs.h"
#include "../traversals/processParameters.h"
#include "../traversals/removeNamedParameters.h"
#include "../traversals/removeNestedFunctions.h"
#include "../traversals/resolveSymbols.h"
#include "../traversals/scopeResolveGotos.h"
#include "../traversals/scopeResolveSymbols.h"
#include "../traversals/specializeCallExprs.h"
#include "../traversals/testGetStuff.h"
#include "../traversals/transformLetExprs.h"
#include "../traversals/verify.h"
#include "../traversals/view.h"

/* any Passes or Traversals that are expected to be parsed as part of
   a command-line specified passlist.h file must be listed here in the
   format shown.  Make sure to also #include the header file defining
   the pass above.  */

START_PASSLIST_REGISTRATION
REGISTER(ApplyGettersSetters);
REGISTER(ApplyThisParameters);
REGISTER(BuildBinary);
REGISTER(BuildClassHierarchy);
REGISTER(BuildDefaultFunctions);
REGISTER(BuildLValueFunctions);
REGISTER(CheckIDs);
REGISTER(CheckTypeInfo);
REGISTER(Codegen);
REGISTER(CodegenOne);
REGISTER(CopyPropagation);
REGISTER(CreateConfigVarTable);
REGISTER(CreateEntryPoint);
REGISTER(CreateNestedFuncIterators);
REGISTER(DummyPass);
REGISTER(EliminateReturns);
REGISTER(FindUnresolvedSymbols);
REGISTER(FilesToAST);
REGISTER(Fixup);
REGISTER(Flatten);
REGISTER(FunctionResolution);
REGISTER(GetStuff);
REGISTER(HtmlView);
REGISTER(InlineFunctions);
REGISTER(InsertDefaultInitVariables);
REGISTER(InsertFunctionTemps);
REGISTER(InsertLiteralTemps);
REGISTER(InsertOutParameterInitializations);
REGISTER(InsertThisParameters);
REGISTER(InsertVariableInitializations);
REGISTER(Instantiate);
REGISTER(NormalizeFunctions);
REGISTER(NormalizeParsedAST);
REGISTER(PostAnalysisCleanup);
REGISTER(PreAnalysisHacks);
REGISTER(PreAnalysisCleanup);
REGISTER(PrintAST);        // BLC: pretty-prints all or part of the AST
REGISTER(PrintProgram);    // BLC: pretty-prints the whole program
REGISTER(PrintSymtab);
REGISTER(ProcessImportExprs);
REGISTER(ProcessParameters); // BLC: handle parameter intents
REGISTER(ReconstructIterators);
REGISTER(RemoveDeadSymbols);
REGISTER(RemoveNamedParameters);
REGISTER(RemoveNestedFunctions);
REGISTER(RemoveTypeVariableActuals);
REGISTER(RemoveTypeVariableFormals);
REGISTER(RenameCSymbols);  // BLC: rename symbols for C codegen
REGISTER(ResolveSymbols);       // SJD: Resolve symbols after analysis
REGISTER(ResolveTypes);
REGISTER(RunAnalysis);
REGISTER(ScopeResolveGotos);
REGISTER(ScopeResolveSymbols);
REGISTER(SemanticCheckI);
REGISTER(SemanticCheckII);
REGISTER(SemanticCheckIII);
REGISTER(SpecializeCallExprs);
REGISTER(TestGetStuff);
REGISTER(TransformLetExprs);
REGISTER(Verify);
REGISTER(View);
STOP_PASSLIST_REGISTRATION
