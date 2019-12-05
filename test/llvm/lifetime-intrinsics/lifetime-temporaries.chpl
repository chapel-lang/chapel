// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for temporaries and hence can change/break in the future when/if the generation
// of these temporaries change

proc mytest_temporaries() {
  var x = 1;
  var y = 2;
  // CHECK: %[[TMP:[a-zA-Z0-9_]+]] = alloca i64
  // CHECK: %[[REG1:[0-9]+]] = bitcast i64* %[[TMP]] to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  // CHECK: %{{[0-9]+}} = bitcast i64* %[[TMP]] to i8*
  // CHECK: %[[REG2:[0-9]+]] = bitcast i64* %[[TMP]] to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG2]])
  return x + y;
}

mytest_temporaries();
