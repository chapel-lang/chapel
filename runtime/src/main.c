#include "arg.h"
#include "chplmem.h"
#include "chplrt.h"

extern void _chpl_main(void);
extern void CreateConfigVarTable(void);

int main(int argc, char* argv[]) {
  initMemTable();
  CreateConfigVarTable();

  parseArgs(argc, argv);
  _chpl_main();

  return 0;
}
