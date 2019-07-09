// This test is used to verify that llvm.lifetime instrinsics are being
// emitted for procedures having no return statements (ret void)

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_no_return() {
  const a : int = 32;
  // CHECK: %[[REG1:[0-9]+]] = bitcast i64* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  // CHECK: {{[0-9]+}} = bitcast i64* %a_chpl to i8*
  refidentity(a);
  var b : real = 3.99;
  // CHECK: %[[REG2:[0-9]+]] = bitcast double* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
  refidentity(b);
  var c = b * 2 + a;
  // CHECK: %[[REG3:[0-9]+]] = bitcast double* %c_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])
  refidentity(c);
  if c > 2 {
    b = 2;
  }
  // CHECK: %[[REG4:[0-9]+]] = bitcast i64* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG4]])
  // CHECK: %[[REG5:[0-9]+]] = bitcast double* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG5]])
  // CHECK: %[[REG6:[0-9]+]] = bitcast double* %c_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG6]])
}

mytest_no_return();
