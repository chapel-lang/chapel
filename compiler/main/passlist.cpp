/* any Passes or Traversals that are expected to be parsed as part of
   a passlist.h file must be listed here in the format shown.  Make
   sure to also #include the header file defining the pass in
   passlist.h.  */

REGISTER(CheckIDs);
REGISTER(CheckTypeInfo);
REGISTER(DummyPass);
REGISTER(FindUnknownTypes);
REGISTER(GetStuff);
REGISTER(PrintAST);        // BLC: pretty-prints all or part of the AST
REGISTER(PrintProgram);    // BLC: pretty-prints the whole program
REGISTER(PrintStmts);
REGISTER(PrintSymtab);     // BLC: prints out the whole symboltable
REGISTER(RunAnalysis);
REGISTER(SetupStmtLinks);
REGISTER(TestGetStuff);
REGISTER(VerifyASTType);

