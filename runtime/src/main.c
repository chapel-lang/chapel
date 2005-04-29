#include "arg.h"
#include "chplmem.h"
#include "chplrt.h"

extern void _chpl_main(void);
extern void CreateConfigVarTable(void);

int main(int argc, char* argv[]) {
  parseArgs(argc, argv);
  initMemTable();
  CreateConfigVarTable();

  _chpl_main();
  printFinalMemStat();

  return 0;
}
