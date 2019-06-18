// This test is used to verify that llvm.lifetime instrinsics are being emitted
// for procedures that throw an exception

// CHECK: call void @llvm.lifetime.start{{.*}}
// CHECK: call void @llvm.lifetime.end{{.*}}

class EmptyStringError : Error {
  proc init() {}
}

proc mytest_throws(f: string) throws {
  var a = 42;
  var b = a + 42;
  if f.isEmpty() then
    throw new owned EmptyStringError();
  if b != 42 * 2 {
    return a;
  } else {
    return b;
  }
}

mytest_throws("");
