#include "buildBinary.h"
#include "driver.h"
#include "files.h"

void BuildBinary::run(ModuleSymbol* moduleList) {
  if (!suppressCodegen) {
    makeBinary();
  }
}
