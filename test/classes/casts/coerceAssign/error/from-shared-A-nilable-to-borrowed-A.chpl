// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A? to borrowed A
  var a = new shared A?();
  var allocTo = new owned A();
  var a_:borrowed A = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
