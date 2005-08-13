#include "driver.h"
#include "files.h"
#include "filesToAST.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "countTokens.h"
#include "yy.h"

ModuleSymbol* prelude = NULL;
ModuleSymbol* seqModule = NULL;

void FilesToAST::run(Vec<ModuleSymbol*>* modules) {
  // parse prelude
  Symboltable::parsePrelude();
  char* chplroot = sysdirToChplRoot(system_dir);
  char* parserPath = glomstrings(2, chplroot, "/compiler/parser");
  char* preludePath = glomstrings(2, parserPath, "/prelude.chpl");
  prelude = ParseFile(preludePath, MOD_INTERNAL);

  findInternalTypes();

  // parse user files
  Symboltable::doneParsingPreludes();

  yydebug = debugParserLevel;

  ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_complex.chpl"),
            MOD_STANDARD);
  ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_file.chpl"),
            MOD_STANDARD);
  ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_htuple.chpl"),
            MOD_STANDARD);

  seqModule = ParseFile(glomstrings(2, chplroot, "/modules/standard/_chpl_seq.chpl"),
                        MOD_STANDARD);
  dtSequence = Symboltable::lookupInternalType("_seq")->type;

  int filenum = 0;
  char* inputFilename = NULL;

  while (inputFilename = nthFilename(filenum++)) {
    ParseFile(inputFilename, MOD_USER);
  }
  finishCountingTokens();

  Symboltable::doneParsingUserFiles();
}
