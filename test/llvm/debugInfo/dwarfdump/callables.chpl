
// DWARFDUMP(skip_subprograms=False): myFunc
proc myFunc(x: int) {
  const ret = x + 1;
  return ret;
}

// DWARFDUMP(skip_subprograms=False): getString
proc getString() {
  const ret = "Hello from getString!";
  return ret;
}

// DWARFDUMP(skip_subprograms=False): myRec
record myRec {
  var a: int;
  var b: real;
  // DWARFDUMP(skip_subprograms=False): toStringRec
  proc toStringRec() {
    const ret = "myRec(" + a:string + ", " + b:string + ")";
    return ret;
  }
}
// DWARFDUMP(skip_subprograms=False): myMethodRec
proc myRec.myMethodRec(y: int): int {
  return a + y;
}
// DWARFDUMP(skip_subprograms=False): setterRec
proc ref myRec.setterRec(b: real) {
  this.b = b;
}
// DWARFDUMP(skip_subprograms=False): getMyRec
proc getMyRec(x: int, y: real) {
  const ret = new myRec(x, y);
  return ret;
}

// DWARFDUMP(skip_subprograms=False): myClass
class myClass {
  var a: int;
  var b: bool;
  var c: string;
  // DWARFDUMP(skip_subprograms=False): toStringCls
  proc toStringCls() {
    const ret = "myClass(" + a:string + ", " + b:string + ", " + c:string + ")";
    return ret;
  }
}
// DWARFDUMP(skip_subprograms=False): myMethodCls
proc myClass.myMethodCls(y: int): int {
  return a + y;
}
// DWARFDUMP(skip_subprograms=False): setterCls
proc ref myClass.setterCls(b: bool) {
  this.b = b;
}
// DWARFDUMP(skip_subprograms=False): getMyClass
proc getMyClass(a: int, b: bool, c: string) {
  const ret = new myClass(a, b, c);
  return ret;
}

proc main() {
  writeln("Calling myFunc(5): ", myFunc(5));
  writeln(getString());
  var r = getMyRec(10, 3.14);
  writeln("myRec: ", r.toStringRec());
  writeln("myRec.myMethodRec(5): ", r.myMethodRec(5));
  r.setterRec(2.71);
  writeln("After setterRec, myRec: ", r.toStringRec());
  var c = getMyClass(42, true, "Chapel");
  writeln("myClass: ", c.toStringCls());
  writeln("myClass.myMethodCls(8): ", c.myMethodCls(8));
  c.setterCls(false);
  writeln("After setterCls, myClass: ", c.toStringCls());
}
