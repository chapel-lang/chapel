#include "../passes/buildBinary.h"
#include "../passes/codegen.h"
#include "../passes/createEntryPoint.h"
#include "../passes/filesToAST.h"
#include "../passes/printProgram.h"
#include "../passes/renameCSymbols.h"
#include "../passes/runAnalysis.h"

#include "../symtab/printSymtab.h"
#include "../symtab/legalizeCNames.h"

#include "../traversals/checkIDs.h"
#include "../traversals/checkSemantics.h"
#include "../traversals/checkTypeInfo.h"
#include "../traversals/cleanup.h"
#include "../traversals/cloneAllFns.h"
#include "../traversals/createConfigVarTable.h"
#include "../traversals/findUnknownTypes.h"
#include "../traversals/findUnresolvedSymbols.h"
#include "../traversals/fixup.h"
#include "../traversals/getstuff.h"
#include "../traversals/insertUnionChecks.h"
#include "../traversals/methodsToFunctions.h"
#include "../traversals/printAST.h"
#include "../traversals/processParameters.h"
#include "../traversals/resolveSymbols.h"
#include "../traversals/testGetStuff.h"
#include "../traversals/verifyASTType.h"

/* any Passes or Traversals that are expected to be parsed as part of
   a command-line specified passlist.h file must be listed here in the
   format shown.  Make sure to also #include the header file defining
   the pass above.  */

START_PASSLIST_REGISTRATION
REGISTER(BuildBinary);
REGISTER(CheckIDs);
REGISTER(CheckSemantics);
REGISTER(CheckTypeInfo);
REGISTER(Cleanup);                // SJD: Post-parsing cleanup, e.g. resolve symbols, insert this
REGISTER(CloneAllFns);
REGISTER(Codegen);
REGISTER(CreateConfigVarTable);
REGISTER(CreateEntryPoint);
REGISTER(DummyPass);
REGISTER(FindUnknownTypes);
REGISTER(FindUnresolvedSymbols);
REGISTER(FilesToAST);
REGISTER(Fixup);
REGISTER(GetStuff);
REGISTER(InsertUnionChecks);     // SJD: Insert runtime type checks for unions
REGISTER(LegalizeCNames);
REGISTER(MethodsToFunctions);    // SJD: Convert methods to functions
REGISTER(PrintAST);        // BLC: pretty-prints all or part of the AST
REGISTER(PrintProgram);    // BLC: pretty-prints the whole program
REGISTER(PrintSymtab);
REGISTER(ProcessParameters); // BLC: handle parameter intents
REGISTER(RemoveTypeVariableActuals);
REGISTER(RemoveTypeVariableFormals);
REGISTER(RenameCSymbols);  // BLC: rename symbols for C codegen
REGISTER(ResolveSymbols);       // SJD: Resolve symbols after analysis
REGISTER(RunAnalysis);
REGISTER(TestGetStuff);
REGISTER(VerifyASTType);
STOP_PASSLIST_REGISTRATION
