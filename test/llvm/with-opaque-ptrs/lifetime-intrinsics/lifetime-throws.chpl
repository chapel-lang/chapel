// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures that throw an exception

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

class EmptyStringError : Error {
  proc init() {}
}

proc mytest_throws(f: string) throws {
  var a = 42;
  // CHECK: %a_chpl = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %a_chpl)
  refidentity(a);
  var b = a + 42;
  // CHECK: %b_chpl = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %b_chpl)
  refidentity(b);
  if f.isEmpty() then
    throw new owned EmptyStringError();
  if b != 42 * 2 {
    return a;
  } else {
    return b;
  }
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %a_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %b_chpl)
}

mytest_throws("");
