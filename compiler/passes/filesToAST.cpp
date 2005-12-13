#include "driver.h"
#include "files.h"
#include "filesToAST.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "countTokens.h"
#include "yy.h"
#include "../traversals/fixup.h"
#include "runtime.h"

void parse(void) {
  // parse prelude
  Symboltable::parsePrelude();
  char* chplroot = sysdirToChplRoot(system_dir);
  prelude = ParseFile(stringcat(chplroot, "/modules/standard/prelude.chpl"),
                      MOD_INTERNAL);

  // parse user files
  Symboltable::doneParsingPreludes();

  yydebug = debugParserLevel;
  
  if (!fnostdincs) {
    ParseFile(stringcat(chplroot, "/modules/standard/_chpl_complex.chpl"),
              MOD_STANDARD);
  }
  fileModule = ParseFile(stringcat(chplroot, 
                                   "/modules/standard/_chpl_file.chpl"),
                         MOD_STANDARD);
  if (!fnostdincs) {
    tupleModule = ParseFile(stringcat(chplroot,
                                      "/modules/standard/_chpl_htuple.chpl"),
                            MOD_STANDARD);
    ParseFile(stringcat(chplroot, "/modules/standard/_chpl_adomain.chpl"),
              MOD_STANDARD);
    ParseFile(stringcat(chplroot, "/modules/standard/_chpl_data.chpl"),
              MOD_STANDARD);
    seqModule = ParseFile(stringcat(chplroot, "/modules/standard/_chpl_seq.chpl"),
                          MOD_STANDARD);
    ParseFile(stringcat(chplroot, "/modules/standard/_chpl_standard.chpl"),
              MOD_STANDARD);
  }

  int filenum = 0;
  char* inputFilename = NULL;

  while ((inputFilename = nthFilename(filenum++))) {
    ParseFile(inputFilename, MOD_USER);
  }
  finishCountingTokens();

  Symboltable::doneParsingUserFiles();

  Pass* fixup = new Fixup();
  fixup->run(Symboltable::getModules(MODULES_ALL));

  if (!fnostdincs) {
    chpl_htuple = dynamic_cast<TypeSymbol*>(Symboltable::lookupInScope("_htuple", tupleModule->modScope));
    chpl_seq = dynamic_cast<TypeSymbol*>(Symboltable::lookupInScope("seq", seqModule->modScope));
  }

  chpl_stdin = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stdin", fileModule->modScope));
  chpl_stdout = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stdout", fileModule->modScope));
  chpl_stderr = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stderr", fileModule->modScope));

  setterToken = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("_setterToken", prelude->modScope));
  methodToken = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("_methodToken", prelude->modScope));

  chpl_true = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("true", prelude->modScope));

  chpl_false = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("false", prelude->modScope));

}
