// This test checks whether we add llvm.invariant.start to global config const variables

// Match something like this
//  %26 = bitcast i64* @configConst_chpl to i8*
//  %27 = call {}* @llvm.invariant.start.p0i8(i64 8, i8* %26)

// This used to match
//   call {}* @llvm.invariant.start.p0i8(i64 8, i8* bitcast (i64* @configConst_chpl to i8*))
// and it would be fine if it went back to that for some reason.

// CHECK-DAG: %[[R:[0-9]+]] = bitcast i64* @configConst_chpl to i8*
// CHECK-DAG: call {}* @llvm.invariant.start.p0i8(i64 8, i8* %[[R]])
config const configConst = 1000;
writeln(configConst);
