// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for references in the correct manner pointing to the alloca itself
// and not the memory it points to

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_ref() {
  var someVar: int = 42;
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %someVar_chpl)
  ref toSomeVar : int = someVar;
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %toSomeVar_chpl)
  refidentity(toSomeVar);
  toSomeVar = 112;
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %someVar_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %toSomeVar_chpl)
}

mytest_ref();
