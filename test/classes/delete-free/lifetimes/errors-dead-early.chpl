class MyClass { var x: int; }

proc test1() {
  var x: borrowed MyClass? = nil;
  x = (new owned MyClass()).borrow();
  // owned class deinited here
  writeln(x); // use after free
}
test1();

proc acceptsIn(in arg) { }

proc identity(arg) { return arg; }
proc test2() {
  var x = new MyClass(1);
  var b = identity(x.borrow());
  acceptsIn(x); // copy elided
  writeln(b);
}
test2();

proc refIdentity(ref arg) ref { return arg; }
proc test3() {
  var x = new MyClass(1);
  ref r = refIdentity(x);
  acceptsIn(x); // copy elided
  writeln(r);
}
test3();

proc test4() {
  var arr: [1..1] borrowed MyClass?;
  arr[1] = new MyClass(1);
  writeln(arr);
}
test4();

record MyRecord {
  var c: borrowed MyClass?;
}

proc test5() {
  var x: MyRecord;
  x.c = new borrowed MyClass(1);
  writeln(x);
}
test5();

proc test6() {
  var a:borrowed MyClass? = nil;
  a = new borrowed MyClass();
}
test6();

config param printInitDeinit = true;

class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
  proc ref set1() ref {
    this.x = 1;
    this.ptr.xx = 1;
    return this;
  }
}
proc =(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}

proc concatenate(a: R, b: R): string {
  return a.toString() + " " + b.toString();
}

proc concatenate(a: R, b: borrowed C): string {
  return a.toString() + " " + b.xx:string;
}

proc concatenate(a: R, b: int): string {
  return a.toString() + " " + b:string;
}

proc copy(in arg) {
  return arg;
}

proc test7() {
  var x = new R(1);
  writeln("test5b.a");
  var b = x.ptr.borrow();
  writeln(concatenate(copy(x), b));
}
test7();

proc test8() { // should be an error too, one day
  var x = new R(1);
  const ref rx = x.ptr.xx;
  writeln(concatenate(copy(x), rx));
}
test8();
