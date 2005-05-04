#include "../passes/buildBinary.h"
#include "../passes/codegen.h"
#include "../passes/createEntryPoint.h"
#include "../passes/filesToAST.h"
#include "../passes/printProgram.h"
#include "../passes/renameCSymbols.h"
#include "../passes/runAnalysis.h"

#include "../symtab/computeVisibleFunctions.h"
#include "../symtab/insertDefaultInitVariables.h"
#include "../symtab/insertOutParameterInitializations.h"
#include "../symtab/insertWriteFns.h"
#include "../symtab/legalizeCNames.h"
#include "../symtab/printSymtab.h"
#include "../symtab/removeDeadSymbols.h"
#include "../symtab/removeLikeTypes.h"
#include "../symtab/renameOverloadedFunctions.h"
#include "../symtab/resolveTypes.h"

#include "../traversals/applyThisParameters.h"
#include "../traversals/buildClassConstructorsEtc.h"
#include "../traversals/checkIDs.h"
#include "../traversals/checkSemantics.h"
#include "../traversals/checkTypeInfo.h"
#include "../traversals/createConfigVarTable.h"
#include "../traversals/destructureTupleAssignments.h"
#include "../traversals/eliminateReturns.h"
#include "../traversals/expandClassWiths.h"
#include "../traversals/expandSeqExprAssignments.h"
#include "../traversals/findUnknownTypes.h"
#include "../traversals/findUnresolvedSymbols.h"
#include "../traversals/fixup.h"
#include "../traversals/getstuff.h"
#include "../traversals/insertIndexType.h"
#include "../traversals/insertAnonymousDomains.h"
#include "../traversals/insertAnonymousTypes.h"
#include "../traversals/insertElidedLoops.h"
#include "../traversals/insertFunctionTemps.h"
#include "../traversals/insertTemps.h"
#include "../traversals/insertThisParameters.h"
#include "../traversals/insertUnionChecks.h"
#include "../traversals/insertVariableInitializations.h"
#include "../traversals/omitForNoAnalysis.h"
#include "../traversals/printAST.h"
#include "../traversals/processParameters.h"
#include "../traversals/scopeResolveGotos.h"
#include "../traversals/scopeResolveSymbols.h"
#include "../traversals/specializeParenOpExprs.h"
#include "../traversals/removeNamedParameters.h"
#include "../traversals/removeSeqOperations.h"
#include "../traversals/resolveOverloadedOperators.h"
#include "../traversals/resolveSymbols.h"
#include "../traversals/testGetStuff.h"
#include "../traversals/transformLetExprs.h"
#include "../traversals/verifyASTType.h"

/* any Passes or Traversals that are expected to be parsed as part of
   a command-line specified passlist.h file must be listed here in the
   format shown.  Make sure to also #include the header file defining
   the pass above.  */

START_PASSLIST_REGISTRATION
REGISTER(ApplyThisParameters);
REGISTER(BuildBinary);
REGISTER(BuildClassConstructorsEtc);
REGISTER(CheckIDs);
REGISTER(CheckSemantics);
REGISTER(CheckTypeInfo);
REGISTER(Codegen);
REGISTER(ComputeVisibleFunctions);
REGISTER(CreateConfigVarTable);
REGISTER(CreateEntryPoint);
REGISTER(DestructureTupleAssignments);
REGISTER(DummyPass);
REGISTER(EliminateReturns);
REGISTER(ExpandClassWiths);
REGISTER(ExpandSeqExprAssignments);
//REGISTER(FindUnknownTypes);
REGISTER(FindUnresolvedSymbols);
REGISTER(FilesToAST);
REGISTER(Fixup);
REGISTER(GetStuff);
REGISTER(InsertIndexType);
REGISTER(InsertAnonymousDomains);
REGISTER(InsertAnonymousTypes);
REGISTER(InsertDefaultInitVariables);
REGISTER(InsertElidedLoops);
REGISTER(InsertFunctionTemps);
REGISTER(InsertOutParameterInitializations);
REGISTER(InsertTemps);
REGISTER(InsertThisParameters);
REGISTER(InsertUnionChecks);     // SJD: Insert runtime type checks for unions
REGISTER(InsertVariableInitializations);
REGISTER(InsertWriteFns);
REGISTER(LegalizeCNames);
REGISTER(OmitForNoAnalysis);
REGISTER(PrintAST);        // BLC: pretty-prints all or part of the AST
REGISTER(PrintProgram);    // BLC: pretty-prints the whole program
REGISTER(PrintSymtab);
REGISTER(ProcessParameters); // BLC: handle parameter intents
REGISTER(RemoveDeadSymbols);
REGISTER(RemoveLikeTypes);
REGISTER(RemoveNamedParameters);
REGISTER(RemoveSeqOperations);
REGISTER(RemoveTypeVariableActuals);
REGISTER(RemoveTypeVariableFormals);
REGISTER(RenameCSymbols);  // BLC: rename symbols for C codegen
REGISTER(RenameOverloadedFunctions);
REGISTER(ResolveOverloadedOperators);
REGISTER(ResolveSymbols);       // SJD: Resolve symbols after analysis
REGISTER(ResolveTypes);
REGISTER(ResolveTupleTypes);
REGISTER(RunAnalysis);
REGISTER(ScopeResolveGotos);
REGISTER(ScopeResolveSymbols);
REGISTER(SpecializeParenOpExprs);
REGISTER(TestGetStuff);
REGISTER(TransformLetExprs);
REGISTER(VerifyASTType);
STOP_PASSLIST_REGISTRATION
