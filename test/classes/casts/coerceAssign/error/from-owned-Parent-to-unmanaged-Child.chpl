// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent to unmanaged Child
  var a = new owned Parent();
  var a_ = new unmanaged Child();
  a_ = a;
}
proc main() {
  foo();
}
