#include "buildBinary.h"
#include "driver.h"
#include "files.h"

void BuildBinary::run(ModuleList* moduleList) {
  if (!suppressCodegen) {
    makeBinary();
  }
}
