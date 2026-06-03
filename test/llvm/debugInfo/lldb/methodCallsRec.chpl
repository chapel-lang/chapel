use Debugger;

record myRec {
  var a: int;
  var b: real;
  proc toString() {
    const ret = "myRec(" + a:string + ", " + b:string + ")";
    return ret;
  }
}
proc myRec.myMethod(y: int): int {
  return a + y;
}
proc ref myRec.setter(b: real) {
  this.b = b;
}

proc main() {
  var myObj = new myRec(10, 3.14);
  writeln("Calling myMethod(5): ", myObj.myMethod(5));
  myObj.setter(2.71);
  writeln("After setter, myObj: ", myObj.toString());

  breakpoint;
}

// CHECK: b methodCallsRec.chpl:8
// CHECK-NEXT: methodCallsRec`toString
// CHECK-SAME: methodCallsRec.chpl:8
// CHECK: b methodCallsRec.chpl:12
// CHECK-NEXT: methodCallsRec`myMethod
// CHECK-SAME: methodCallsRec.chpl:12
// CHECK: b methodCallsRec.chpl:15
// CHECK-NEXT: methodCallsRec`setter
// CHECK-SAME: methodCallsRec.chpl:15

// CHECK: stop reason = breakpoint 3
// CHECK: p this
// CHECK-NEXT: (_ref(myRec)) 0x{{0*[0-9a-fA-F]+}} (a = 10, b = 3.14
// CHECK: c

// CHECK: stop reason = breakpoint 4
// CHECK: p *this
// CHECK-NEXT: (methodCallsRec::myRec){{.*}}(a = 10, b = 3.14
// CHECK: c

// CHECK: stop reason = breakpoint 2
// CHECK: p this->a
// CHECK-NEXT: (int(64)) 10
// CHECK: p this.a
// CHECK-NEXT: (int(64)) 10
// CHECK: frame var
// CHECK-NEXT: (_ref(myRec)) this = 0x{{0*[0-9a-fA-F]+}} (a = 10, b = 2.71
// CHECK-NEXT: (string) ret = "myRec(10, 2.71)"
// CHECK: p ret
// CHECK-NEXT: (string) "myRec(10, 2.71)"
// CHECK: c

// CHECK: p myObj
// CHECK-NEXT: (methodCallsRec::myRec){{.*}}(a = 10, b = 2.71

// CHECK: p myObj.myMethod(2)
// CHECK-NEXT: (int(64)) 12

// CHECK: p myObj.setter(8.1)
// CHECK: p myObj
// CHECK-NEXT: (methodCallsRec::myRec){{.*}}(a = 10, b = 8.
