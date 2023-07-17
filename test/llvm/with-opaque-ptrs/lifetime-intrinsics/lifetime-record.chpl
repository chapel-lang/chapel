// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for variables of a record type inside a procedure

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

record R {}

proc mytest_record() {
  var r = new R();
  // CHECK: %r_chpl = alloca %R_chpl
  // CHECK: call void @llvm.lifetime.start.p0(i64 4, ptr %r_chpl)
  refidentity(r);
  // CHECK: call void @llvm.lifetime.end.p0(i64 4, ptr %r_chpl)
}

mytest_record();
