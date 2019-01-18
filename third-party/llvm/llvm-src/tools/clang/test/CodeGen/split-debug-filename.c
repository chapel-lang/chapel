// REQUIRES: x86-registered-target
// RUN: %clang_cc1 -debug-info-kind=limited -split-dwarf-file foo.dwo -S -emit-llvm -o - %s | FileCheck %s
// RUN: %clang_cc1 -debug-info-kind=limited -enable-split-dwarf -split-dwarf-file foo.dwo -S -emit-llvm -o - %s | FileCheck --check-prefix=VANILLA %s
// RUN: %clang_cc1 -triple x86_64-unknown-linux -debug-info-kind=limited -enable-split-dwarf -split-dwarf-file %t.dwo -emit-obj -o - %s | llvm-objdump -section-headers - | FileCheck --check-prefix=O %s
// RUN: llvm-objdump -section-headers %t.dwo | FileCheck --check-prefix=DWO %s

int main (void) {
  return 0;
}

// Testing to ensure that the dwo name gets output into the compile unit.
// CHECK: !DICompileUnit({{.*}}, splitDebugFilename: "foo.dwo"

// Testing to ensure that the dwo name is not output into the compile unit if
// it's for vanilla split-dwarf rather than split-dwarf for implicit modules.
// VANILLA-NOT: splitDebugFilename

// O-NOT: .dwo
// DWO: .dwo
