// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures with multiple blocks in them

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

proc mytest_multi_blocks() {
  // CHECK: %a_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %a_chpl)
  // CHECK: %b_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %b_chpl)
  // CHECK: %c_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %c_chpl)
  // CHECK: %e_chpl = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %e_chpl)
  // CHECK: %test1_chpl = alloca double
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %test1_chpl)
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
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %a_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %b_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %c_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %e_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %test1_chpl)
  return a + b;
}

mytest_multi_blocks();
