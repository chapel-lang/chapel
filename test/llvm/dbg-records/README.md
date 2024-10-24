# dbg-records tests

Starting with LLVM 19, debug information is represented using a new format
called debug records. This replaces the previous debug intrinsics. For our
purposes here, this changed how the debug information is printed in the LLVM IR
and necessitated changes to the debug information tests. The previous tests are
in the `dbg-intrinsics` directory.
