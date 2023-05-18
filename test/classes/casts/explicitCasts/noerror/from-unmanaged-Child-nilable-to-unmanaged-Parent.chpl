// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Child? to unmanaged Parent
  var a = new unmanaged Child?();
  var a_ = a:unmanaged Parent;
}
proc main() {
  foo();
}
