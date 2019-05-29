// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for variables of a class type inside a procedure

// CHECK: %[[REG1:[0-9]+]] = bitcast %_owned_C* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
// CHECK: %[[REG2:[0-9]+]] = bitcast %chpl_C_chpl_object** %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
// CHECK: %[[REG3:[0-9]+]] = bitcast %_owned_C* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])

class C {}

proc mytest_class() {
  var x: owned C = new owned C();
}

mytest_class();

