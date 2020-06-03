// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures with multiple blocks in them

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_multi_blocks() {
  // CHECK: %a_chpl = alloca double
  // CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast double* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  // CHECK: %b_chpl = alloca double
  // CHECK-NEXT: %[[REG2:[0-9]+]] = bitcast double* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
  // CHECK: %c_chpl = alloca double
  // CHECK-NEXT: %[[REG3:[0-9]+]] = bitcast double* %c_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])
  // CHECK: %e_chpl = alloca i64
  // CHECK-NEXT: %[[REG4:[0-9]+]] = bitcast i64* %e_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG4]])
  // CHECK: %test1_chpl = alloca double
  // CHECK-NEXT: %[[REG5:[0-9]+]] = bitcast double* %test1_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG5]])
  // CHECK: %{{[0-9]+}} = bitcast double* %a_chpl to i8*
  // CHECK: %{{[0-9]+}} = bitcast double* %b_chpl to i8*
  // CHECK: %{{[0-9]+}} = bitcast double* %test1_chpl to i8*
  var a = 1.09;
  refidentity(a);
  var b = 11.11;
  refidentity(b);
  {
    var c = 4.0;
    refidentity(c);
    c = a + b + 22.2;
    {
      var e = 42;
      e += e * 2;
    }
  }
  {
    var test1 = 41.0 + b;
    refidentity(test1);
  }
  // CHECK: %[[REG6:[0-9]+]] = bitcast double* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG6]])
  // CHECK: %[[REG7:[0-9]+]] = bitcast double* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG7]])
  // CHECK: %[[REG8:[0-9]+]] = bitcast double* %c_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG8]])
  // CHECK: %[[REG9:[0-9]+]] = bitcast i64* %e_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG9]])
  // CHECK: %[[REG10:[0-9]+]] = bitcast double* %test1_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG10]])
  return a + b;
}

mytest_multi_blocks();
