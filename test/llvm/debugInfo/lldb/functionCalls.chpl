use Debugger;
proc myFunc(x: int) {
  const ret = x + 1;
  return ret;
}
// TODO: resolve https://github.com/chapel-lang/chapel/issues/28320 to make this
// work in the debugger
proc getString() {
  const ret = "Hello from getString!";
  return ret;
}
record myRec {
  var a: int;
  var b: real;
}
// TODO: resolve https://github.com/chapel-lang/chapel/issues/28320 to make this
// work in the debugger
proc getMyRec(x: int, y: real) {
  const ret = new myRec(x, y);
  return ret;
}
record myRec2 {
  var a: int;
  var b: real;
  var c: bool;
}
// TODO: resolve https://github.com/chapel-lang/chapel/issues/28320 to make this
// work in the debugger
proc getMyRec2(x: int, y: real, z: bool) {
  const ret = new myRec2(x, y, z);
  return ret;
}
class MyClass {
  var s: string;
}
proc getMyClass() {
  const ret = new MyClass("This is MyClass");
  return ret;
}
proc main() {
  writeln("Calling myFunc(5): ", myFunc(5));
  writeln(getString());
  writeln(getMyRec(10, 3.14));
  writeln(getMyRec2(20, 2.71, true));
  writeln(getMyClass());
  breakpoint;
}

// CHECK: b myFunc
// CHECK-NEXT: functionCalls`myFunc
// CHECK-SAME: functionCalls.chpl:2
// CHECK: b functionCalls.chpl:4
// CHECK-NEXT: functionCalls`myFunc
// CHECK-SAME: functionCalls.chpl:4

// CHECK: b getString
// CHECK: b functionCalls.chpl:10
// CHECK-NEXT: functionCalls`getString
// CHECK-SAME: functionCalls.chpl:10

// CHECK: b getMyRec
// CHECK-NEXT: functionCalls`getMyRec
// CHECK-SAME: functionCalls.chpl:18
// CHECK: b functionCalls.chpl:20
// CHECK-NEXT: functionCalls`getMyRec
// CHECK-SAME: functionCalls.chpl:20

// CHECK: b getMyRec2
// CHECK-NEXT: functionCalls`getMyRec2
// CHECK-SAME: functionCalls.chpl:29
// CHECK: b functionCalls.chpl:31
// CHECK-NEXT: functionCalls`getMyRec2
// CHECK-SAME: functionCalls.chpl:31

// CHECK: b getMyClass
// CHECK-NEXT: functionCalls`getMyClass
// CHECK-SAME: functionCalls.chpl:36
// CHECK: b functionCalls.chpl:38
// CHECK-NEXT: functionCalls`getMyClass
// CHECK-SAME: functionCalls.chpl:38

// CHECK: stop reason = breakpoint 2
// CHECK: stop reason = breakpoint 3
// CHECK: p ret
// CHECK: (int(64)) 6

// CHECK: stop reason = breakpoint 4
// CHECK: stop reason = breakpoint 5
// CHECK: p ret
// CHECK: (string) "Hello from getString!"

// CHECK: stop reason = breakpoint 6
// CHECK: stop reason = breakpoint 7
// CHECK: p ret
// CHECK: (functionCalls::myRec)  (a = 10, b = 3.14

// CHECK: stop reason = breakpoint 8
// CHECK: stop reason = breakpoint 9
// CHECK: p ret
// CHECK: (functionCalls::myRec2)  (a = 20, b = 2.71, c = true)

// CHECK: stop reason = breakpoint 10
// CHECK: stop reason = breakpoint 11
// CHECK: p ret
// CHECK: (owned MyClass) 0x{{[0-9a-f]+}} {
// CHECK-NEXT: super
// CHECK-NEXT: s = "This is MyClass"

// CHECK: p myFunc(10)
// CHECK-NEXT: (int(64)) 11
// CHECK: p getMyClass()
// TODO: need struct returns to work properly for this with `--no-local`
// CHECK-LOCAL-NEXT: (owned MyClass) 0x{{[0-9a-f]+}} {
// CHECK-LOCAL-NEXT: super
// CHECK-LOCAL-NEXT: s = "This is MyClass"
