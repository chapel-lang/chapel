// This test just verifies that llvm.ident
// has the Chapel version as its value in the IR

// CHECK: !llvm.ident = !{![[LLVM_IDENT:[0-9]+]]}
// CHECK: ![[LLVM_IDENT]] = !{!"Chapel version {{.*}}"}
proc mytest() {
  return 1;
}

writeln(mytest());
