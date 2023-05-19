// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures that throw an exception

// Simple hack to retain elements to show up in LLVM IR with lifetime instrinsics
proc refidentity(const ref a) const ref { return a; }

class EmptyStringError : Error {
  proc init() {}
}

proc mytest_throws(f: string) throws {
  var a = 42;
  // CHECK: %a_chpl = alloca i64
  // CHECK-NEXT: %[[REG1:[0-9]+]] = bitcast i64* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG1]])
  refidentity(a);
  var b = a + 42;
  // CHECK: %b_chpl = alloca i64
  // CHECK-NEXT: %[[REG2:[0-9]+]] = bitcast i64* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.start.p0i8(i64 8, i8* %[[REG2]])
  refidentity(b);
  // CHECK: %{{[0-9]+}} = bitcast i64* %a_chpl to i8*
  // CHECK: %{{[0-9]+}} = bitcast i64* %b_chpl to i8*
  if f.isEmpty() then
    throw new owned EmptyStringError();
  if b != 42 * 2 {
    return a;
  } else {
    return b;
  }
  // CHECK: %[[REG3:[0-9]+]] = bitcast i64* %a_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG3]])
  // CHECK: %[[REG4:[0-9]+]] = bitcast i64* %b_chpl to i8*
  // CHECK-NEXT: call void @llvm.lifetime.end.p0i8(i64 8, i8* %[[REG4]])
}

mytest_throws("");
