
@unstable("I am unstable")
interface myInterface {
  proc Self.foo();
  proc Self.bar();
}

record R1: myInterface { // should warn
  proc foo() {
    writeln("foo");
  }
  proc bar() {
    writeln("bar");
  }
}
var r1 = new R1();
r1.foo();
r1.bar();

record R2 {}
R2 implements myInterface; // should warn
proc R2.foo() {
  writeln("foo");
}
proc R2.bar() {
  writeln("bar");
}
var r2 = new R2();
r2.foo();
r2.bar();
