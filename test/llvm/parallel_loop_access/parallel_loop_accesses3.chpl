use CPtr;

config const n = 11;
config param nn = 11;
// check for correct access.group and llvm.loop.parallel_accesses hinting
// for outer and global tuple variables

proc load(arg: int(32)) {
  if arg < 0 then halt("x");
}

// CHECK: void @loop1
proc loop1() {
  var outer: c_array(int(32), nn);
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP1:[0-9]+]]
    outer[i] = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP1]]
    load(outer[i]);
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP1:[0-9]+]]
  }
}
loop1();

// CHECK: ![[LOOP1]] = distinct !{![[LOOP1]], ![[PA1:[0-9]+]]
// CHECK: ![[PA1]] = !{!"llvm.loop.parallel_accesses", ![[GROUP1]]

var globalCArray: c_array(int(32), nn);

// CHECK: void @loop2
proc loop2() {
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP2:[0-9]+]]
    globalCArray[i] = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP2]]
    load(globalCArray[i]);
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP2:[0-9]+]]
  }
}
loop2();
// CHECK: ![[LOOP2]] = distinct !{![[LOOP2]], ![[PA2:[0-9]+]]
// CHECK: ![[PA2]] = !{!"llvm.loop.parallel_accesses", ![[GROUP2]]

// CHECK: void @loop3
proc loop3() {
  var tup: nn * int(32);
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP3:[0-9]+]]
    tup[i] = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP3]]
    load(tup[i]);
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP3:[0-9]+]]
  }
}
loop3();
// CHECK: ![[LOOP3]] = distinct !{![[LOOP3]], ![[PA3:[0-9]+]]
// CHECK: ![[PA3]] = !{!"llvm.loop.parallel_accesses", ![[GROUP3]]

var globalTup: nn * int(32);

// CHECK: void @loop4
proc loop4() {
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP4:[0-9]+]]
    globalTup[i] = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP4]]
    load(globalTup[i]);
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP4:[0-9]+]]
  }
}
loop4();
// CHECK: ![[LOOP4]] = distinct !{![[LOOP4]], ![[PA4:[0-9]+]]
// CHECK: ![[PA4]] = !{!"llvm.loop.parallel_accesses", ![[GROUP4]]

record R {
  var x: int(32);
}

proc loop5() {
  var outer: c_array(R, nn);
  for i in vectorizeOnly(0..n) {
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP5:[0-9]+]]
    outer[i].x = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP5]]
    load(outer[i].x);
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP5:[0-9]+]]
  }
}
loop5();
// CHECK: ![[LOOP5]] = distinct !{![[LOOP5]], ![[PA5:[0-9]+]]
// CHECK: ![[PA5]] = !{!"llvm.loop.parallel_accesses", ![[GROUP5]]

class C {
  var x: int(32);
}

proc loop6() {
  var outer: c_array(int(32), nn);
  for i in vectorizeOnly(0..n) {
    var c = new unmanaged C();
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP6:[0-9]+]]
    c.x = i:int(32);
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP6]]
    load(c.x);
    // intentionally leaks memory (for test simplicity)

    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP6:[0-9]+]]
  }
}
loop6();
// CHECK: ![[LOOP6]] = distinct !{![[LOOP6]], ![[PA6:[0-9]+]]
// CHECK: ![[PA6]] = !{!"llvm.loop.parallel_accesses", ![[GROUP6]]
