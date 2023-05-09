// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures having multiple return statements (gotoReturns)

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_multi_return() {
  // CHECK: %x_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %x_chpl)
  // CHECK: %a_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %a_chpl)
  // CHECK: %y_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %y_chpl)
  // CHECK: %z_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %z_chpl)
  const x = 1.09;
  refidentity(x);
  const a = 1.22;
  refidentity(a);
  if (x == 1.09) {
    var y: real = 1.99;
    var z: real = 1.22;
    z += 0.02 + x;
    y += 0.003 + a;
    if x > 1 {
      return x;
    } else if y > 0 {
      return z;
    }
  }
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %x_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %a_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %y_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %z_chpl)
  return a;
}

mytest_multi_return();
