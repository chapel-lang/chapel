#include "_chpl_header.h"
void CreateConfigVarTable(void) {
  initConfigVarTable();
  installConfigVar("numLocales", "int", "ChapelBase");
  installConfigVar("maxThreads", "int", "ChapelBase");
  installConfigVar("memRatio", "int", "HPCCProblemSize");
  installConfigVar("n", "int(64)", "fft");
  installConfigVar("epsilon", "real", "fft");
  installConfigVar("threshold", "real", "fft");
  installConfigVar("useRandomSeed", "bool", "fft");
  installConfigVar("seed", "int(64)", "fft");
  installConfigVar("printParams", "bool", "fft");
  installConfigVar("printArrays", "bool", "fft");
  installConfigVar("printStats", "bool", "fft");
  parseConfigArgs();
}
#include "_Program.c"
#include "ChapelBase.c"
#include "ChapelStandard.c"
#include "ChapelLocale.c"
#include "ChapelIO.c"
#include "ChapelTuple.c"
#include "ChapelReduce.c"
#include "ChapelRange.c"
#include "ChapelArray.c"
#include "ChapelUtil.c"
#include "SingleLocaleArithmetic.c"
#include "SingleLocaleAssociative.c"
#include "SingleLocaleSparse.c"
#include "SingleLocaleEnum.c"
#include "SingleLocaleOpaque.c"
#include "Types.c"
#include "Math.c"
#include "List.c"
#include "Time.c"
#include "Random.c"
#include "BitOps.c"
#include "Memory.c"
#include "Schedules.c"
#include "Sort.c"
#include "Search.c"
#include "DistCSR.c"
#include "HPCCProblemSize.c"
#include "fft.c"
#include "_memory_management.c"
