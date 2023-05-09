// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for stack variables of basic type inside a procedure

proc mutate(ref arg) {
  arg += 1;
  writeln(arg);
}

proc mytest_proc() {
  // CHECK: %x_chpl = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %x_chpl)
  // CHECK: %y_chpl = alloca i32
  // CHECK: call void @llvm.lifetime.start.p0(i64 4, ptr %y_chpl)
  // CHECK: %z_chpl = alloca i16
  // CHECK: call void @llvm.lifetime.start.p0(i64 2, ptr %z_chpl)
  var x: int(64);
  mutate(x);
  {
    var y: int(32);
    mutate(y);
  }
  var z: int(16);
  mutate(z);
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %x_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 4, ptr %y_chpl)
  // CHECK: call void @llvm.lifetime.end.p0(i64 2, ptr %z_chpl)
}

mytest_proc();
