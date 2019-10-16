// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for variables of a class type inside a procedure

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

class C {}

proc mytest_class() {
  var x: unmanaged C = new unmanaged C();
  // CHECK: %[[REG1:[0-9]+]] = bitcast %chpl_C_chpl_object** %x_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  refidentity(x);
  delete x;
  // CHECK: %[[REG2:[0-9]+]] = bitcast %chpl_C_chpl_object** %x_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG2]])
}

mytest_class();

