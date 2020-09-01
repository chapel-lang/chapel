use CPtr;

config const n = 11;
config param nn = 11;
// check for correct access.group and llvm.loop.parallel_accesses hinting

proc load(arg: int(32)) {
  if arg < 0 then halt("x");
}

proc keep(ref arg) { }
proc mark() { }

// CHECK: void @loop1
proc loop1(A, B) {
  // Check that we don't generate llvm.access.group metadata in
  // non-order-independent loops
  for i in 0..n {
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    A[i] = B[i]*3;
  }
}
{
  var A : [0..n] int(32);
  var B : [0..n] int(32);
  loop1(A,B);
}

// Check that we don't generate llvm.access.group metadata in
// loads/stores for stack local variables in the loop.

// CHECK: void @loop2
proc loop2() {
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    var tmp: int(32) = i:int(32);
    keep(tmp);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(tmp);
  }
}
loop2();

// CHECK: void @loop3
proc loop3() {
  for i in vectorizeOnly(0..n) {
    var tmp: c_array(int(32), nn);
    // CHECK: mark
    mark();
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    tmp[1] = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(tmp[1]);
  }
}
loop3();

// CHECK: void @loop4
proc loop4() {
  for i in vectorizeOnly(0..n) {
    var tmp: nn*int(32);
    // CHECK: mark
    mark();
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    tmp[1] = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(tmp[1]);
  }
}
loop4();

record R {
  var x: int(32);
}

// CHECK: void @loop5
proc loop5() {
  for i in vectorizeOnly(0..n) {
    var tmp: R;
    // CHECK: mark
    mark();
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    tmp.x = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(tmp.x);
  }
}
loop5();

// CHECK: void @loop6
proc loop6() {
  for i in vectorizeOnly(0..n) {
    var tmp: R;
    // CHECK: mark
    mark();
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    (tmp, 1)[0].x = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load((tmp, 1)[0].x);
  }
}
loop6();

// CHECK: void @loop7
proc loop7() {
  for i in vectorizeOnly(0..n) {
    var tmp: R;
    // CHECK: mark
    mark();
    ref rr = (tmp, 1)[0];
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    rr.x = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(rr.x);
  }
}
loop7();

// CHECK: void @loop8
proc loop8() {
  for i in vectorizeOnly(0..n) {
    var tmp: R;
    // CHECK: mark
    mark();
    ref rr = (tmp, 1)[0].x;
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    rr = i:int(32);
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    load(rr);
  }
}
loop8();
