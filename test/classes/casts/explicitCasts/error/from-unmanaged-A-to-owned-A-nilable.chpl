// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged A to owned A?
  var a = new unmanaged A();
  var a_ = a:owned A?;
}
proc main() {
  foo();
}
