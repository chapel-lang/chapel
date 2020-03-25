// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for references in the correct manner pointing to the alloca itself
// and not the memory it points to

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_ref() {
  var someVar: int = 42;
  // CHECK: %[[REG1:[0-9]+]] = bitcast i64* %someVar_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  ref toSomeVar : int = someVar;
  // CHECK: %[[REG2:[0-9]+]] = bitcast i64** %toSomeVar_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
  refidentity(toSomeVar);
  toSomeVar = 112;
  // CHECK: %[[REG3:[0-9]+]] = bitcast i64* %someVar_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG3]])
  // CHECK: %[[REG4:[0-9]+]] = bitcast i64** %toSomeVar_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG4]])
}

mytest_ref();
