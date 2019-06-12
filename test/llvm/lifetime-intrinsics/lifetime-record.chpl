// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for variables of a record type inside a procedure

// CHECK: %[[REG1:[0-9]+]] = bitcast %R_chpl* %initTemp_chpl to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 4, i8* %[[REG1]])

// CHECK: %[[REG2:[0-9]+]] = bitcast %R_chpl* %initTemp_chpl to i8*
// CHECK: %[[REG3:[0-9]+]] = bitcast %R_chpl* %initTemp_chpl to i8*
// CHECK: call void @llvm.lifetime.end.p0i8(i64 4, i8* %[[REG3]])

record R {}

proc mytest_record() {
  var r = new R();
}

mytest_record();

