// This test checks whether we add llvm.invariant.start to global config const variables

// CHECK: store i64 %{{[0-9]+}}, i64* @configConst_chpl
// CHECK: %{{[0-9]+}} = call {}* @llvm.invariant.start.p0i8(i64 8, i8* bitcast (i64* @configConst_chpl to i8*))
config const configConst = 1000;
writeln(configConst);
