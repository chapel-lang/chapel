record R {
  var myfield: integral;
}

{
  var rr: R(int) = new R(0);
  writeln(rr);
}

class MyClass { }
record A {
  var myfield: MyClass?;
}

{
  var aa = new A(new MyClass());
  writeln(aa, " : ", aa.type:string);
}

record B {
  var myfield: MyClass?;
}

{
  var bb: B(owned MyClass?) = new B(nil: owned MyClass?);
  writeln(bb, " : ", bb.type:string);
}

record C {
  var myfield: MyClass;
  proc init() {
    this.myfield = new MyClass();
  }
}

{
  var cc = new C();
}

record D {
  var myfield: owned MyClass;
  proc init() {
    this.myfield = new MyClass();
  }
}

{
  var dd = new D();
}

record GR {
  type t;
}

record E {
  var myfield: GR;
}

{
  var ee = new E(new GR(int));
}

record F {
  var myfield: GR(?);
}

{
  var ff = new F(new GR(int));
}

class GC {
  type t;
}

record I {
  var myfield: owned GC(?);
}

{
  var ii = new I(new GC(int));
}

record GRD {
  type t = int;
}

record J {
  var myfield: GRD;
}

{
  var jj = new J(new GRD(int));
}

record K {
  var myfield: domain;
}

{
  var kk = new K({1..10});
}
