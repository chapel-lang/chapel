// This test is used to verify that llvm.lifetime.start instrinsic is being emitted
// for stack variables of basic type inside a procedure

// CHECK: %[[REG1:[0-9]+]] = bitcast i64* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
// CHECK: %[[REG2:[0-9]+]] = bitcast i32* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
// CHECK: %[[REG3:[0-9]+]] = bitcast i16* %{{[0-9]+}} to i8*
// CHECK: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG3]])

proc mutate(ref arg) {
  arg += 1;
  writeln(arg);
}

proc mytest_proc() {
  var x: int(64);
  mutate(x);
  {
    var y: int(32);
    mutate(y);
  }
  var z: int(16);
  mutate(z);
}

mytest_proc();
