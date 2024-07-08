// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for temporaries and hence can change/break in the future when/if the generation
// of these temporaries change

proc mytest_temporaries() {
  var x = 1;
  var y = 2;
  // CHECK: %[[TMP:[a-zA-Z0-9_]+]] = alloca i64
  // CHECK: call void @llvm.lifetime.start.p0(i64 8, ptr %[[TMP]])
  // CHECK: call void @llvm.lifetime.end.p0(i64 8, ptr %[[TMP]])
  return x + y;
}

mytest_temporaries();
