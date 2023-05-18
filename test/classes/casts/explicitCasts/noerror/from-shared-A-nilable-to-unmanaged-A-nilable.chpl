// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A? to unmanaged A?
  var a = new shared A?();
  var a_ = a:unmanaged A?;
}
proc main() {
  foo();
}
