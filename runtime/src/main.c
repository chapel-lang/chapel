#include "arg.h"
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chplthreads.h"

extern void _chpl_main(void);
extern void CreateConfigVarTable(void);

int main(int argc, char* argv[]) {
  parseArgs(argc, argv);
  initMemTable();
  CreateConfigVarTable();
  initChplio();
  initChplThreads();
  _chpl_main();
  exitChplThreads();
  printFinalMemStat();

  return 0;
}
