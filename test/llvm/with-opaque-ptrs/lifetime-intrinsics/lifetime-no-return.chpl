// This test is used to verify that llvm.lifetime instrinsics are being
// emitted for procedures having no return statements (ret void)

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_no_return() {
  // CHECK: %a_chpl = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %a_chpl)
  // CHECK: %b_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %b_chpl)
  // CHECK: %c_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %c_chpl)
  const a : int = 32;
  refidentity(a);
  var b : real = 3.99;
  refidentity(b);
  var c = b * 2 + a;
  refidentity(c);
  if c > 2 {
    b = 2;
  }
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %a_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %b_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %c_chpl)
}

mytest_no_return();
