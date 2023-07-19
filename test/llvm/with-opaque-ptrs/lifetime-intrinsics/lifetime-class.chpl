// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for variables of a class type inside a procedure

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

class C {}

proc mytest_class() {
  var x: unmanaged C = new unmanaged C();
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %x_chpl)
  refidentity(x);
  delete x;
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %x_chpl)
}

mytest_class();

