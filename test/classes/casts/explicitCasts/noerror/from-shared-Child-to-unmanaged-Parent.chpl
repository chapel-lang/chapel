// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child to unmanaged Parent
  var a = new shared Child();
  var a_ = a:unmanaged Parent;
}
proc main() {
  foo();
}
