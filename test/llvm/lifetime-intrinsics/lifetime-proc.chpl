// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for stack variables of basic type inside a procedure

proc mutate(ref arg) {
  arg += 1;
  writeln(arg);
}

proc mytest_proc() {
  // CHECK: %x_chpl = alloca i64
  // CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast i64* %x_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  // CHECK: %y_chpl = alloca i32
  // CHECK-NEXT: %[[REG2:[0-9]+]] = bitcast i32* %y_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 4, i8* %[[REG2]])
  // CHECK: %z_chpl = alloca i16
  // CHECK-NEXT: %[[REG3:[0-9]+]] = bitcast i16* %z_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 2, i8* %[[REG3]])
  var x: int(64);
  mutate(x);
  {
    var y: int(32);
    mutate(y);
  }
  var z: int(16);
  mutate(z);
  // CHECK: %[[REG4:[0-9]+]] = bitcast i64* %x_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG4]])
  // CHECK: %[[REG5:[0-9]+]] = bitcast i32* %y_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 4, i8* %[[REG5]])
  // CHECK: %[[REG6:[0-9]+]] = bitcast i16* %z_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 2, i8* %[[REG6]])
}

mytest_proc();
