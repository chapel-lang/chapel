// This module exists just exist to make sure the compiler doesn't mark
// certain test files as 'main modules' because they're the only file in the
// compilation command. This is relevant for warnings that we explicitly want
// silenced for main files. This file is marked 'main' instead.
//
// We write this file to emit a warning about its own implicit module --
// but since this module is marked 'main', the warning should be silenced.

var anotherModuleVar = 42;

module SubModule {}
