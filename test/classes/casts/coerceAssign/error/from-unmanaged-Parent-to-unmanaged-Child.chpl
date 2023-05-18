// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent to unmanaged Child
  var a = new unmanaged Parent();
  var a_ = new unmanaged Child();
  a_ = a;
}
proc main() {
  foo();
}
