// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A? to borrowed A?
  var a = new shared A?();
  var a_ = a:borrowed A?;
}
proc main() {
  foo();
}
