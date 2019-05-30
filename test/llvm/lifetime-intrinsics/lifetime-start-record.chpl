// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for variables of a record type inside a procedure

// CHECK: %[[REG:[0-9]+]] = bitcast %R_chpl* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG]])

record R {}

proc mytest_record() {
  var r = new R();
}

mytest_record();

