use Debugger;

class myClass {
  var a: int;
  var b: real;
  proc toString() {
    const ret = "myClass(" + a:string + ", " + b:string + ")";
    return ret;
  }
}
proc myClass.myMethod(y: int): int {
  return a + y;
}
proc myClass.setter(b: real) {
  this.b = b;
}
proc main() {
  var myOwnedObj = new myClass(10, 3.14);
  writeln("Calling myMethod(5): ", myOwnedObj.myMethod(5));
  myOwnedObj.setter(2.71);
  writeln("After setter, myOwnedObj: ", myOwnedObj.toString());

  var myUnmanagedObj = new unmanaged myClass(20, 1.61); defer { delete myUnmanagedObj; }
  writeln("Calling myMethod(10): ", myUnmanagedObj.myMethod(10));
  myUnmanagedObj.setter(0.577);
  writeln("After setter, myUnmanagedObj: ", myUnmanagedObj.toString());

  breakpoint;
}

// CHECK: b methodCallsClass.chpl:8
// CHECK-NEXT: methodCallsClass`toString
// CHECK-SAME: methodCallsClass.chpl:8
// CHECK: b methodCallsClass.chpl:12
// CHECK-NEXT: methodCallsClass`myMethod
// CHECK-SAME: methodCallsClass.chpl:12
// CHECK: b methodCallsClass.chpl:15
// CHECK-NEXT: methodCallsClass`setter
// CHECK-SAME: methodCallsClass.chpl:15

// CHECK: stop reason = breakpoint 3
// CHECK: p this
// CHECK-NEXT: (methodCallsClass::myClass *) 0x{{0*[0-9a-fA-F]+}}
// CHECK: c

// CHECK: stop reason = breakpoint 4
// CHECK: p *this
// CHECK-NEXT: (methodCallsClass::myClass) {
// CHECK-NEXT: super = (cid =
// CHECK-NEXT: a = 10
// CHECK-NEXT: b = 3.14
// CHECK-NEXT: }
// CHECK: c

// CHECK: stop reason = breakpoint 2
// CHECK: p this->a
// CHECK-NEXT: (int(64)) 10
// CHECK: p ret
// CHECK-NEXT: (string) "myClass(10, 2.71)"
// CHECK: c

// CHECK: stop reason = breakpoint 3
// CHECK: c

// CHECK: stop reason = breakpoint 4
// CHECK: c

// CHECK: stop reason = breakpoint 2
// CHECK: frame var
// CHECK-NEXT: (methodCallsClass::myClass *) this = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: (string) ret = "myClass(20, 0.577)"

// CHECK: p myOwnedObj
// CHECK-NEXT: (owned myClass) 0x{{0*[0-9a-fA-F]+}} {
// CHECK-NEXT: super = (cid =
// CHECK-NEXT: a = 10
// CHECK-NEXT: b = 2.71
// CHECK-NEXT: }

// CHECK: p myOwnedObj.chpl_p->myMethod(2)
// CHECK-NEXT: (int(64)) 12

// CHECK: p myOwnedObj.chpl_p->setter(8.1)
// CHECK: p myOwnedObj
// CHECK-NEXT: (owned myClass) 0x{{0*[0-9a-fA-F]+}} {
// CHECK-NEXT: super = (cid =
// CHECK-NEXT: a = 10
// CHECK-NEXT: b = 8.
// CHECK-NEXT: }

// CHECK: p *myUnmanagedObj
// CHECK-NEXT: (methodCallsClass::myClass) {
// CHECK-NEXT: super = (cid =
// CHECK-NEXT: a = 20
// CHECK-NEXT: b = 0.5
// CHECK-NEXT: }

// CHECK: p myUnmanagedObj->myMethod(3)
// CHECK-NEXT: (int(64)) 23

// CHECK: p myUnmanagedObj->setter(9.2)
// CHECK: p *myUnmanagedObj
// CHECK-NEXT: (methodCallsClass::myClass) {
// CHECK-NEXT: super = (cid =
// CHECK-NEXT: a = 20
// CHECK-NEXT: b = 9.
