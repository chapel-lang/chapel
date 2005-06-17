#include "driver.h"
#include "files.h"
#include "filesToAST.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "yy.h"

ModuleSymbol* internalPrelude = NULL;
ModuleSymbol* prelude = NULL;
ModuleSymbol* seqModule = NULL;

void FilesToAST::run(ModuleList* moduleList) {
  // parse internal prelude
  Symboltable::parseInternalPrelude();
  char* chplroot = sysdirToChplRoot(system_dir);
  char* parserPath = glomstrings(2, chplroot, "/compiler/parser");
    
  char* preludePath = glomstrings(2, parserPath, "/internal_prelude.chpl");
  internalPrelude = ParseFile(preludePath, MOD_INTERNAL);
  //Type::findInternalType -> Symboltable::lookupXxxType

  // parse prelude
  Symboltable::parsePrelude();
  preludePath = glomstrings(2, parserPath, "/prelude.chpl");
  //parser.cpp: -> Symboltable::Start/FinishModuleDefinition
  //parser.cpp: yyparse()
  prelude = ParseFile(preludePath, MOD_INTERNAL);

  findInternalTypes();

  // parse user files
  Symboltable::doneParsingPreludes();

  yydebug = debugParserLevel;

  ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_complex.chpl"),
            MOD_STANDARD);

  if (analyzeAST) {
    seqModule = ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_seq.chpl"),
                          MOD_STANDARD);
    dtSequence = Symboltable::lookupInternalType("_seq")->type;
    //    dtSequence = Symboltable::lookupInScope("seq", seqModule->modScope)->type;
  }

  int filenum = 0;
  char* inputFilename = NULL;

  while (inputFilename = nthFilename(filenum++)) {
    ParseFile(inputFilename, MOD_USER);
  }

  Symboltable::doneParsingUserFiles();
}
