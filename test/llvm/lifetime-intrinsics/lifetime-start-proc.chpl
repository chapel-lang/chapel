// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for stack variables of basic type inside a procedure

// CHECK: %[[REG1:[0-9]+]] = bitcast i64* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
// CHECK: %[[REG2:[0-9]+]] = bitcast i64* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
// CHECK: %[[REG3:[0-9]+]] = bitcast i64* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])

proc mutate(ref arg) {
  arg += 1;
  writeln(arg);
}

proc mytest_proc() {
  var x: int;
  mutate(x);
  {
    var y: int;
    mutate(y);
  }
  var z: int;
  mutate(z);
}

mytest_proc();
