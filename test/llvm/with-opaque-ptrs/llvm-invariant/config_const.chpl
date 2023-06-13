// This test checks whether we add llvm.invariant.start to global config const variables

// CHECK: store i64 %{{[0-9]+}}, ptr @configConst_chpl
// CHECK: %{{[0-9]+}} = call ptr @llvm.invariant.start.p0(i64 8, ptr @configConst_chpl)
config const configConst = 1000;
writeln(configConst);
