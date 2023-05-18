// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A to shared A
  var a = new shared A();
  var a_ = new shared A();
  a_ = a;
}
proc main() {
  foo();
}
