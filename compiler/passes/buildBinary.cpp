#include "buildBinary.h"
#include "driver.h"
#include "files.h"

void BuildBinary::run(Vec<ModuleSymbol*>* modules) {
  if (!suppressCodegen) {
    makeBinary();
  }
}
