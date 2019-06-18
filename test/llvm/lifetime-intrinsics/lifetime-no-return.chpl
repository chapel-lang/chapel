// This test is used to verify that llvm.lifetime instrinsics are being
// emitted for procedures having no return statements (ret void)

proc mytest_no_return() {
  const a : int = 32;
  var b : real = 3.99;
  var c = b * 2 + a;
  // CHECK: %[[REG1:[0-9]+]] = bitcast double* %call_tmp_chpl5 to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  // CHECK: %{{[0-9]+}} = bitcast double* %call_tmp_chpl5 to i8*
  // CHECK: %[[REG2:[0-9]+]] = bitcast double* %coerce_tmp_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
  // CHECK: %{{[0-9]+}} = bitcast double* %coerce_tmp_chpl to i8*
  if c > 2 {
    b = 2;
  }
  // CHECK: %[[REG3:[0-9]+]] = bitcast double* %coerce_tmp_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG3]])
  // CHECK: %[[REG4:[0-9]+]] = bitcast double* %call_tmp_chpl5 to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG4]])
  // CHECK-NEXT: ret void
}

mytest_no_return();
