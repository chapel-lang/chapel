module ChapelEnv {
  param CHPL_HOME            = __primitive("get compiler variable", "CHPL_HOME");
  param CHPL_HOST_PLATFORM   = __primitive("get compiler variable", "CHPL_HOST_PLATFORM");
  param CHPL_TARGET_PLATFORM = __primitive("get compiler variable", "CHPL_TARGET_PLATFORM");
  param CHPL_HOST_COMPILER   = __primitive("get compiler variable", "CHPL_HOST_COMPILER");
  param CHPL_TARGET_COMPILER = __primitive("get compiler variable", "CHPL_TARGET_COMPILER");
  param CHPL_TARGET_ARCH     = __primitive("get compiler variable", "CHPL_TARGET_ARCH");
  param CHPL_LOCALE_MODEL    = __primitive("get compiler variable", "CHPL_LOCALE_MODEL");
  param CHPL_COMM            = __primitive("get compiler variable", "CHPL_COMM");
  param CHPL_COMM_SUBSTRATE  = __primitive("get compiler variable", "CHPL_COMM_SUBSTRATE");
  param CHPL_GASNET_SEGMENT  = __primitive("get compiler variable", "CHPL_GASNET_SEGMENT");
  param CHPL_TASKS           = __primitive("get compiler variable", "CHPL_TASKS");
  param CHPL_THREADS         = __primitive("get compiler variable", "CHPL_THREADS");
  param CHPL_LAUNCHER        = __primitive("get compiler variable", "CHPL_LAUNCHER");
  param CHPL_TIMERS          = __primitive("get compiler variable", "CHPL_TIMERS");
  param CHPL_MEM             = __primitive("get compiler variable", "CHPL_MEM");
  param CHPL_MAKE            = __primitive("get compiler variable", "CHPL_MAKE");
  param CHPL_ATOMICS         = __primitive("get compiler variable", "CHPL_ATOMICS");
  param CHPL_NETWORK_ATOMICS = __primitive("get compiler variable", "CHPL_NETWORK_ATOMICS");
  param CHPL_GMP             = __primitive("get compiler variable", "CHPL_GMP");
  param CHPL_HWLOC           = __primitive("get compiler variable", "CHPL_HWLOC");
  param CHPL_REGEXP          = __primitive("get compiler variable", "CHPL_REGEXP");
  param CHPL_WIDE_POINTERS   = __primitive("get compiler variable", "CHPL_WIDE_POINTERS");
  param CHPL_LLVM            = __primitive("get compiler variable", "CHPL_LLVM");
  param CHPL_AUX_FILESYS     = __primitive("get compiler variable", "CHPL_AUX_FILESYS");
}
