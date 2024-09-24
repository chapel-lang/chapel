# dbg-intrinsics tests

Starting with LLVM 19, debug information is represented using a new format
called debug records. This replaces the previous debug intrinsics. This makes
this directory obsolete for newer LLVM versions, as the tests relied on the old
way of printing debug information in the LLVM IR. The new tests are in the
`dbg-records` directory.
