#include "arg.h"
#include "chplcomm.h"
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chplthreads.h"

extern void _chpl_main(void);
extern void CreateConfigVarTable(void);

int main(int argc, char* argv[]) {
  _chpl_comm_init(&argc, &argv);
  parseArgs(argc, argv);
  initMemTable();
  CreateConfigVarTable();
  initChplThreads();
  if (_chpl_comm_locale_id() == 1)
    _chpl_main();
  exitChplThreads();
  printFinalMemStat(0, 0);
  _chpl_comm_done();

  return 0;
}
