// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Child to owned Parent?
  var a = new unmanaged Child();
  var a_ = a:owned Parent?;
}
proc main() {
  foo();
}
