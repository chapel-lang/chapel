// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child to borrowed Parent?
  var a = new shared Child();
  var a_ = a:borrowed Parent?;
}
proc main() {
  foo();
}
