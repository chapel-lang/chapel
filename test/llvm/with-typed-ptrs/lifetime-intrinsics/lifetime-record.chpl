// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for variables of a record type inside a procedure

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

record R {}

proc mytest_record() {
  var r = new R();
  // CHECK: %r_chpl = alloca %R_chpl
  // CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast %R_chpl* %r_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 4, i8* %[[REG1]])
  // CHECK: %{{[0-9]+}} = bitcast %R_chpl* %r_chpl to i8*
  refidentity(r);
  // CHECK: %[[REG2:[0-9]+]] = bitcast %R_chpl* %r_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 4, i8* %[[REG2]])
}

mytest_record();
