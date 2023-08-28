#include "error.h"

const int mainHasArgs = 0;
const int mainPreserveDelimiter = 0;
const int warnUnstable = 0;
void CreateConfigVarTable(void) {
  initConfigVarTable();
  installConfigVar("printModuleInitOrder", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "printModuleInitOrder is deprecated"
                   , /* unstable = */ 0, "printModuleInitOrder is unstable"
                   );
  installConfigVar("dataParTasksPerLocale", "int(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "dataParTasksPerLocale is deprecated"
                   , /* unstable = */ 0, "dataParTasksPerLocale is unstable"
                   );
  installConfigVar("dataParIgnoreRunningTasks", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "dataParIgnoreRunningTasks is deprecated"
                   , /* unstable = */ 0, "dataParIgnoreRunningTasks is unstable"
                   );
  installConfigVar("dataParMinGranularity", "int(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "dataParMinGranularity is deprecated"
                   , /* unstable = */ 0, "dataParMinGranularity is unstable"
                   );
  installConfigVar("memTrack", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memTrack is deprecated"
                   , /* unstable = */ 0, "memTrack is unstable"
                   );
  installConfigVar("memStats", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memStats is deprecated"
                   , /* unstable = */ 0, "memStats is unstable"
                   );
  installConfigVar("memLeaksByType", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memLeaksByType is deprecated"
                   , /* unstable = */ 0, "memLeaksByType is unstable"
                   );
  installConfigVar("memLeaks", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memLeaks is deprecated"
                   , /* unstable = */ 0, "memLeaks is unstable"
                   );
  installConfigVar("memMax", "uint(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memMax is deprecated"
                   , /* unstable = */ 0, "memMax is unstable"
                   );
  installConfigVar("memThreshold", "uint(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memThreshold is deprecated"
                   , /* unstable = */ 0, "memThreshold is unstable"
                   );
  installConfigVar("memLog", "string", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memLog is deprecated"
                   , /* unstable = */ 0, "memLog is unstable"
                   );
  installConfigVar("memLeaksLog", "string", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memLeaksLog is deprecated"
                   , /* unstable = */ 0, "memLeaksLog is unstable"
                   );
  installConfigVar("memLeaksByDesc", "string", "Built-in", /* private = */ 0, /* deprecated = */ 0, "memLeaksByDesc is deprecated"
                   , /* unstable = */ 0, "memLeaksByDesc is unstable"
                   );
  installConfigVar("debugGpu", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "debugGpu is deprecated"
                   , /* unstable = */ 0, "debugGpu is unstable"
                   );
  installConfigVar("gpuNoCpuModeWarning", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "gpuNoCpuModeWarning is deprecated"
                   , /* unstable = */ 0, "gpuNoCpuModeWarning is unstable"
                   );
  installConfigVar("enableGpuP2P", "bool", "Built-in", /* private = */ 0, /* deprecated = */ 0, "enableGpuP2P is deprecated"
                   , /* unstable = */ 0, "enableGpuP2P is unstable"
                   );
  installConfigVar("defaultHashTableResizeThreshold", "real(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "defaultHashTableResizeThreshold is deprecated"
                   , /* unstable = */ 0, "defaultHashTableResizeThreshold is unstable"
                   );
  installConfigVar("numLocales", "int(64)", "Built-in", /* private = */ 0, /* deprecated = */ 0, "numLocales is deprecated"
                   , /* unstable = */ 0, "numLocales is unstable"
                   );
  installConfigVar("numTrainImages", "int(64)", "demo_cnn", /* private = */ 0, /* deprecated = */ 0, "numTrainImages is deprecated"
                   , /* unstable = */ 0, "numTrainImages is unstable"
                   );
  installConfigVar("numTestImages", "int(64)", "demo_cnn", /* private = */ 0, /* deprecated = */ 0, "numTestImages is deprecated"
                   , /* unstable = */ 0, "numTestImages is unstable"
                   );
  installConfigVar("learnRate", "real(64)", "demo_cnn", /* private = */ 0, /* deprecated = */ 0, "learnRate is deprecated"
                   , /* unstable = */ 0, "learnRate is unstable"
                   );
  installConfigVar("batchSize", "int(64)", "demo_cnn", /* private = */ 0, /* deprecated = */ 0, "batchSize is deprecated"
                   , /* unstable = */ 0, "batchSize is unstable"
                   );
  installConfigVar("numEpochs", "int(64)", "demo_cnn", /* private = */ 0, /* deprecated = */ 0, "numEpochs is deprecated"
                   , /* unstable = */ 0, "numEpochs is unstable"
                   );
  installConfigVar("dataPath", "string", "MNISTTester", /* private = */ 0, /* deprecated = */ 0, "dataPath is deprecated"
                   , /* unstable = */ 0, "dataPath is unstable"
                   );
}


