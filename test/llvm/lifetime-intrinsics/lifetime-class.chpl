// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for variables of a class type inside a procedure

// CHECK: %[[REG:[0-9]+]] = bitcast %chpl_C_chpl_object** %new_temp_chpl to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG]])

// CHECK: %[[REG:[0-9]+]] = bitcast %chpl_C_chpl_object** %new_temp_chpl to i8*
// CHECK: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG]])

class C {}

proc mytest_class() {
  var x: unmanaged C = new unmanaged C();
  delete x;
}

mytest_class();

