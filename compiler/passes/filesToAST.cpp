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


void FilesToAST::run(ModuleSymbol* moduleList) {
  // parse internal prelude
  Symboltable::parseInternalPrelude();
  char* preludePath = glomstrings(2, system_dir, 
				  "/parser/internal_prelude.chpl");
  internalPrelude = ParseFile(preludePath, true);
  findInternalTypes();

  // parse prelude
  Symboltable::parsePrelude();
  preludePath = glomstrings(2, system_dir, "/parser/prelude.chpl");
  prelude = ParseFile(preludePath, true);

  // parse user files
  Symboltable::doneParsingPreludes();

  yydebug = debugParserLevel;

  int filenum = 0;
  char* inputFilename = NULL;

  while (inputFilename = nthFilename(filenum++)) {
    ParseFile(inputFilename);
  }

  Symboltable::doneParsingUserFiles();
}
