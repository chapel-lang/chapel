// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A? to borrowed A
  var a = new shared A?();
  var a_:borrowed A = a;
}
proc main() {
  foo();
}
