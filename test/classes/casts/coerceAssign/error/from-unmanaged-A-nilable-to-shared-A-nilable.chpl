// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A? to shared A?
  var a = new unmanaged A?();
  var a_ = new shared A?();
  a_ = a;
}
proc main() {
  foo();
}
