#include "arg.h"

extern void _chpl_main(void);

int main(int argc, char* argv[]) {

  parseArgs(argc, argv);
  _chpl_main();

  return 0;
}
