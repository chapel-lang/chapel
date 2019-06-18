// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures having multiple return statements (gotoReturns)

proc mytest_multi_return() {
  const x = 1.09;
  const a = 1.22;
  {
    var y: real = 1.99;
    var z: real = 1.22;
    z += 0.02 + x;
    y += 0.003 + a;
    // CHECK: %[[REG1:[0-9]+]] = bitcast double* %ret_chpl to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
    // CHECK: %[[REG2:[0-9]+]] = bitcast double* %y_chpl to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
    // CHECK: %[[REG3:[0-9]+]] = bitcast double* %z_chpl to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])
    // CHECK: %[[REG4:[0-9]+]] = bitcast double** %i_lhs_chpl to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG4]])
    // CHECK: %[[REG5:[0-9]+]] = bitcast double** %i_lhs_chpl2 to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG5]])
    // CHECK: %[[REG6:[0-9]+]] = bitcast double* %coerce_tmp_chpl to i8*
    // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG6]])
    if x > 1 {
      // CHECK: %[[REG7:[0-9]+]] = bitcast double* %y_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG7]])
      // CHECK: %[[REG8:[0-9]+]] = bitcast double* %z_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG8]])
      // CHECK: %[[REG9:[0-9]+]] = bitcast double* %coerce_tmp_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG9]])
      // CHECK: %[[REG10:[0-9]+]] = bitcast double** %i_lhs_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG10]])
      // CHECK: %[[REG11:[0-9]+]] = bitcast double** %i_lhs_chpl2 to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG11]])
      // CHECK: br label %_end{{.*}}
      return x;
    } else if y > 0
      // CHECK: %[[REG12:[0-9]+]] = bitcast double* %coerce_tmp_chpl2 to i8*
      // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG12]])
    {
      // CHECK: %[[REG13:[0-9]+]] = bitcast double* %y_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG13]])
      // CHECK: %[[REG14:[0-9]+]] = bitcast double* %z_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG14]])
      // CHECK: %[[REG15:[0-9]+]] = bitcast double* %coerce_tmp_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG15]])
      // CHECK: %[[REG16:[0-9]+]] = bitcast double** %i_lhs_chpl to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG16]])
      // CHECK: %[[REG17:[0-9]+]] = bitcast double** %i_lhs_chpl2 to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG17]])
      // CHECK: %[[REG18:[0-9]+]] = bitcast double* %coerce_tmp_chpl2 to i8*
      // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG18]])
      // CHECK: br label %_end{{.*}}
      return z;
    }
  }
  // CHECK: %[[REG19:[0-9]+]] = bitcast double* %ret_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG19]])
  return a;
}

mytest_multi_return();
